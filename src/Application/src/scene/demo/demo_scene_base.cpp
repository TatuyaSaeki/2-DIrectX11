//===========================================================================
//! @file	demo_scene_base.cpp
//!	@brief	デモシーンのベース
//===========================================================================

// TMP
namespace A_DemoSceneType {
DemoSceneType demoSceneType = DemoSceneType::None;

void setDemoSceneType(DemoSceneType sceneType)
{
    demoSceneType = sceneType;
}

DemoSceneType getDemoSceneType()
{
    return demoSceneType;
}
}   // namespace A_DemoSceneType

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
DemoSceneBase::DemoSceneBase()
{
    //=============================================================
    // キー初期化
    //=============================================================
    if(!initializeKeyInput()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | keyInitialize");
        return;
    }

    //=============================================================
    // タスクマネージャー初期化
    //=============================================================
    if(!initializeTaskManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | initializeTaskManager");
        return;
    }

    //=============================================================
    // カメラマネージャー初期化
    //=============================================================
    if(!initializeCameraManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | cameraManagerInitialize");
        return;
    }
    //taskManager_->addManager(cameraManager_.get());

    //=============================================================
    // ライトマネージャー初期化
    //=============================================================
    if(!initializeLightManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | initializelightManager");
        return;
    }
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
DemoSceneBase::~DemoSceneBase()
{
    this->cleanup();
}

//---------------------------------------------------------------------------
//! キー初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeKeyInput()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base 33行目");
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! カメラマネージャー初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeCameraManager()
{
    cameraManager_.reset(new CameraManager());
    if(!cameraManager_)
        return false;
    if(!cameraManager_->initialize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! ライトマネージャー初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeLightManager()
{
    lightManager_.reset(new LightManager());
    if(!lightManager_)
        return false;
    if(!lightManager_->initialize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! タスクマネージャ初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeTaskManager()
{
    taskManager_.reset(new TaskManager());
    if(!taskManager_)
        return false;
    if(!taskManager_->initialize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! カメラ初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeCameraFps(const Vector3& position, const Vector3& lookAt)
{
    //=============================================================
    // FPSカメラ初期化
    //=============================================================
    auto worldCamera = new CameraFPS();
    if(!worldCamera)
        return false;
    if(!worldCamera->initialize())
        return false;

    // 位置設定
    worldCamera->setPosition(position);
    worldCamera->setLookAt(lookAt);

    // カメラ追加
    cameraManager_->addCamera(worldCamera);

    return true;
}

//---------------------------------------------------------------------------
//! ライト初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeLight()
{
    return true;
}

//---------------------------------------------------------------------------
//! モデル初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeModel()
{
    return true;
}

//---------------------------------------------------------------------------
//! テクスチャ初期化
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeTexture()
{
    return true;
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void DemoSceneBase::render(RenderMode renderMode)
{
    // 全方位シャドウマップ用(未実装)
    if(renderMode == RenderMode::Shadow) {
        cameraManager_->cbTransferAllCamera();
        return;
    }

    // シーンカメラのみ
    cameraManager_->cbTransferActiveCamera();
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void DemoSceneBase::cleanup()
{
    taskManager_->cleanup();
    cameraManager_->cleanup();
    lightManager_->cleanup();
}

//---------------------------------------------------------------------------
//! 平行光源取得
//---------------------------------------------------------------------------
Light* DemoSceneBase::getDirectionalLight() const
{
    return lightManager_->getDirectionalLight();
}

//---------------------------------------------------------------------------
//! 現在のデモシーンタイプ取得
//---------------------------------------------------------------------------
DemoSceneType DemoSceneBase::getDemoSceneType() const
{
    return demoSceneType_;
}