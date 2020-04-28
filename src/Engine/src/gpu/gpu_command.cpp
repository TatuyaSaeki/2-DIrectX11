//===========================================================================
//!	@file	gpu_command.cpp
//!	@brief	GPUコマンド
//===========================================================================

namespace gpu {

namespace {
const gpu::InputLayout* inputLayout_;
const gpu::ShaderVs*    shaderVs_;
}   // namespace

//---------------------------------------------------------------------------
//! 入力レイアウトの設定
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout)
{
    inputLayout_ = inputLayout;
}

// ここもファイル名いる
void setInputLayout(const std::string& vsFileName)
{
    inputLayout_ = ShaderManager::getInputLayout(vsFileName);
}

//---------------------------------------------------------------------------
// 頂点バッファの設定
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset)
{
    ID3D11Buffer* buffers[1]{ buffer->getD3DBuffer() };
    u32           strides[1]{ stride };
    u32           offsets[1]{ offset };
    device::D3DContext()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
}

//---------------------------------------------------------------------------
//! インデックスバッファの設定
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset)
{
    device::D3DContext()->IASetIndexBuffer(buffer->getD3DBuffer(), DXGI_FORMAT_R32_UINT, offset);
}

//---------------------------------------------------------------------------
//! シェーダーの設定
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps, ptr<const gpu::ShaderGs> gs, ptr<const gpu::ShaderHs> hs, ptr<const gpu::ShaderDs> ds)
{
    auto d3dContext = device::D3DContext();

    d3dContext->VSSetShader(vs->get(), nullptr, 0);

    // PSは警告対策
    if(ps)
        d3dContext->PSSetShader(ps->get(), nullptr, 0);
    else
        d3dContext->PSSetShader(nullptr, nullptr, 0);

    if(gs)
        d3dContext->GSSetShader(gs->get(), nullptr, 0);
    if(hs)
        d3dContext->HSSetShader(hs->get(), nullptr, 0);
    if(ds)
        d3dContext->DSSetShader(ds->get(), nullptr, 0);

    shaderVs_ = vs;
}

//---------------------------------------------------------------------------
//! シェーダーの設定
//---------------------------------------------------------------------------
void setShader(const std::string& fileNameVs, const std::string& fileNamePs, const std::string* fileNameGs, const std::string* fileNameHs, const std::string* fileNameDs)
{
    inputLayout_            = ShaderManager::getInputLayout(fileNameVs);
    gpu::ShaderVs* shaderVs = ShaderManager::getShaderVs(fileNameVs);
    gpu::ShaderPs* shaderPs = nullptr;
    gpu::ShaderGs* shaderGs = nullptr;
    gpu::ShaderHs* shaderHs = nullptr;
    gpu::ShaderDs* shaderDs = nullptr;

    if(fileNamePs == "nullptr")
        shaderPs = nullptr;
    else
        shaderPs = ShaderManager::getShaderPs(fileNamePs);

    if(fileNameGs)
        shaderGs = ShaderManager::getShaderGs(*fileNameGs);
    if(fileNameHs)
        shaderHs = ShaderManager::getShaderHs(*fileNameHs);
    if(fileNameDs)
        shaderDs = ShaderManager::getShaderDs(*fileNameDs);

    setShader(shaderVs, shaderPs, shaderGs, shaderHs, shaderDs);
}

//---------------------------------------------------------------------------
//! シェーダーの設定
//---------------------------------------------------------------------------
void setShader(const std::string& fileNameVs, const std::string* fileNamePs)
{
    inputLayout_            = ShaderManager::getInputLayout(fileNameVs);
    gpu::ShaderVs* shaderVs = ShaderManager::getShaderVs(fileNameVs);
    gpu::ShaderPs* shaderPs = nullptr;

    if(shaderPs)
        shaderPs = ShaderManager::getShaderPs(*fileNamePs);

    setShader(shaderVs, shaderPs);
}

//---------------------------------------------------------------------------
//! シェーダーの設定
//---------------------------------------------------------------------------
void setShader(const UseShader& shaderName)
{
    setShader(shaderName.vsName_, shaderName.psName_, shaderName.gsName_, shaderName.hsName_, shaderName.dsName_);
}

