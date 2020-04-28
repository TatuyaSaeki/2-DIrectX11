//===========================================================================
//!	@file	shadow_map.cpp
//!	@brief	�V���h�E�}�b�v�Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool ShadowMap::initialize(s32 resolution)
{
    resolution_ = resolution;

    //=============================================================
    // �f�v�X�e�N�X�`���̍쐬
    //=============================================================
    depthTexture_.reset(gpu::createRenderTarget(resolution, resolution, DXGI_FORMAT_D32_FLOAT));
    if(!depthTexture_)
        return false;

    //gpu::setTexture(10, depthTexture_);

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void ShadowMap::finalize()
{
    depthTexture_.reset();
}

//---------------------------------------------------------------------------
//! �V���h�E�p�X�̊J�n
//---------------------------------------------------------------------------
void ShadowMap::begin(const Light& light)
{
    gpu::setTexture(10, nullptr);

    // �V���h�E�f�v�X�o�b�t�@�̃N���A
    device::D3DContext()->ClearDepthStencilView(depthTexture_->getD3DDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // �f�v�X�o�b�t�@�̐ݒ�(�{��Z���[�h)
    // �`�����f�v�X�e�N�X�`���ɐݒ�
    gpu::setRenderTarget(nullptr, depthTexture_);

    const f32 range         = 30.0f;    // �B�e�̕�(�}�͈̔�)
    const f32 limitDistance = 100.0f;   // �e�̉��N���b�v��

    // �����J�����̐ݒ�
    const Matrix matLightView = light.getMatView();
    const Matrix matLightProj = Matrix::scale(Vector3(0.5f / range, 0.5f / range, 1.0f / limitDistance * -1.0f));

    // �e�p�萔�o�b�t�@�X�V
    auto CBShadow = cbShadow_.begin();
    {
        CBShadow->matLightView_ = matLightView;
        CBShadow->matLightProj_ = matLightProj;
    }
    cbShadow_.end();

    gpu::setConstantBuffer(8, cbShadow_);
}

//---------------------------------------------------------------------------
//! �V���h�E�p�X�̏I��
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
//! �f�v�X�e�N�X�`���擾
//---------------------------------------------------------------------------
gpu::Texture* ShadowMap::getDepthTexture()
{
    return depthTexture_.get();
}