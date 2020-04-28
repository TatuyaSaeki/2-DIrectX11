//===========================================================================
//!	@file	toon.cpp
//!	@brief	�V���h�E�}�b�v�Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool Toon::initialize(s32 resolution)
{
    resolution_ = resolution;

	//=============================================================
    // �e�N�X�`���̍쐬
    //=============================================================
    toonTexture_.reset(gpu::createTexture("texture/toon.png"));
    if(!toonTexture_)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void Toon::cleanup()
{
    toonTexture_.reset();
}

//---------------------------------------------------------------------------
//! �G�b�W�`��J�n
//---------------------------------------------------------------------------
void Toon::beginEdge()
{
    auto cbToon   = cbToon_.begin();
    cbToon->edge_ = true;
    cbToon_.end();

    gpu::setConstantBuffer(gpuSlotCBToon, cbToon_);

    // �J�����O�ݒ�
    GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);
}

//--------------------------------------------------------------------------- -
//! �g�D�[���`��J�n
//---------------------------------------------------------------------------
void Toon::begin()
{
    auto cbToon   = cbToon_.begin();
    cbToon->edge_ = false;
    cbToon_.end();

    gpu::setConstantBuffer(gpuSlotCBToon, cbToon_);
    gpu::setTexture(gpuSlotToonTexture_, toonTexture_);

    // �J�����O�ݒ�߂�
    GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
}

//---------------------------------------------------------------------------
//! �g�D�[���`��I��
//---------------------------------------------------------------------------
void Toon::end()
{
    // tmp
    gpu::setTexture(gpuSlotToonTexture_, nullptr);
}