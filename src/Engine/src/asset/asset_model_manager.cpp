//===========================================================================
//! @file	asset_model_manager.cpp
//!	@brief	アセットモデル管理クラス
//===========================================================================

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
AssetModelManager::~AssetModelManager()
{
	this->cleanup();
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool AssetModelManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! 解放
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
//! モデル取得
//---------------------------------------------------------------------------
std::shared_ptr<AssetModel> AssetModelManager::getModel(const std::string& fileName)
{
    // ない場合
    if(!assetModels_.count(fileName)) {
        auto model = this->createModel(fileName);
        if(!model)
            return nullptr;
		// 配列追加
		this->addModel(fileName, model);
    }

    return assetModels_[fileName];
}

//---------------------------------------------------------------------------
//! モデル作成
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
//!	モデル追加
//---------------------------------------------------------------------------
void AssetModelManager::addModel(const std::string& fileName, AssetModel* const assetModel)
{
    assetModels_.insert(std::make_pair(fileName, assetModel));
}
