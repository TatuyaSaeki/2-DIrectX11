//===========================================================================
//! @file	shader_manager.h
//!	@brief	�V�F�[�_�[�Ǘ�
//===========================================================================
#pragma once

// �܂Ƃ߂Ă邾��
struct ShaderVs
{
    std::unique_ptr<gpu::InputLayout> inputLayout_;
    std::unique_ptr<gpu::ShaderVs>    shaderVs_;
};

//===========================================================================
//! @class ShaderManager
//===========================================================================
class ShaderManager : public ManagerBase
{
public:
    //-----------------------------------------------------------------------
    //! @name	������
	//-----------------------------------------------------------------------
    //@{

    //! @brief �f�X�g���N�^
    ~ShaderManager();

    //@}
    //---------------------------------------------------------------------------
    //! @name	�擾
    //---------------------------------------------------------------------------

    //! @brief ���̓��C�A�E�g�擾
    static gpu::InputLayout* const getInputLayout(const std::string& funcName);

    //! @brief ���_�V�F�[�_�[���擾
    static gpu::ShaderVs* const getShaderVs(const std::string& fileName);

    //! @brief �s�N�Z���V�F�[�_�[���擾
    static gpu::ShaderPs* const getShaderPs(const std::string& fileName);

    //! @brief �W�I���g���V�F�[�_�[���擾
    static gpu::ShaderGs* const getShaderGs(const std::string& fileName);

    //! @brief �n���V�F�[�_�[���擾
    static gpu::ShaderHs* const getShaderHs(const std::string& fileName);

    //! @brief �h���C���V�F�[�_�[���擾
    static gpu::ShaderDs* const getShaderDs(const std::string& fileName);

	//@}

private:

	//! @brief �R���X�g���N�^
    ShaderManager() = default;

    //! @brief ���̓��C�A�E�g�쐬
    static gpu::InputLayout* createInputLayout(const std::string& funcName);

private:
    //std::unordered_map<std::string, ShaderVs>                       shaderVs_;
    //std::unordered_map<std::string, std::unique_ptr<gpu::ShaderPs>> shaderPs_;
};
