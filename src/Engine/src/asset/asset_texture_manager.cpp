//===========================================================================
//! @file	asset_texture_manager.cpp
//!	@brief	テクスチャ管理クラス
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool AssetTextureManager::initialize()
{
    //=============================================================
    //	システムテクスチャ初期化
    //=============================================================
	systemTextures_[SYSTEM_TEXTURE_NULL_WHITE].reset(this->createTexture("texture/system/NullWhite.dds"));
	systemTextures_[SYSTEM_TEXTURE_NULL_BLACK].reset(this->createTexture("texture/system/NullBlack.dds"));
	systemTextures_[SYSTEM_TEXTURE_NULL_NORMAL].reset(this->createTexture("texture/system/NullNormal.png"));
	systemTextures_[SYSTEM_TEXTURE_CUBEMAP_NULL_BLACK].reset(this->createTexture("texture/system/NullBlackCubemap.dds"));
	systemTextures_[SYSTEM_TEXTURE_TOON].reset(this->createTexture("texture/system/toon.png"));

    for(u32 i = 0; i < (u32)std::size(systemTextures_); ++i) {
        if(!systemTextures_[i]) {
            GM_ASSERT_MESSAGE(false, "システムテクスチャ初期化失敗");
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void AssetTextureManager::cleanup()
{
    for(auto& systemTexture : systemTextures_) {
        systemTexture.reset();
    }

    for(auto& assetTexture : assetTextures_) {
        auto texture = assetTexture.second;
        if(texture) {
            texture.reset();
        }
    }

    assetTextures_.clear();
}

//---------------------------------------------------------------------------
//! システムテクスチャ取得
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetTextureManager::getSystemTexture(SYSTEM_TEXTURE type)
{
    if(SYSTEM_TEXTURE_MAX <= type) {
        GM_ASSERT_MESSAGE(false, "getSystemTexture()");
    }

    return systemTextures_[type];
}

//---------------------------------------------------------------------------
//! テクスチャ取得
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetTextureManager::getTexture(const std::string& fileName, [[maybe_unused]] bool isCubemap)
{
    // ない場合
    if(!assetTextures_.count(fileName)) {
        auto texture = this->createTexture(fileName);
        if(!texture) {
            GM_ASSERT_MESSAGE(false, "テクスチャの作成に失敗");
            return nullptr;
        }
        this->addTexture(fileName, texture);
    }

    return assetTextures_[fileName];
}

//---------------------------------------------------------------------------
//! テクスチャ作成
//---------------------------------------------------------------------------
gpu::Texture* AssetTextureManager::createTexture(const std::string& fileName, bool isCubemap)
{
    std::unique_ptr<gpu::Texture> texture(gpu::createTexture(fileName, isCubemap));

    if(!texture) {
        return nullptr;
    }

    return texture.release();
}

//---------------------------------------------------------------------------
//!	テクスチャ追加
//---------------------------------------------------------------------------
void AssetTextureManager::addTexture(const std::string& fileName, gpu::Texture* assetTexture)
{
    assetTextures_.insert(std::make_pair(fileName, assetTexture));
}
