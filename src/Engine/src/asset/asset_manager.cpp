//===========================================================================
//! @file	AssetManager.cpp
//! @brief	アセット全般管理用クラス
//===========================================================================

//-----------------------------------------------------------------------
//! インスタンス取得
//-----------------------------------------------------------------------
AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
AssetManager::AssetManager()
{
    assetModelManager_.reset(new AssetModelManager());
    if(!SAFE_TASK::initialize(assetModelManager_.get())) {
        GM_ASSERT_MESSAGE(false, "assetModelManager_初期化失敗");
    }

    assetTextureManager_.reset(new AssetTextureManager());
    if(!SAFE_TASK::initialize(assetTextureManager_.get())) {
        GM_ASSERT_MESSAGE(false, "assetTextureManager_初期化失敗");
    }
}

//---------------------------------------------------------------------------
//! モデル取得
//---------------------------------------------------------------------------
std::shared_ptr<AssetModel> AssetManager::getModel(const std::string& filePath) const
{
    return assetModelManager_->getModel(filePath);
}

//---------------------------------------------------------------------------
//! システムテクスチャ取得
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetManager::getSystemTexture(SYSTEM_TEXTURE type) const
{
    return assetTextureManager_->getSystemTexture(type);
}

//---------------------------------------------------------------------------
//! テクスチャ取得
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetManager::getTexture(const std::string& filePath, bool isCubemap) const
{
    return assetTextureManager_->getTexture(filePath, isCubemap);
}

//===========================================================================
//!	namespace Asset
//===========================================================================
namespace Asset {

//! モデルの取得
std::shared_ptr<AssetModel> getModel(const std::string& filePath)
{
    return AssetManager::getInstance().getModel(filePath);
}

//! システムテクスチャ取得
std::shared_ptr<gpu::Texture> getSystemTexture(SYSTEM_TEXTURE type)
{
    return AssetManager::getInstance().getSystemTexture(type);
}

//! テクスチャの取得
std::shared_ptr<gpu::Texture> getTexture(const std::string& filePath, bool isCubemap)
{
    return AssetManager::getInstance().getTexture(filePath, isCubemap);
}

}   // namespace Asset