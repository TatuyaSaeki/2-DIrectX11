//===========================================================================
//! @file	system_render.cpp
//!	@brief	DirectX11描画システム管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SystemRender::setup(u32 width, u32 height)
{
    HRESULT hr = S_OK;

    //=============================================================
    //	描画ターゲットの作成
    //=============================================================
    {
        // スワップチェインの中にあるバックバッファメモリーを取得
        ID3D11Texture2D* d3dRenderTarget = nullptr;
        hr                               = DirectX11::getInstance().getD3DSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dRenderTarget);
        if(hr != S_OK)
            return false;

        // バックバッファ作成
        backBuffer_.reset(gpu::createTexture(d3dRenderTarget));
    }

    // HDRバッファの作成
    for(u32 i = 0; i < 2; ++i) {
        auto hdr = gpu::createRenderTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT);
        hdrBuffer_[i].reset(hdr);
    }
    // SDRバッファの作成
    for(u32 i = 0; i < 2; ++i) {
        auto sdr = gpu::createRenderTarget(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
        sdrBuffer_[i].reset(sdr);
    }

    // HDRワークバッファの作成
    for(auto& t : hdrWorkBuffer_) {
        for(u32 mip = 0; mip < std::size(t); ++mip) {
            u32 w = std::max(1u, width >> mip);    // 1/2 ずつしていく
            u32 h = std::max(1u, height >> mip);   // 1/2 ずつしていく

            auto hdr = gpu::createRenderTarget(w, h, DXGI_FORMAT_R11G11B10_FLOAT);
            t[mip].reset(hdr);
        }
    }

    //=============================================================
    // Ｚバッファの作成
    //=============================================================
    {
        //---- GPUメモリの確保(Texture2D)
        D3D11_TEXTURE2D_DESC desc{};

        desc.Width              = width;                                                       // 幅
        desc.Height             = height;                                                      // 高さ
        desc.MipLevels          = 1;                                                           // ミップレベル段数
        desc.ArraySize          = 1;                                                           // 配列数
        desc.Format             = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;                            // ピクセルフォーマット
        desc.SampleDesc.Count   = 1;                                                           // マルチサンプルサンプル数
        desc.SampleDesc.Quality = 0;                                                           // マルチサンプル品質
        desc.Usage              = D3D11_USAGE_DEFAULT;                                         // 用途			(ZバッファはDEFAULT固定)
        desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL /*| D3D11_BIND_SHADER_RESOURCE*/;   // バインド設定 (Zバッファに設定)
        desc.CPUAccessFlags     = 0;                                                           // CPUアクセス	(Zバッファは0固定)
        desc.MiscFlags          = 0;                                                           // その他オプション

        ID3D11Texture2D* d3dTexture2D = nullptr;
        hr                            = device::D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D);
        if(hr != S_OK)
            return false;

        depthStencil_.reset(gpu::createTexture(d3dTexture2D));
    }

    //=============================================================
    //	カリング設定
    //=============================================================
    {
        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        //desc.FillMode             = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        desc.CullMode             = D3D11_CULL_MODE::D3D11_CULL_NONE;
        desc.DepthClipEnable      = FALSE;
        desc.MultisampleEnable    = FALSE;
        desc.DepthBiasClamp       = 0;
        desc.SlopeScaledDepthBias = 0;
        hr                        = device::D3DDevice()->CreateRasterizerState(&desc, &d3dRasterizeState_);
        if(hr != S_OK) {
            return false;
        }
        device::D3DContext()->RSSetState(d3dRasterizeState_.Get());
    }

    //=============================================================
    // テクスチャーサンプラー
    //=============================================================
    {
        D3D11_SAMPLER_DESC desc{};

        desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // 拡大・縮小・ミップをバイリニア補間 (TriLinearFilter)
        desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;        // U方向 - リピートWRAP
        desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;        // V方向 - リピートWRAP
        desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;        // W方向 - リピートWRAP
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;            // 読み込んだ値と比較する演算
        desc.MinLOD         = 0;                                 // 最小ミップLOD
        desc.MaxLOD         = D3D11_FLOAT32_MAX;                 // 最大ミップLOD
        hr                  = device::D3DDevice()->CreateSamplerState(&desc, &d3dSamplerState_LinearWrap_);
        if(hr != S_OK) {
            return false;
        }

        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // U方向 - CLAMP
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // V方向 - CLAMP
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // W方向 - CLAMP
        hr            = device::D3DDevice()->CreateSamplerState(&desc, &d3dSamplerState_LinearClamp_);
        if(hr != S_OK) {
            return false;
        }
    }

    //=============================================================
    // ブレンドステート
    //=============================================================
    {
        D3D11_BLEND_DESC desc{};

        desc.AlphaToCoverageEnable  = false;   // Alpha-to-coverageを使用するかどうか
        desc.IndependentBlendEnable = false;   // 個別にブレンドモード設定をするかどうか

        auto& rt = desc.RenderTarget[0];

        rt.BlendEnable           = false;
        rt.SrcBlend              = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
        rt.BlendOp               = D3D11_BLEND_OP_ADD;
        rt.SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
        rt.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        device::D3DDevice()->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_OFF]);

        // 通常ブレンド
        rt.BlendEnable = true;
        device::D3DDevice()->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_BLEND]);

        // 加算ブレンド
        rt.SrcBlend       = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend      = D3D11_BLEND_ONE;
        rt.SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha = D3D11_BLEND_ONE;
        device::D3DDevice()->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_ADD]);
    }

    //=============================================================
    //	フィルタ初期化
    //=============================================================
    filterManager_.reset(new FilterManager());
    if(!filterManager_->initialize())
        return false;

    // IBL
    if(!ibl_.initialize())
        return false;

    width_  = width;
    height_ = height;

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SystemRender::cleanup()
{
    ibl_.cleanup();

    if(filterManager_) {
        filterManager_->cleanup();
    }
}

