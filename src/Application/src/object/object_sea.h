//===========================================================================
//! @file	object_sea.h
//! @brife	�C�I�u�W�F�N�g
//===========================================================================
#pragma once

//===========================================================================
//! @class Sea
//===========================================================================
class Sea : public ObjectBaseModel
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
	Sea();

    //! @brief �f�X�g���N�^
    virtual ~Sea() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //! @brief �X�V
    void update() override;

	//-----------------------------------------------------------------------
	//! @brief �`��
	//! @param [in] �`�悵�������[�h
	//-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief ImGui�`��
    void showImGuiWindow() override;

    //! @brief �g�̐��l������
    void initializeWave();

    //@}

private:
    //std::unique_ptr<ObjectBaseModel> floor_;          //!< ���p���f��
    gpu::ConstantBuffer<cbSeaWave>   cbWave_;         //!< �萔�o�b�t�@
    u32                              gpuSlot_ = 10;   //!< GPU�X���b�g�ԍ�

    struct Wave
    {
        f32 speed_        = 0.01f;   //!< �i�ރX�s�[�h
        f32 moveDistance_ = 0.0f;    //!< �i�񂾋���
        f32 fineness_     = 0.5f;    //!< �ׂ���
        f32 sharp_        = 0.5f;    //!< ���x
        f32 height_       = 0.5f;    //!< ����
    };
    Wave wave_;   //!< �g�p

    //std::shared_ptr<gpu::Texture> buffer_;
};