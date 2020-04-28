//===========================================================================
//! @file	asset_model_manager.cpp
//!	@brief	�A�Z�b�g���f���Ǘ��N���X
//===========================================================================

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
AssetModelManager::~AssetModelManager()
{
	this->cleanup();
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool AssetModelManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void AssetModelManager::cleanup()
{
    for(auto assetModel : assetModels_) {
        auto model = assetModel.second;
        if(model) {
            model->cleanup();
            model.reset();
        }
    }

    assetModels_.clear();
}

//---------------------------------------------------------------------------
//! ���f���擾
//---------------------------------------------------------------------------
std::shared_ptr<AssetModel> AssetModelManager::getModel(const std::string& fileName)
{
    // �Ȃ��ꍇ
    if(!assetModels_.count(fileName)) {
        auto model = this->createModel(fileName);
        if(!model)
            return nullptr;
		// �z��ǉ�
		this->addModel(fileName, model);
    }

    return assetModels_[fileName];
}

//---------------------------------------------------------------------------
//! ���f���쐬
//---------------------------------------------------------------------------
AssetModel* AssetModelManager::createModel(const std::string& fileName)
{
    // FBX
    if(fileName.find(".fbx") != std::string::npos ||
       fileName.find(".FBX") != std::string::npos) {

        std::unique_ptr<AssetModelFBX> model(new AssetModelFBX());
		if (!model)
			return false;
        if(!model->load(fileName.c_str()))
            return nullptr;

        return model.release();
    }

    // mqo

    return nullptr;
}

//---------------------------------------------------------------------------
//!	���f���ǉ�
//---------------------------------------------------------------------------
void AssetModelManager::addModel(const std::string& fileName, AssetModel* const assetModel)
{
    assetModels_.insert(std::make_pair(fileName, assetModel));
}
