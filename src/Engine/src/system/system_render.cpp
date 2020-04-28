//===========================================================================
//! @file	system_render.cpp
//!	@brief	DirectX11�`��V�X�e���Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SystemRender::setup(u32 width, u32 height)
{
    HRESULT hr = S_OK;

    //=============================================================
    //	�`��^�[�Q�b�g�̍쐬
    //=============================================================
    {
        // �X���b�v�`�F�C���̒��ɂ���o�b�N�o�b�t�@�������[���擾
        ID3D11Texture2D* d3dRenderTarget = nullptr;
        hr                               = DirectX11::getInstance().getD3DSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dRenderTarget);
        if(hr != S_OK)
            return false;

        // �o�b�N�o�b�t�@�쐬
        backBuffer_.reset(gpu::createTexture(d3dRenderTarget));
    }

    // HDR�o�b�t�@�̍쐬
    for(u32 i = 0; i < 2; ++i) {
        auto hdr = gpu::createRenderTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT);
        hdrBuffer_[i].reset(hdr);
    }
    // SDR�o�b�t�@�̍쐬
    for(u32 i = 0; i < 2; ++i) {
        auto sdr = gpu::createRenderTarget(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
        sdrBuffer_[i].reset(sdr);
    }

    // HDR���[�N�o�b�t�@�̍쐬
    for(auto& t : hdrWorkBuffer_) {
        for(u32 mip = 0; mip < std::size(t); ++mip) {
            u32 w = std::max(1u, width >> mip);    // 1/2 �����Ă���
            u32 h = std::max(1u, height >> mip);   // 1/2 �����Ă���

            auto hdr = gpu::createRenderTarget(w, h, DXGI_FORMAT_R11G11B10_FLOAT);
            t[mip].reset(hdr);
        }
    }

    //=============================================================
    // �y�o�b�t�@�̍쐬
    //=============================================================
    {
        //---- GPU�������̊m��(Texture2D)
        D3D11_TEXTURE2D_DESC desc{};

        desc.Width              = width;                                                       // ��
        desc.Height             = height;                                                      // ����
        desc.MipLevels          = 1;                                                           // �~�b�v���x���i��
        desc.ArraySize          = 1;                                                           // �z��
        desc.Format             = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;                            // �s�N�Z���t�H�[�}�b�g
        desc.SampleDesc.Count   = 1;                                                           // �}���`�T���v���T���v����
        desc.SampleDesc.Quality = 0;                                                           // �}���`�T���v���i��
        desc.Usage              = D3D11_USAGE_DEFAULT;                                         // �p�r			(Z�o�b�t�@��DEFAULT�Œ�)
        desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL /*| D3D11_BIND_SHADER_RESOURCE*/;   // �o�C���h�ݒ� (Z�o�b�t�@�ɐݒ�)
        desc.CPUAccessFlags     = 0;                                                           // CPU�A�N�Z�X	(Z�o�b�t�@��0�Œ�)
        desc.MiscFlags          = 0;                                                           // ���̑��I�v�V����

        ID3D11Texture2D* d3dTexture2D = nullptr;
        hr                            = device::D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D);
        if(hr != S_OK)
            return false;

        depthStencil_.reset(gpu::createTexture(d3dTexture2D));
    }

    //=============================================================
    //	�J�����O�ݒ�
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
    // �e�N�X�`���[�T���v���[
    //=============================================================
    {
        D3D11_SAMPLER_DESC desc{};

        desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // �g��E�k���E�~�b�v���o�C���j�A��� (TriLinearFilter)
        desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;        // U���� - ���s�[�gWRAP
        desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;        // V���� - ���s�[�gWRAP
        desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;        // W���� - ���s�[�gWRAP
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;            // �ǂݍ��񂾒l�Ɣ�r���鉉�Z
        desc.MinLOD         = 0;                                 // �ŏ��~�b�vLOD
        desc.MaxLOD         = D3D11_FLOAT32_MAX;                 // �ő�~�b�vLOD
        hr                  = device::D3DDevice()->CreateSamplerState(&desc, &d3dSamplerState_LinearWrap_);
        if(hr != S_OK) {
            return false;
        }

        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // U���� - CLAMP
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // V���� - CLAMP
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // W���� - CLAMP
        hr            = device::D3DDevice()->CreateSamplerState(&desc, &d3dSamplerState_LinearClamp_);
        if(hr != S_OK) {
            return false;
        }
    }

    //=============================================================
    // �u�����h�X�e�[�g
    //=============================================================
    {
        D3D11_BLEND_DESC desc{};

        desc.AlphaToCoverageEnable  = false;   // Alpha-to-coverage���g�p���邩�ǂ���
        desc.IndependentBlendEnable = false;   // �ʂɃu�����h���[�h�ݒ�����邩�ǂ���

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

        // �ʏ�u�����h
        rt.BlendEnable = true;
        device::D3DDevice()->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_BLEND]);

        // ���Z�u�����h
        rt.SrcBlend       = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend      = D3D11_BLEND_ONE;
        rt.SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha = D3D11_BLEND_ONE;
        device::D3DDevice()->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_ADD]);
    }

    //=============================================================
    //	�t�B���^������
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
//! ���
//---------------------------------------------------------------------------
void SystemRender::cleanup()
{
    ibl_.cleanup();

    if(filterManager_) {
        filterManager_->cleanup();
    }
}

