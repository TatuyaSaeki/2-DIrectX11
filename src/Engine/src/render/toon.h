//===========================================================================
//! @file	toon.h
//!	@brief	�A�j�����`��
//===========================================================================
#pragma once

//===========================================================================
//! @clsss Toon
//===========================================================================
class Toon
{
public:
	//-----------------------------------------------------------------------
	//!	@name	������
	//-----------------------------------------------------------------------
	//@{

	//! @brief �R���X�g���N�^
	Toon() = default;

	//! @brief �f�X�g���N�^
	~Toon() = default;

	//@}
	//-----------------------------------------------------------------------
	//!	@name	�^�X�N
	//-----------------------------------------------------------------------
	//@{

	//! @brief ������
	bool initialize(s32 resolution = 2048);

	//! @brief ���
	void cleanup();

	//! @brief �G�b�W�`��J�n
	void beginEdge();

	//! @brief �g�D�[���`��J�n
	void begin();

	//! @brief �g�D�[���`��I��
	void end();

	//@}

private:
	s32                           resolution_ = 1280;
	std::unique_ptr<gpu::Texture> toonTexture_;   //!< �g�D�[���p�e�N�X�`��
	gpu::ConstantBuffer<cbToon> cbToon_;	//!< �萔�o�b�t�@
	u32 gpuSlotCBToon= 9;
	u32 gpuSlotToonTexture_ = 11;	//!< gpu�e�N�X�`���X���b�g�ԍ�
};