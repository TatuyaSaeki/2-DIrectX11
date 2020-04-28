//===========================================================================
//! @file	filter_tone_mapping.cpp
//! @brife	�g�[���}�b�s���O�t�B���^
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool ToneMapping::initialize()
{
	filterType_ = FilterType::ToneMapping;

    return true;
}

//---------------------------------------------------------------------------
//! �J�n
//---------------------------------------------------------------------------
void ToneMapping::begin()
{
	auto hdrBuffer = GmRender()->getRenderBuffer();

	// SDR�o�b�N�o�b�t�@��ݒ�
	gpu::setRenderTarget(GmRender()->getRenderBuffer(0, false));

	dxTexture(hdrBuffer);

	// �g�[���}�b�s���O�t�B���^�[�V�F�[�_�[�ݒ�
	gpu::setShader("vsPrim2D", "psToneMap");

	GmRender()->put();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void ToneMapping::finalize()
{
}