//---------------------------------------------------------------------------
//! 定数バッファの設定
//---------------------------------------------------------------------------
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer)
{
    auto d3dContext = device::D3DContext();

    ID3D11Buffer* d3dBuffer = buffer ? buffer->getD3DBuffer() : nullptr;
    d3dContext->VSSetConstantBuffers(slot, 1, &d3dBuffer);
    d3dContext->GSSetConstantBuffers(slot, 1, &d3dBuffer);
    d3dContext->DSSetConstantBuffers(slot, 1, &d3dBuffer);
    d3dContext->PSSetConstantBuffers(slot, 1, &d3dBuffer);
}

//---------------------------------------------------------------------------
//! テクスチャの設定
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture)
{
    auto srv = texture ? texture->getD3DSrv() : nullptr;

    device::D3DContext()->DSSetShaderResources(slot, 1, &srv);   // テクスチャ
    device::D3DContext()->PSSetShaderResources(slot, 1, &srv);   // テクスチャ
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset)
{
    auto d3dContext     = device::D3DContext();
    auto inputLayout    = const_cast<gpu::InputLayout*>(inputLayout_);
    auto d3dInputLayout = inputLayout->getInputLayout();

    d3dContext->IASetInputLayout(d3dInputLayout);
    d3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    d3dContext->Draw(vertexCount, vertexOffset);
}

//---------------------------------------------------------------------------
//! インデックス描画
//---------------------------------------------------------------------------
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset, u32 vertexOffset)
{
    auto d3dContext     = device::D3DContext();
    auto inputLayout    = const_cast<gpu::InputLayout*>(inputLayout_);
    auto d3dInputLayout = inputLayout->getInputLayout();

    d3dContext->IASetInputLayout(d3dInputLayout);
    d3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    d3dContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

//---------------------------------------------------------------------------
//! 描画ターゲットの設定
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil)
{
    gpu::setRenderTarget(1, &renderTarget, depthStencil);
}

//---------------------------------------------------------------------------
//! 描画ターゲットの設定(MRT用)
//---------------------------------------------------------------------------
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil)
{
    ID3D11RenderTargetView* d3dRtvs[8];

    for(u32 i = 0; i < renderTargetCount; ++i) {
        d3dRtvs[i] = (renderTargets[i]) ? renderTargets[i]->getD3DRtv() : nullptr;
    }

    auto d3dDsv = (depthStencil) ? depthStencil->getD3DDsv() : nullptr;
    device::D3DContext()->OMSetRenderTargets(renderTargetCount, d3dRtvs, d3dDsv);

    //---- ビューポート設定（描画ターゲットの変更時はビューポートの更新も行う）
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.0f;   // 左上X
    viewport.TopLeftY = 0.0f;   // 左上Y
    viewport.Width    = 0.0f;   // 幅
    viewport.Height   = 0.0f;   // 高さ
    viewport.MinDepth = 0.0f;   // Z最小値
    viewport.MaxDepth = 1.0f;   // Z最大値

    auto texture = (renderTargets[0]) ? renderTargets[0] : depthStencil;
    if(texture) {
        viewport.Width  = static_cast<f32>(texture->getWidth());
        viewport.Height = static_cast<f32>(texture->getHeight());
    }

    device::D3DContext()->RSSetViewports(1, &viewport);
}

//---------------------------------------------------------------------------
//! 描画ターゲットのクリア
//---------------------------------------------------------------------------
void clearTarget(ptr<gpu::Texture> target, const Vector4& color, f32 depth, u8 stencil)
{
    // 描画ターゲットのクリア
    if(target->getD3DRtv()) {
        auto clearColor = reinterpret_cast<const f32*>(&color);
        device::D3DContext()->ClearRenderTargetView(target->getD3DRtv(), clearColor);
    }

    // デプスバッファのクリア
    if(target->getD3DDsv()) {
        device::D3DContext()->ClearDepthStencilView(target->getD3DDsv(),
                                                    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                                    depth,
                                                    stencil);
    }
}

}   // namespace gpu
