//===========================================================================
//!	@file	gpu_shader.h
//!	@brief	�V�F�[�_�[
//===========================================================================
#pragma once

//===========================================================================
//!	@namespace gpu
//===========================================================================
namespace gpu {

//-----------------------------------------------------------------------
//! �V�F�[�_�[�R���p�C��
//!	@param	[in]	fileName		�t�@�C����
//!	@param	[in]	functionName	�V�F�[�_�[�֐���
//!	@param	[in]	shaderModel		�o�[�W�����݂����Ȃ���?
//!	@return	[out]	���s��nullptr
//-----------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const char* fileName, const char* functionName, const char* shaderModel);

//===========================================================================
//! @class ShaderBase
//===========================================================================
class ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderBase() = default;

    //! @brief �R���X�g���N�^
    virtual ~ShaderBase() = default;

    //! @brief  �V�F�[�_�[�o�C�i���̃A�h���X���擾
    const void* getShaderBinary() const;

	//-----------------------------------------------------------------------
	//! @brief  �V�F�[�_�[�o�C�i���̃T�C�Y���擾
	//! @return �V�F�[�_�[�o�C�i���̃T�C�Y
	//-----------------------------------------------------------------------
    size_t getShaderBinarySize() const;

protected:
    com_ptr<ID3DBlob> shaderBinary_;
};

//===========================================================================
//! @class ShaderVs
//===========================================================================
class ShaderVs : public ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderVs(ID3DBlob* shaderBinary);

    //! @brief �f�X�g���N�^
    virtual ~ShaderVs() override = default;

    //! D@brief 3D�V�F�[�_�[���擾
	ID3D11VertexShader* get() const;

    //! @brief �V�F�[�_�[�̍쐬
    static gpu::ShaderVs* create(const std::string& fileName);

private:
    com_ptr<ID3D11VertexShader> shader_;
};

//===========================================================================
//! @clsss ShaderPs
//===========================================================================
class ShaderPs : public ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderPs(ID3DBlob* shaderBinary);

    //! @brief �f�X�g���N�^
    virtual ~ShaderPs() override = default;

    //! @brief D3D�V�F�[�_�[���擾
	ID3D11PixelShader* get() const;

    //! �V�F�[�_�[�̍쐬
    static gpu::ShaderPs* create(const std::string& fileName);

private:
    com_ptr<ID3D11PixelShader> shader_;
};

//===========================================================================
//! @clsss ShaderGs
//===========================================================================
class ShaderGs : public ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderGs(ID3DBlob* shaderBinary);

    //! @brief �f�X�g���N�^
    virtual ~ShaderGs() override = default;

    //! @brief D3D�V�F�[�_�[���擾
	ID3D11GeometryShader* get() const;

    //! @brief �V�F�[�_�[�̍쐬
    static gpu::ShaderGs* create(const std::string& fileName);

private:
    com_ptr<ID3D11GeometryShader> shader_;
};

//===========================================================================
//! @clsss ShaderHs
//===========================================================================
class ShaderHs : public ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderHs(ID3DBlob* shaderBinary);

    //! @brief �f�X�g���N�^
    virtual ~ShaderHs() override = default;

    //! @brief D3D�V�F�[�_�[���擾
	ID3D11HullShader* get() const;

    //! @brief �V�F�[�_�[�̍쐬
    static gpu::ShaderHs* create(const std::string& fileName);

private:
    com_ptr<ID3D11HullShader> shader_;
};

//===========================================================================
//! @clsss ShaderDs
//===========================================================================
class ShaderDs : public ShaderBase
{
public:
    //! @brief �R���X�g���N�^
    ShaderDs(ID3DBlob* shaderBinary);

    //! @brief �f�X�g���N�^
    virtual ~ShaderDs() override = default;

    //! @brief D3D�V�F�[�_�[���擾
	ID3D11DomainShader* get() const;

    //! @brief �V�F�[�_�[�̍쐬
    static gpu::ShaderDs* create(const std::string& fileName);

private:
    com_ptr<ID3D11DomainShader> shader_;
};

}   // namespace gpu
