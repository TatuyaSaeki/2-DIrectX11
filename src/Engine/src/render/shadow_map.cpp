//===========================================================================
//!	@file	shadow_map.cpp
//!	@brief	シャドウマップ管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool ShadowMap::initialize(s32 resolution)
{
    resolution_ = resolution;

    //=============================================================
    // デプステクスチャの作成
    //=============================================================
    depthTexture_.reset(gpu::createRenderTarget(resolution, resolution, DXGI_FORMAT_D32_FLOAT));
    if(!depthTexture_)
        return false;

    //gpu::setTexture(10, depthTexture_);

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void ShadowMap::finalize()
{
    depthTexture_.reset();
}

//---------------------------------------------------------------------------
//! シャドウパスの開始
//---------------------------------------------------------------------------
void ShadowMap::begin(const Light& light)
{
    gpu::setTexture(10, nullptr);

    // シャドウデプスバッファのクリア
    device::D3DContext()->ClearDepthStencilView(depthTexture_->getD3DDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // デプスバッファの設定(倍速Zモード)
    // 描画先をデプステクスチャに設定
    gpu::setRenderTarget(nullptr, depthTexture_);

    const f32 range         = 30.0f;    // 撮影の幅(±の範囲)
    const f32 limitDistance = 100.0f;   // 影の遠クリップ面

    // 光源カメラの設定
    const Matrix matLightView = light.getMatView();
    const Matrix matLightProj = Matrix::scale(Vector3(0.5f / range, 0.5f / range, 1.0f / limitDistance * -1.0f));

    // 影用定数バッファ更新
    auto CBShadow = cbShadow_.begin();
    {
        CBShadow->matLightView_ = matLightView;
        CBShadow->matLightProj_ = matLightProj;
    }
    cbShadow_.end();

    gpu::setConstantBuffer(8, cbShadow_);
}

//---------------------------------------------------------------------------
//! シャドウパスの終了
//---------------------------------------------------------------------------
void ShadowMap::end()
{
    auto renderBuffer = GmRender()->getRenderBuffer();
    auto depthStencil = GmRender()->getDepthStencil();
    gpu::setRenderTarget(renderBuffer, depthStencil);
    //
    //gpu::setTexture(10, nullptr);
    gpu::setTexture(TEX_DEPTH, depthTexture_);
}

//---------------------------------------------------------------------------
//! デプステクスチャ取得
//---------------------------------------------------------------------------
gpu::Texture* ShadowMap::getDepthTexture()
{
    return depthTexture_.get();
}