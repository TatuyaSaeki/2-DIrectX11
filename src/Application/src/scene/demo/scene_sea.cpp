//===========================================================================
//! @file	scene_sea.cpp
//!	@brief	デモシーン 海
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
SceneSea::SceneSea()
{
    demoSceneType_ = DemoSceneType::Sea;
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneSea::initialize()
{
    //=============================================================
    // カメラ初期化
    //=============================================================
    auto cameraPosition = Vector3(+0.0f, +20.0f, +70.0f);
    auto cameraLookAt   = Vector3(+0.0f, +1.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

    //=============================================================
    // ライト初期化
    //=============================================================
    if(!this->initializeLight())
        return false;

    //=============================================================
    // モデル初期化
    //=============================================================
    if(!this->initializeModel())
        return false;

    //=============================================================
    // 描画設定
    //=============================================================
    RenderConfig().initialize();

	RenderConfig().setIBLTextureType(IBLTextureType::BlueSky);
    RenderConfig().isGlare(false);
    A_DemoSceneType::setDemoSceneType(this->getDemoSceneType());

    return true;
}

//---------------------------------------------------------------------------
//! ライト初期化
//---------------------------------------------------------------------------
bool SceneSea::initializeLight()
{
    //=============================================================
    // 平行光源初期化
    //=============================================================
    {
        auto position = Vector3(0.0f, 5.0f, 0.0f);
        auto lookAt   = Vector3(0.0f, 0.0f, 0.0f);
        auto color    = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        if(!lightManager_->addLight(position, lookAt, color, 1.0f))
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! モデル初期化
//---------------------------------------------------------------------------
bool SceneSea::initializeModel()
{
    //=============================================================
    // モデル
    //=============================================================
    {
        // 海
        std::unique_ptr<ObjectBaseModel> sea(new Sea());
        // スカイボックス
        std::unique_ptr<ObjectBaseModel> skybox(new Skybox());

        if(!skybox)
            return false;
        if(!sea)
            return false;

        if(!sea->initialize("shape/Subdivision_Plane.fbx", 200.0f))
            return false;
        if(!skybox->initialize("shape/box.fbx", +200.0f))
            return false;

        sea->setShader("vsSea2", "psSea2");

        skybox->setPosition(Vector3::ZERO);

        // タスクに追加
        taskManager_->addObject(sea.release());
        taskManager_->addObject(skybox.release());
    }

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SceneSea::update(f32 deleteTime)
{
    // カメラ更新
    cameraManager_->update();

    // ライト更新
    lightManager_->update();

    // オブジェクト更新
    taskManager_->update();
}

//---------------------------------------------------------------------------
//! 描画(描画モード指定)
//---------------------------------------------------------------------------
void SceneSea::render(RenderMode renderMode)
{
    this->DemoSceneBase::render(renderMode);

    // オブジェクト描画
    taskManager_->render(renderMode);

    // ライト描画
    lightManager_->render(renderMode);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SceneSea::cleanup()
{
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void SceneSea::showImGuiWindow()
{
    cameraManager_->showImGuiWindow();
    taskManager_->showImGuiWindow();
}