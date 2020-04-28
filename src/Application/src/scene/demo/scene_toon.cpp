//===========================================================================
//!	@brief	デモシーン ライト
//===========================================================================

//------------------------------------------------------------------------------------------------
//! 初期化
//------------------------------------------------------------------------------------------------
bool SceneToon::initialize()
{
    //-------------------------------------------
    // カメラ初期化
    //-------------------------------------------
    auto cameraPosition = Vector3(+0.0f, +5.0f, +10.0f);
    auto cameraLookAt   = Vector3(+0.0f, +2.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

    //-------------------------------------------
    // ライト初期化
    //-------------------------------------------
    if(!initializeLight())
        return false;

    //-------------------------------------------
    // モデル初期化
    //-------------------------------------------
    if(!initializeModel())
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------
//! ライト初期化
//------------------------------------------------------------------------------------------------
bool SceneToon::initializeLight()
{
    //-------------------------------------------
    // 平行光源初期化
    //-------------------------------------------
    {
        auto position = Vector3(10.0f, +15.0f, +15.0f);
        auto lookAt   = Vector3(0.0f, 1.0f, 0.0f);
        auto color    = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        if(!lightManager_->addLight(position, lookAt, color, 1.0f))
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------
//! モデル初期化
//------------------------------------------------------------------------------------------------
bool SceneToon::initializeModel()
{
    toonTexture_.reset(gpu::createTexture("model/fbx/Japanese_Temple_Model/Japanese_Temple_Paint2_Japanese_Shrine_Mat_AlbedoTransparency.png"));
    modelTexture_.reset(gpu::createTexture("texture/towerA.png"));

    // 床
    modelFloor_.reset(new Object());
    if(!modelFloor_)
        return false;
    if(!modelFloor_->initialize("plane.fbx", 100.0f))
        return false;

    // FBXモデル
    modelFbx1_.reset(new Object());
    if(!modelFbx1_)
        return false;
    if(!modelFbx1_->initialize("Japanese_Temple_Model/temple_japanese.fbx", 0.5f))
        return false;

    modelFbx2_.reset(new Object());
    if(!modelFbx2_)
        return false;

    if(!modelFbx2_->initialize("Japanese_Temple_Model/temple_japanese.fbx", 0.5f))
        return false;

    // 位置設定
    modelFloor_->setPosition(Vector3::ZERO);
    modelFbx1_->setPosition(Vector3(8.0f, 1.0f, 0.0f));
    modelFbx2_->setPosition(Vector3(-8.0f, 1.0f, 0.0f));

    modelFbx1_->setShader("vsStandard", "psDirectionalLight");
    modelFbx2_->setShader("vsStandard", "psToon");

    taskManager_->addObject(modelFbx1_);
    taskManager_->addObject(modelFbx2_);

	gpu::setTexture(11, toonTexture_.get());

    return true;
}

//------------------------------------------------------------------------------------------------
//! 更新
//------------------------------------------------------------------------------------------------
void SceneToon::update()
{
    // カメラ更新
    cameraManager_->update();

    // ライト更新
    lightManager_->update();

    taskManager_->update();
}

//------------------------------------------------------------------------------------------------
//! 描画
//------------------------------------------------------------------------------------------------
void SceneToon::render()
{
    // グラウンド描画
    gpu::setShader("vsStandard", "psStandard");
    gridRender(128.0f);

    // ライト描画
    gpu::setShader("vsStandard", "psStandard");
    lightManager_->render();

    taskManager_->render();

    // 床描画
    //modelFloor_->setShader("vsStandard", "psLight");
    //modelFloor_->render();
}

//------------------------------------------------------------------------------------------------
//! 描画
//------------------------------------------------------------------------------------------------
void SceneToon::render(RenderMode renderMode)
{
    DemoSceneBase::render(renderMode);

    switch(renderMode) {
        case RenderMode::Scene:
            modelFbx1_->setShader("vsStandard", "psDirectionalLight");
            modelFbx2_->setShader("vsStandard", "psToon");
            break;
        default:
            break;
    }

    render();
}

//------------------------------------------------------------------------------------------------
//! 解放
//------------------------------------------------------------------------------------------------
void SceneToon::cleanup()
{
}
