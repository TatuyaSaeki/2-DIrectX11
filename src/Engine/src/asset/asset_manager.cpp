//===========================================================================
//! @file	AssetManager.cpp
//! @brief	�A�Z�b�g�S�ʊǗ��p�N���X
//===========================================================================

//-----------------------------------------------------------------------
//! �C���X�^���X�擾
//-----------------------------------------------------------------------
AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
AssetManager::AssetManager()
{
    assetModelManager_.reset(new AssetModelManager());
    if(!SAFE_TASK::initialize(assetModelManager_.get())) {
        GM_ASSERT_MESSAGE(false, "assetModelManager_���������s");
    }

    assetTextureManager_.reset(new AssetTextureManager());
    if(!SAFE_TASK::initialize(assetTextureManager_.get())) {
        GM_ASSERT_MESSAGE(false, "assetTextureManager_���������s");
    }
}

//---------------------------------------------------------------------------
//! ���f���擾
//---------------------------------------------------------------------------
std::shared_ptr<AssetModel> AssetManager::getModel(const std::string& filePath) const
{
    return assetModelManager_->getModel(filePath);
}

//---------------------------------------------------------------------------
//! �V�X�e���e�N�X�`���擾
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetManager::getSystemTexture(SYSTEM_TEXTURE type) const
{
    return assetTextureManager_->getSystemTexture(type);
}

//---------------------------------------------------------------------------
//! �e�N�X�`���擾
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> AssetManager::getTexture(const std::string& filePath, bool isCubemap) const
{
    return assetTextureManager_->getTexture(filePath, isCubemap);
}

//===========================================================================
//!	namespace Asset
//===========================================================================
namespace Asset {

//! ���f���̎擾
std::shared_ptr<AssetModel> getModel(const std::string& filePath)
{
    return AssetManager::getInstance().getModel(filePath);
}

//! �V�X�e���e�N�X�`���擾
std::shared_ptr<gpu::Texture> getSystemTexture(SYSTEM_TEXTURE type)
{
    return AssetManager::getInstance().getSystemTexture(type);
}

//! �e�N�X�`���̎擾
std::shared_ptr<gpu::Texture> getTexture(const std::string& filePath, bool isCubemap)
{
    return AssetManager::getInstance().getTexture(filePath, isCubemap);
}

}   // namespace Asset