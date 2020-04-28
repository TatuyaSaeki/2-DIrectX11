//===========================================================================
//! @file	object_water.h
//! @brife	���\���p�N���X
//===========================================================================
#pragma once

//===========================================================================
//! @class Water
//===========================================================================
class Water : public ObjectBaseModel
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Water();

    //! @brief �f�X�g���N�^
    virtual ~Water() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ������
    //! @param [in] buffer	       �o�b�t�@�p�e�N�X�`��
    //! @param [in] modelPaht      ���f���p�X
    //! @param [in] scale          ���f���̃X�P�[��
    //! @param [in] primitiveType  �`�掞�̃v���~�e�B�u�^�C�v
    //! @return true		       ����
    //! @return false		       ���s
    //-----------------------------------------------------------------------
    bool initialize(std::shared_ptr<gpu::Texture> buffer, const std::string& modelPath, f32 scale = 1.0f, PRIMITIVE_TYPE primitiveType = PT_TRIANGLES);

    //! @brief �X�V
    void update() override;

    //-----------------------------------------------------------------------
    //! @brief �`��
    //! @param [in] �`�悵�������[�h
    //-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief ImGui�`��
    void showImGuiWindow() override;

    //@}

private:
    gpu::ConstantBuffer<cbSeaWave> cbWave_;         //!< �萔�o�b�t�@
    u32                            gpuSlot_ = 10;   //!< GPU�X���b�g�ԍ�

    struct Wave
    {
        f32 speed_        = 0.01f;   //!< �i�ރX�s�[�h
        f32 moveDistance_ = 0.0f;    //!< �i�񂾋���
        f32 fineness_     = 0.5f;    //!< �ׂ���
        f32 sharp_        = 0.5f;    //!< ���x
        f32 height_       = 0.5f;    //!< ����
    };
    Wave wave_;   //!< �g�p
};