//---------------------------------------------------------------------------
//! 描画開始
//---------------------------------------------------------------------------
void SystemRender::begin()
{
    //isHDR_ = true;   // これ以降はHDRモード

    device::D3DContext()->PSSetSamplers(0, 1, d3dSamplerState_LinearWrap_.GetAddressOf());    // テクスチャサンプラー
    device::D3DContext()->PSSetSamplers(2, 1, d3dSamplerState_LinearClamp_.GetAddressOf());   // テクスチャサンプラー

    // IBL
    ibl_.begin();
}

//---------------------------------------------------------------------------
//! 描画終了
//---------------------------------------------------------------------------
void SystemRender::end()
{
    filterManager_->begin();
}

//---------------------------------------------------------------------------
//! シーン描画開始
//---------------------------------------------------------------------------
void SystemRender::beginScene()
{
    auto renderBuffer = getRenderBuffer();

    //=============================================================
    // 描画ターゲットとデプスバッファをクリア
    //=============================================================

    // カラーバッファクリア
    clearTarget(renderBuffer, { 0.1f, 0.1f, 0.1f, 1.0f });

    // デプスバッファクリア
    clearTarget(depthStencil_, { 0.1f, 0.1f, 0.1f, 1.0f });

    //=============================================================
    // 描画ターゲットの設定
    //=============================================================
    gpu::setRenderTarget(renderBuffer, depthStencil_);
}

//---------------------------------------------------------------------------
//! シーン描画終了
//---------------------------------------------------------------------------
void SystemRender::endScene()
{
    ibl_.end();
}

//---------------------------------------------------------------------------
//!	画面更新
//---------------------------------------------------------------------------
void SystemRender::swap(s32 interval)
{
    // 描画バッファをスワップチェインに反映
    auto renderBuffer = GmRender()->getRenderBuffer(0, filterManager_->isHDR());

    gpu::setRenderTarget(GmRender()->getBackBuffer());
    dxTexture(renderBuffer);

    // コピーシェーダー設定
    gpu::setShader("vsPrim2D", "psTexture3D");

    put();

    // バックバッファとフロントバッファを入れ替え
    DirectX11::getInstance().getD3DSwapChain()->Present(interval, 0);
    //DirectX11::getInstance().getD3DSwapChain()->Present(0, 0);
}

//---------------------------------------------------------------------------
//! 全画面描画
//---------------------------------------------------------------------------
void SystemRender::put()
{
    // 全画面フィル
    // 巨大な三角形で描画することで境界線がなくなり、最大効率でレンダリングできる
    dxBegin(PT_TRIANGLES);
    {
        dxColor4ub(255, 255, 255, 255);

        dxTexCoord2f(0.0f, 1.0f);
        dxVertex3f(-1.0f, -1.0f, 0.0f);

        dxTexCoord2f(0.0f, -1.0f);
        dxVertex3f(-1.0f, +3.0f, 0.0f);

        dxTexCoord2f(2.0f, 1.0f);
        dxVertex3f(+3.0f, -1.0f, 0.0f);
    }
    dxEnd();
}

//---------------------------------------------------------------------------
//! バッファ切り替え
//---------------------------------------------------------------------------
void SystemRender::swapRenderBuffer()
{
    currentBuffer_ ^= 1;
}

//---------------------------------------------------------------------------
//! カリング設定
//---------------------------------------------------------------------------
void SystemRender::settingCullingState(D3D11_FILL_MODE fileMode, D3D11_CULL_MODE cullingState)
{
    //=============================================================
    //	カリング設定
    //=============================================================
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode             = fileMode;
    rasterizerDesc.CullMode             = cullingState;
    rasterizerDesc.DepthClipEnable      = FALSE;
    rasterizerDesc.MultisampleEnable    = FALSE;
    rasterizerDesc.DepthBiasClamp       = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    device::D3DDevice()->CreateRasterizerState(&rasterizerDesc, &d3dRasterizeState_);
    device::D3DContext()->RSSetState(d3dRasterizeState_.Get());
}
