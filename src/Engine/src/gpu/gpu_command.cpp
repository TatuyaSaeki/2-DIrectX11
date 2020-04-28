//===========================================================================
//!	@file	gpu_command.cpp
//!	@brief	GPU�R�}���h
//===========================================================================

namespace gpu {

namespace {
const gpu::InputLayout* inputLayout_;
const gpu::ShaderVs*    shaderVs_;
}   // namespace

//---------------------------------------------------------------------------
//! ���̓��C�A�E�g�̐ݒ�
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout)
{
    inputLayout_ = inputLayout;
}

// �������t�@�C��������
void setInputLayout(const std::string& vsFileName)
{
    inputLayout_ = ShaderManager::getInputLayout(vsFileName);
}

//---------------------------------------------------------------------------
// ���_�o�b�t�@�̐ݒ�
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset)
{
    ID3D11Buffer* buffers[1]{ buffer->getD3DBuffer() };
    u32           strides[1]{ stride };
    u32           offsets[1]{ offset };
    device::D3DContext()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
}

//---------------------------------------------------------------------------
//! �C���f�b�N�X�o�b�t�@�̐ݒ�
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset)
{
    device::D3DContext()->IASetIndexBuffer(buffer->getD3DBuffer(), DXGI_FORMAT_R32_UINT, offset);
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̐ݒ�
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps, ptr<const gpu::ShaderGs> gs, ptr<const gpu::ShaderHs> hs, ptr<const gpu::ShaderDs> ds)
{
    auto d3dContext = device::D3DContext();

    d3dContext->VSSetShader(vs->get(), nullptr, 0);

    // PS�͌x���΍�
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
//! �V�F�[�_�[�̐ݒ�
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
//! �V�F�[�_�[�̐ݒ�
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
//! �V�F�[�_�[�̐ݒ�
//---------------------------------------------------------------------------
void setShader(const UseShader& shaderName)
{
    setShader(shaderName.vsName_, shaderName.psName_, shaderName.gsName_, shaderName.hsName_, shaderName.dsName_);
}

//---------------------------------------------------------------------------
//! �萔�o�b�t�@�̐ݒ�
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
//! �e�N�X�`���̐ݒ�
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture)
{
    auto srv = texture ? texture->getD3DSrv() : nullptr;

    device::D3DContext()->DSSetShaderResources(slot, 1, &srv);   // �e�N�X�`��
    device::D3DContext()->PSSetShaderResources(slot, 1, &srv);   // �e�N�X�`��
}

//---------------------------------------------------------------------------
//! �`��
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
//! �C���f�b�N�X�`��
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
//! �`��^�[�Q�b�g�̐ݒ�
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil)
{
    gpu::setRenderTarget(1, &renderTarget, depthStencil);
}

//---------------------------------------------------------------------------
//! �`��^�[�Q�b�g�̐ݒ�(MRT�p)
//---------------------------------------------------------------------------
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil)
{
    ID3D11RenderTargetView* d3dRtvs[8];

    for(u32 i = 0; i < renderTargetCount; ++i) {
        d3dRtvs[i] = (renderTargets[i]) ? renderTargets[i]->getD3DRtv() : nullptr;
    }

    auto d3dDsv = (depthStencil) ? depthStencil->getD3DDsv() : nullptr;
    device::D3DContext()->OMSetRenderTargets(renderTargetCount, d3dRtvs, d3dDsv);

    //---- �r���[�|�[�g�ݒ�i�`��^�[�Q�b�g�̕ύX���̓r���[�|�[�g�̍X�V���s���j
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.0f;   // ����X
    viewport.TopLeftY = 0.0f;   // ����Y
    viewport.Width    = 0.0f;   // ��
    viewport.Height   = 0.0f;   // ����
    viewport.MinDepth = 0.0f;   // Z�ŏ��l
    viewport.MaxDepth = 1.0f;   // Z�ő�l

    auto texture = (renderTargets[0]) ? renderTargets[0] : depthStencil;
    if(texture) {
        viewport.Width  = static_cast<f32>(texture->getWidth());
        viewport.Height = static_cast<f32>(texture->getHeight());
    }

    device::D3DContext()->RSSetViewports(1, &viewport);
}

//---------------------------------------------------------------------------
//! �`��^�[�Q�b�g�̃N���A
//---------------------------------------------------------------------------
void clearTarget(ptr<gpu::Texture> target, const Vector4& color, f32 depth, u8 stencil)
{
    // �`��^�[�Q�b�g�̃N���A
    if(target->getD3DRtv()) {
        auto clearColor = reinterpret_cast<const f32*>(&color);
        device::D3DContext()->ClearRenderTargetView(target->getD3DRtv(), clearColor);
    }

    // �f�v�X�o�b�t�@�̃N���A
    if(target->getD3DDsv()) {
        device::D3DContext()->ClearDepthStencilView(target->getD3DDsv(),
                                                    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                                    depth,
                                                    stencil);
    }
}

}   // namespace gpu
