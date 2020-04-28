//===========================================================================
//!	@file	gpu_shader.h
//!	@brief	シェーダー
//===========================================================================
#pragma once

//===========================================================================
//!	@namespace gpu
//===========================================================================
namespace gpu {

//-----------------------------------------------------------------------
//! シェーダーコンパイル
//!	@param	[in]	fileName		ファイル名
//!	@param	[in]	functionName	シェーダー関数名
//!	@param	[in]	shaderModel		バージョンみたいなもの?
//!	@return	[out]	失敗でnullptr
//-----------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const char* fileName, const char* functionName, const char* shaderModel);

//===========================================================================
//! @class ShaderBase
//===========================================================================
class ShaderBase
{
public:
    //! @brief コンストラクタ
    ShaderBase() = default;

    //! @brief コンストラクタ
    virtual ~ShaderBase() = default;

    //! @brief  シェーダーバイナリのアドレスを取得
    const void* getShaderBinary() const;

	//-----------------------------------------------------------------------
	//! @brief  シェーダーバイナリのサイズを取得
	//! @return シェーダーバイナリのサイズ
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
    //! @brief コンストラクタ
    ShaderVs(ID3DBlob* shaderBinary);

    //! @brief デストラクタ
    virtual ~ShaderVs() override = default;

    //! D@brief 3Dシェーダーを取得
	ID3D11VertexShader* get() const;

    //! @brief シェーダーの作成
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
    //! @brief コンストラクタ
    ShaderPs(ID3DBlob* shaderBinary);

    //! @brief デストラクタ
    virtual ~ShaderPs() override = default;

    //! @brief D3Dシェーダーを取得
	ID3D11PixelShader* get() const;

    //! シェーダーの作成
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
    //! @brief コンストラクタ
    ShaderGs(ID3DBlob* shaderBinary);

    //! @brief デストラクタ
    virtual ~ShaderGs() override = default;

    //! @brief D3Dシェーダーを取得
	ID3D11GeometryShader* get() const;

    //! @brief シェーダーの作成
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
    //! @brief コンストラクタ
    ShaderHs(ID3DBlob* shaderBinary);

    //! @brief デストラクタ
    virtual ~ShaderHs() override = default;

    //! @brief D3Dシェーダーを取得
	ID3D11HullShader* get() const;

    //! @brief シェーダーの作成
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
    //! @brief コンストラクタ
    ShaderDs(ID3DBlob* shaderBinary);

    //! @brief デストラクタ
    virtual ~ShaderDs() override = default;

    //! @brief D3Dシェーダーを取得
	ID3D11DomainShader* get() const;

    //! @brief シェーダーの作成
    static gpu::ShaderDs* create(const std::string& fileName);

private:
    com_ptr<ID3D11DomainShader> shader_;
};

}   // namespace gpu
