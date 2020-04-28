//===========================================================================
//! @file	scene_light.cpp
//!	@brief	ライトシーン
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
SceneLight::SceneLight()
{
    demoSceneType_ = DemoSceneType::Light;
};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneLight::initialize()
{
    //=============================================================
    // カメラ初期化
    //=============================================================
    auto cameraPosition = Vector3(+0.0f, +5.0f, +10.0f);
    auto cameraLookAt   = Vector3(+0.0f, +4.0f, +0.0f);
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
    // テクスチャ初期化
    //=============================================================
    if(!this->initializeTexture())
        return false;

    //=============================================================
    // 描画設定
    //=============================================================
    RenderConfig().initialize();

    A_DemoSceneType::setDemoSceneType(this->getDemoSceneType());

    return true;
}

//---------------------------------------------------------------------------
//! ライト初期化
//---------------------------------------------------------------------------
bool SceneLight::initializeLight()
{
    //=============================================================
    // 平行光源初期化
    //=============================================================
    {
        auto position = Vector3(+20.0f, +15.0f, +0.0f);
        auto lookAt   = Vector3(+0.0f, +1.0f, +0.0f);
        auto color    = Vector4(+1.0f, +1.0f, +1.0f, +1.0f);
        if(!lightManager_->addLight(position, lookAt, color, +50.0f))
            return false;
    }

#if 0
	//=============================================================
	// 点光源初期化
	//=============================================================
	{
		auto position	= Vector3(-1.0f, +5.0f, +6.0f);
		auto color		= Vector4(+0.0f, +0.0f, +1.0f, +1.0f);
		if (!lightManager_->addLight(position, color, +5.0f))
			return false;

		position	= Vector3(-1.0f, +3.0f, +0.0f);
		color		= Vector4(+0.0f, +1.0f, +0.0f, +1.0f);
		if (!lightManager_->addLight(position, color, +5.0f))
			return false;

		position	= Vector3(+5.0f, +3.0f, +0.0f);
		color		= Vector4(+1.0f, +0.0f, +0.0f, +1.0f);
		if (!lightManager_->addLight(position, color, +5.0f))
			return false;
	}

	//=============================================================
	// スポットライト初期化
	//=============================================================
	{
		auto position	= Vector3(-15.0f, +10.0f, +0.0f);
		auto lookAt		= Vector3(-0.0f,  +1.0f,  +0.0f);
		auto color		= Vector4(+1.0f,  +1.0f,  +0.0f, +1.0f);
		if (!lightManager_->addSLight(position, lookAt, color, +8.0f))
			return false;
	}
#endif

    return true;
}

//---------------------------------------------------------------------------
//! モデル初期化
//---------------------------------------------------------------------------
bool SceneLight::initializeModel()
{
    u32 modelCount = 0;

    //=============================================================
    // メモリ確保
    //=============================================================

    // 床
    std::unique_ptr<ObjectBaseModel> floor(new Floor());
    if(!floor)
        return false;

    // スカイボックス
    std::unique_ptr<ObjectBaseModel> skybox(new Skybox());
    if(!skybox)
        return false;

    // 最初に描画するモデル
    std::unique_ptr<ObjectBaseModel> model1(new ObjectBaseModel());
    if(!model1)
        return false;

    std::unique_ptr<ObjectBaseModel> model2(new ObjectBaseModel());
    if(!model2)
        return false;

    //=============================================================
    // 初期化
    //=============================================================
    {
        // 床
        if(!floor->initialize("shape/plane.fbx", +40.0f)) {
            return false;
        }
        ++modelCount;

        // ｓｋｙぼｘ
        if(!skybox->initialize("shape/box.fbx", +40.0f)) {
            return false;
        }
        ++modelCount;

        if(!model1->initialize("Cerberus/Cerberus_LP.fbx", +0.03f)) {
            return false;
        }
        ++modelCount;

        if(!model2->initialize("Knight/Knight.fbx", +4.0f)) {
            return false;
        }

        ++modelCount;
    }

    //=============================================================
    // 設定
    //=============================================================
    {
        floor->setPosition(Vector3::ZERO);
        skybox->setPosition(Vector3::ZERO);
        model1->setPosition({ -3.0f, +3.0f, +1.0f });
        model2->setPosition({ +3.0f, +1.0f, +1.0f });
    }

    //=============================================================
    // 追加
    //=============================================================
    taskManager_->addObject(floor.release());
    taskManager_->addObject(skybox.release());
    taskManager_->addObject(model1.release());
    taskManager_->addObject(model2.release());

#if 0
	std::array<ObjectBaseModel, modelCount> models{

	};
    {
        for(auto& object : objects_) {
            taskManager_->addObject(object);
        }
    }
#endif

    return true;
}

//---------------------------------------------------------------------------
//! モデル初期化
//---------------------------------------------------------------------------
bool SceneLight::initializeTexture()
{
    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SceneLight::update(f32 deleteTime)
{
    // カメラ更新
    cameraManager_->update();

    // ライト更新
    lightManager_->update();

    // オブジェクト更新
    taskManager_->update();
}

//---------------------------------------------------------------------------
//! 描画(モード指定)
//---------------------------------------------------------------------------
void SceneLight::render(RenderMode renderMode)
{
    //
    {
        bool isShadow = (renderMode != RenderMode::Shadow);

        if(RenderConfig().isToonmapping() && isShadow) {
            renderMode = RenderMode::Toon;
        }
        if(RenderConfig().isReflection() && isShadow) {
            renderMode = RenderMode::Reflection;
        }
        if(RenderConfig().isFog() && isShadow) {
            renderMode = RenderMode::Fog;
        }
    }

    this->DemoSceneBase::render(renderMode);

    switch(renderMode) {
        //==============================================================
        // 影描画
        //==============================================================
        case RenderMode::Shadow:
            taskManager_->render(renderMode);

            break;

            //==============================================================
            // シーン描画
            //==============================================================
        case RenderMode::Scene:
            taskManager_->render(renderMode);
            // ライト描画
            lightManager_->render(renderMode);

            break;

            //==============================================================
            // トゥーン描画
            //==============================================================
        case RenderMode::Toon:
            renderMode = RenderMode::Edge;

            // カリング設定
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
            // エッジ描画
            taskManager_->render(renderMode);
            // カリングもどし
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);

            renderMode = RenderMode::Toon;

            // 通常描画
            taskManager_->render(renderMode);
            // ライト
            lightManager_->render(renderMode);

            break;

            //==============================================================
            // 環境反射
            //==============================================================
        case RenderMode::Reflection:
            taskManager_->render(renderMode);

            break;

            //==============================================================
            // 霧
            //==============================================================
        case RenderMode::Fog:
            taskManager_->render(renderMode);

            break;

        default:
            break;
    }
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SceneLight::cleanup()
{
    taskManager_->cleanup();
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void SceneLight::showImGuiWindow()
{
    cameraManager_->showImGuiWindow();
    lightManager_->showImGuiWindow();
    taskManager_->showImGuiWindow();

    // tmp
    ImGui::Begin("Light");
    {
        if(ImGui::Button("Reset")) {
            lightManager_->cleanup();
            this->initializeLight();
        }
    }
    ImGui::End();
}