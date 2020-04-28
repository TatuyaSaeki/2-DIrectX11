//===========================================================================
//! @file	model.h
//!	@brief
//===========================================================================
#pragma once

//===========================================================================
//! @class Model
//===========================================================================
class Model
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Model() = default;

    //! @brief �f�X�g���N�^
    virtual ~Model() = default;

    //@}
    //---------------------------------------------------------------------------
    //!	@name	�^�X�N
    //---------------------------------------------------------------------------
    //@{

	//---------------------------------------------------------------------------
    //! @brief �ǂݍ���
	//! @param [in] fileName      �t�@�C����
	//! @param [in] scale         ���f���̃X�P�[��
	//! @param [in] primitiveType �v���~�e�B�u�^�C�v
	//! @return true  ����I��
	//! @return false �G���[�I��
	//---------------------------------------------------------------------------
    virtual bool load(const std::string& fileName, [[maybe_unused]] const Vector3& scale = Vector3::ONE, PRIMITIVE_TYPE primitiveType = PRIMITIVE_TYPE::PT_TRIANGLES);

	//---------------------------------------------------------------------------
    //! @brief �`��
	//! @param [in] matWorld ���[���h�s��
	//---------------------------------------------------------------------------
    virtual void render(const Matrix& matWorld);

    //! @brief ���
    virtual void cleanup();

    //@}
    //---------------------------------------------------------------------------
    //!	@name	�ݒ�
    //---------------------------------------------------------------------------
    //@{

	//---------------------------------------------------------------------------
    //! @brief �e�N�X�`���ݒ�
	//! @param [in] slot    GPU�X���b�g�ԍ�
	//! @param [in] texture �e�N�X�`��
	//---------------------------------------------------------------------------
    bool setTexture(s32 slot, std::shared_ptr<gpu::Texture> texture);

    //@}

protected:
    //! �g�p����e�N�X�`��(�O������̎w��p)
    struct UseTexture
    {
        UseTexture() = default;
        UseTexture(std::shared_ptr<gpu::Texture> texture, s32 gpuSlot)
        : texture_(texture)
        , gpuSlot_(gpuSlot)
        {
        }

        std::shared_ptr<gpu::Texture> texture_;
        s32                           gpuSlot_;
    };

    std::weak_ptr<AssetModel> assetModel_;      //!< �O������󂯎��
    std::vector<UseTexture>   useTexture_;      //!< �g�p����V�F�[�_�[
    PRIMITIVE_TYPE            primitiveType_;   //!< �`�掞�̃v���~�e�B�u�^�C�v
};