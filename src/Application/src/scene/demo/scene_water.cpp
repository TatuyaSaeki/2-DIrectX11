//===========================================================================
//! @file	scene_water.cpp
//!	@brief	���p�V�[��
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
SceneWater::SceneWater()
{
    demoSceneType_ = DemoSceneType::Water;
};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SceneWater::initialize()
{
    //=============================================================
    // �J����������
    //=============================================================
    //auto cameraPosition = Vector3(+0.0f, +30.0f, +50.0f);
    auto cameraPosition = Vector3(+0.0f, +40.0f, +80.0f);
    auto cameraLookAt   = Vector3(+0.0f, +10.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

    //=============================================================
    // ���C�g������
    //=============================================================
    if(!this->initializeLight())
        return false;

    //=============================================================
    // �e�N�X�`�������� (���f�����ŏ�����Ȃ��ƃ_��)
    //=============================================================
    if(!this->initializeTexture())
        return false;

    //=============================================================
    // ���f��������
    //=============================================================
    if(!this->initializeModel())
        return false;

    //=============================================================
    // �`��ݒ�
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
//! ���C�g������
//---------------------------------------------------------------------------
bool SceneWater::initializeLight()
{
    //=============================================================
    // ���s����������
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
//! ���f��������
//---------------------------------------------------------------------------
bool SceneWater::initializeModel()
{
    //=============================================================
    // �������m��
    //=============================================================
    std::unique_ptr<ObjectBaseModel> floor(new Floor());
    if(!floor)
        return false;

    // ObjectBaseModel����Ȃ��̂͂킴��
    std::unique_ptr<Water> water(new Water());
    if(!water)
        return false;

    std::unique_ptr<ObjectBaseModel> model(new ObjectBaseModel());
    if(!model)
        return false;

    //=============================================================
    // ������
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
    // �ݒ�
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
    // �ǉ�
    //=============================================================
    {
        taskManager_->addObject(floor.release());
        taskManager_->addObject(water.release());
		taskManager_->addObject(model.release());
    }

    return true;
}

//---------------------------------------------------------------------------
//! �e�N�X�`��������
//---------------------------------------------------------------------------
bool SceneWater::initializeTexture()
{
    buffer_.reset(std::move(gpu::createRenderTarget(2048, 2048, DXGI_FORMAT_R16G16B16A16_FLOAT)));

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneWater::update(f32 deleteTime)
{
    // �J�����X�V
    cameraManager_->update();

    // ���C�g�X�V
    lightManager_->update();

    // �I�u�W�F�N�g�X�V
    taskManager_->update();
}

//---------------------------------------------------------------------------
//! �`��(�`�惂�[�h�w��)
//---------------------------------------------------------------------------
void SceneWater::render(RenderMode renderMode)
{
    if(renderMode != RenderMode::Scene)
        return;

    this->DemoSceneBase::render(renderMode);

    //if(renderMode == RenderMode::Scene)
    renderMode = RenderMode::Water1;   // ���p�ɃV�[���Ƃ͕ʂɐݒ�

    // �J���[�o�b�t�@�N���A
    clearTarget(buffer_, { 1.0f, 1.0f, 1.0f, 1.0f });

    // ���p����
    {
        // �`��o�b�t�@�ύX
        gpu::setRenderTarget(buffer_.get());

        // �`��(1pass)
        taskManager_->render(renderMode);

        // �o�b�t�@���ɖ߂�
        gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());

        renderMode = RenderMode::Water2;

        // �`��(2pass)
        taskManager_->render(renderMode);
    }

    // ���C�g�`��
    lightManager_->render(renderMode);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneWater::cleanup()
{
    buffer_.reset();
}

//---------------------------------------------------------------------------
//! ImGui�`��
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