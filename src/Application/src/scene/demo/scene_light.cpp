//===========================================================================
//! @file	scene_light.cpp
//!	@brief	���C�g�V�[��
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
SceneLight::SceneLight()
{
    demoSceneType_ = DemoSceneType::Light;
};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SceneLight::initialize()
{
    //=============================================================
    // �J����������
    //=============================================================
    auto cameraPosition = Vector3(+0.0f, +5.0f, +10.0f);
    auto cameraLookAt   = Vector3(+0.0f, +4.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

    //=============================================================
    // ���C�g������
    //=============================================================
    if(!this->initializeLight())
        return false;

    //=============================================================
    // ���f��������
    //=============================================================
    if(!this->initializeModel())
        return false;

    //=============================================================
    // �e�N�X�`��������
    //=============================================================
    if(!this->initializeTexture())
        return false;

    //=============================================================
    // �`��ݒ�
    //=============================================================
    RenderConfig().initialize();

    A_DemoSceneType::setDemoSceneType(this->getDemoSceneType());

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g������
//---------------------------------------------------------------------------
bool SceneLight::initializeLight()
{
    //=============================================================
    // ���s����������
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
	// �_����������
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
	// �X�|�b�g���C�g������
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
//! ���f��������
//---------------------------------------------------------------------------
bool SceneLight::initializeModel()
{
    u32 modelCount = 0;

    //=============================================================
    // �������m��
    //=============================================================

    // ��
    std::unique_ptr<ObjectBaseModel> floor(new Floor());
    if(!floor)
        return false;

    // �X�J�C�{�b�N�X
    std::unique_ptr<ObjectBaseModel> skybox(new Skybox());
    if(!skybox)
        return false;

    // �ŏ��ɕ`�悷�郂�f��
    std::unique_ptr<ObjectBaseModel> model1(new ObjectBaseModel());
    if(!model1)
        return false;

    std::unique_ptr<ObjectBaseModel> model2(new ObjectBaseModel());
    if(!model2)
        return false;

    //=============================================================
    // ������
    //=============================================================
    {
        // ��
        if(!floor->initialize("shape/plane.fbx", +40.0f)) {
            return false;
        }
        ++modelCount;

        // �������ڂ�
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
    // �ݒ�
    //=============================================================
    {
        floor->setPosition(Vector3::ZERO);
        skybox->setPosition(Vector3::ZERO);
        model1->setPosition({ -3.0f, +3.0f, +1.0f });
        model2->setPosition({ +3.0f, +1.0f, +1.0f });
    }

    //=============================================================
    // �ǉ�
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
//! ���f��������
//---------------------------------------------------------------------------
bool SceneLight::initializeTexture()
{
    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneLight::update(f32 deleteTime)
{
    // �J�����X�V
    cameraManager_->update();

    // ���C�g�X�V
    lightManager_->update();

    // �I�u�W�F�N�g�X�V
    taskManager_->update();
}

//---------------------------------------------------------------------------
//! �`��(���[�h�w��)
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
        // �e�`��
        //==============================================================
        case RenderMode::Shadow:
            taskManager_->render(renderMode);

            break;

            //==============================================================
            // �V�[���`��
            //==============================================================
        case RenderMode::Scene:
            taskManager_->render(renderMode);
            // ���C�g�`��
            lightManager_->render(renderMode);

            break;

            //==============================================================
            // �g�D�[���`��
            //==============================================================
        case RenderMode::Toon:
            renderMode = RenderMode::Edge;

            // �J�����O�ݒ�
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
            // �G�b�W�`��
            taskManager_->render(renderMode);
            // �J�����O���ǂ�
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);

            renderMode = RenderMode::Toon;

            // �ʏ�`��
            taskManager_->render(renderMode);
            // ���C�g
            lightManager_->render(renderMode);

            break;

            //==============================================================
            // ������
            //==============================================================
        case RenderMode::Reflection:
            taskManager_->render(renderMode);

            break;

            //==============================================================
            // ��
            //==============================================================
        case RenderMode::Fog:
            taskManager_->render(renderMode);

            break;

        default:
            break;
    }
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneLight::cleanup()
{
    taskManager_->cleanup();
}

//---------------------------------------------------------------------------
//! ImGui�`��
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