//---------------------------------------------------------------------------
//! �`��J�n
//---------------------------------------------------------------------------
void SystemRender::begin()
{
    //isHDR_ = true;   // ����ȍ~��HDR���[�h

    device::D3DContext()->PSSetSamplers(0, 1, d3dSamplerState_LinearWrap_.GetAddressOf());    // �e�N�X�`���T���v���[
    device::D3DContext()->PSSetSamplers(2, 1, d3dSamplerState_LinearClamp_.GetAddressOf());   // �e�N�X�`���T���v���[

    // IBL
    ibl_.begin();
}

//---------------------------------------------------------------------------
//! �`��I��
//---------------------------------------------------------------------------
void SystemRender::end()
{
    filterManager_->begin();
}

//---------------------------------------------------------------------------
//! �V�[���`��J�n
//---------------------------------------------------------------------------
void SystemRender::beginScene()
{
    auto renderBuffer = getRenderBuffer();

    //=============================================================
    // �`��^�[�Q�b�g�ƃf�v�X�o�b�t�@���N���A
    //=============================================================

    // �J���[�o�b�t�@�N���A
    clearTarget(renderBuffer, { 0.1f, 0.1f, 0.1f, 1.0f });

    // �f�v�X�o�b�t�@�N���A
    clearTarget(depthStencil_, { 0.1f, 0.1f, 0.1f, 1.0f });

    //=============================================================
    // �`��^�[�Q�b�g�̐ݒ�
    //=============================================================
    gpu::setRenderTarget(renderBuffer, depthStencil_);
}

//---------------------------------------------------------------------------
//! �V�[���`��I��
//---------------------------------------------------------------------------
void SystemRender::endScene()
{
    ibl_.end();
}

//---------------------------------------------------------------------------
//!	��ʍX�V
//---------------------------------------------------------------------------
void SystemRender::swap(s32 interval)
{
    // �`��o�b�t�@���X���b�v�`�F�C���ɔ��f
    auto renderBuffer = GmRender()->getRenderBuffer(0, filterManager_->isHDR());

    gpu::setRenderTarget(GmRender()->getBackBuffer());
    dxTexture(renderBuffer);

    // �R�s�[�V�F�[�_�[�ݒ�
    gpu::setShader("vsPrim2D", "psTexture3D");

    put();

    // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
    DirectX11::getInstance().getD3DSwapChain()->Present(interval, 0);
    //DirectX11::getInstance().getD3DSwapChain()->Present(0, 0);
}

//---------------------------------------------------------------------------
//! �S��ʕ`��
//---------------------------------------------------------------------------
void SystemRender::put()
{
    // �S��ʃt�B��
    // ����ȎO�p�`�ŕ`�悷�邱�Ƃŋ��E�����Ȃ��Ȃ�A�ő�����Ń����_�����O�ł���
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
//! �o�b�t�@�؂�ւ�
//---------------------------------------------------------------------------
void SystemRender::swapRenderBuffer()
{
    currentBuffer_ ^= 1;
}

//---------------------------------------------------------------------------
//! �J�����O�ݒ�
//---------------------------------------------------------------------------
void SystemRender::settingCullingState(D3D11_FILL_MODE fileMode, D3D11_CULL_MODE cullingState)
{
    //=============================================================
    //	�J�����O�ݒ�
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
