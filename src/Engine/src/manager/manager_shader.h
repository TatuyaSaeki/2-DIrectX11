//===========================================================================
//! @file	shader_manager.h
//!	@brief	シェーダー管理
//===========================================================================
#pragma once

// まとめてるだけ
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
    //! @name	初期化
	//-----------------------------------------------------------------------
    //@{

    //! @brief デストラクタ
    ~ShaderManager();

    //@}
    //---------------------------------------------------------------------------
    //! @name	取得
    //---------------------------------------------------------------------------

    //! @brief 入力レイアウト取得
    static gpu::InputLayout* const getInputLayout(const std::string& funcName);

    //! @brief 頂点シェーダーを取得
    static gpu::ShaderVs* const getShaderVs(const std::string& fileName);

    //! @brief ピクセルシェーダーを取得
    static gpu::ShaderPs* const getShaderPs(const std::string& fileName);

    //! @brief ジオメトリシェーダーを取得
    static gpu::ShaderGs* const getShaderGs(const std::string& fileName);

    //! @brief ハルシェーダーを取得
    static gpu::ShaderHs* const getShaderHs(const std::string& fileName);

    //! @brief ドメインシェーダーを取得
    static gpu::ShaderDs* const getShaderDs(const std::string& fileName);

	//@}

private:

	//! @brief コンストラクタ
    ShaderManager() = default;

    //! @brief 入力レイアウト作成
    static gpu::InputLayout* createInputLayout(const std::string& funcName);

private:
    //std::unordered_map<std::string, ShaderVs>                       shaderVs_;
    //std::unordered_map<std::string, std::unique_ptr<gpu::ShaderPs>> shaderPs_;
};
