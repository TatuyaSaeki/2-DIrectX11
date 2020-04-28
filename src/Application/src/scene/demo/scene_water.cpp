//===========================================================================
//! @file	scene_water.cpp
//!	@brief	水用シーン
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
SceneWater::SceneWater()
{
    demoSceneType_ = DemoSceneType::Water;
};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneWater::initialize()
{
    //=============================================================
    // カメラ初期化
    //=============================================================
    //auto cameraPosition = Vector3(+0.0f, +30.0f, +50.0f);
    auto cameraPosition = Vector3(+0.0f, +40.0f, +80.0f);
    auto cameraLookAt   = Vector3(+0.0f, +10.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

    //=============================================================
    // ライト初期化
    //=============================================================
    if(!this->initializeLight())
        return false;

    //=============================================================
    // テクスチャ初期化 (モデルより最初じゃないとダメ)
    //=============================================================
    if(!this->initializeTexture())
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

    //RenderConfig().isSkyBox(false);
    RenderConfig().isTonemapping(false);
    RenderConfig().isIBL(false);
    RenderConfig().isGlare(true);

    A_DemoSceneType::setDemoSceneType(this->getDemoSceneType());

    return true;
}

//---------------------------------------------------------------------------
//! ライト初期化
//---------------------------------------------------------------------------
bool SceneWater::initializeLight()
{
    //=============================================================
    // 平行光源初期化
    //=============================================================
    {
        auto position = Vector3(20.0f, 30.0f, -50.0f);
        auto lookAt   = Vector3(0.0f, 0.0f, 0.0f);
        auto color    = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        if(!lightManager_->addLight(position, lookAt, color, 8.0f))
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! モデル初期化
//---------------------------------------------------------------------------
bool SceneWater::initializeModel()
{
    //=============================================================
    // メモリ確保
    //=============================================================
    std::unique_ptr<ObjectBaseModel> floor(new Floor());
    if(!floor)
        return false;

    // ObjectBaseModelじゃないのはわざと
    std::unique_ptr<Water> water(new Water());
    if(!water)
        return false;

    std::unique_ptr<ObjectBaseModel> model(new ObjectBaseModel());
    if(!model)
        return false;

    //=============================================================
    // 初期化
    //=============================================================
    {
        if(!floor->initialize("shape/plane.fbx", 60.0f))
            return false;

        if(!water->initialize(buffer_, "shape/plane.fbx", 30.0f, PRIMITIVE_TYPE::PT_3_CONTROL_POINT))
            return false;

        if(!model->initialize("Cerberus/Cerberus_LP.fbx", +0.03f))
            return false;
    }

    //=============================================================
    // 設定
    //=============================================================
    {
        floor->setPosition(+0.0f, -3.0f, +0.0f);
        floor->setTexture(TEX_ALBEDO, Asset::getTexture("texture/Brick.jpg"));
        floor->setShader("vsStandard", "psTexture3D");

        static std::string hsName = "hsWater";
        static std::string dsName = "dsWater";
        water->setShader("vsWater", "psWater", nullptr, &hsName, &dsName);
        water->setTexture(TEX_NORMAL_1, Asset::getTexture("texture/water/water_normal1.png"));
        water->setTexture(TEX_NORMAL_2, Asset::getTexture("texture/water/water_normal2.jpg"));
        water->setTexture(TEX_HEIGHT_1, Asset::getTexture("texture/water/water_height1.png"));
        water->setTexture(TEX_HEIGHT_2, Asset::getTexture("texture/water/water_height2.jpg"));

		model->setPosition({ +0.0f, +3.0f, +0.0f });
    }

    //=============================================================
    // 追加
    //=============================================================
    {
        taskManager_->addObject(floor.release());
        taskManager_->addObject(water.release());
		taskManager_->addObject(model.release());
    }

    return true;
}

//---------------------------------------------------------------------------
//! テクスチャ初期化
//---------------------------------------------------------------------------
bool SceneWater::initializeTexture()
{
    buffer_.reset(std::move(gpu::createRenderTarget(2048, 2048, DXGI_FORMAT_R16G16B16A16_FLOAT)));

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SceneWater::update(f32 deleteTime)
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
void SceneWater::render(RenderMode renderMode)
{
    if(renderMode != RenderMode::Scene)
        return;

    this->DemoSceneBase::render(renderMode);

    //if(renderMode == RenderMode::Scene)
    renderMode = RenderMode::Water1;   // 床用にシーンとは別に設定

    // カラーバッファクリア
    clearTarget(buffer_, { 1.0f, 1.0f, 1.0f, 1.0f });

    // 水用処理
    {
        // 描画バッファ変更
        gpu::setRenderTarget(buffer_.get());

        // 描画(1pass)
        taskManager_->render(renderMode);

        // バッファ元に戻す
        gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());

        renderMode = RenderMode::Water2;

        // 描画(2pass)
        taskManager_->render(renderMode);
    }

    // ライト描画
    lightManager_->render(renderMode);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SceneWater::cleanup()
{
    buffer_.reset();
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void SceneWater::showImGuiWindow()
{
    cameraManager_->showImGuiWindow();
    taskManager_->showImGuiWindow();

    ImGui::Begin("Light");
    if(ImGui::TreeNode("Lights")) {
        lightManager_->getDirectionalLight()->showImGuiWindow();
        ImGui::TreePop();
    }
    ImGui::End();
}