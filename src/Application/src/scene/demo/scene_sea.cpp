//===========================================================================
//! @file	scene_sea.cpp
//!	@brief	�f���V�[�� �C
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
SceneSea::SceneSea()
{
    demoSceneType_ = DemoSceneType::Sea;
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SceneSea::initialize()
{
    //=============================================================
    // �J����������
    //=============================================================
    auto cameraPosition = Vector3(+0.0f, +20.0f, +70.0f);
    auto cameraLookAt   = Vector3(+0.0f, +1.0f, +0.0f);
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
    // �`��ݒ�
    //=============================================================
    RenderConfig().initialize();

	RenderConfig().setIBLTextureType(IBLTextureType::BlueSky);
    RenderConfig().isGlare(false);
    A_DemoSceneType::setDemoSceneType(this->getDemoSceneType());

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g������
//---------------------------------------------------------------------------
bool SceneSea::initializeLight()
{
    //=============================================================
    // ���s����������
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
//! ���f��������
//---------------------------------------------------------------------------
bool SceneSea::initializeModel()
{
    //=============================================================
    // ���f��
    //=============================================================
    {
        // �C
        std::unique_ptr<ObjectBaseModel> sea(new Sea());
        // �X�J�C�{�b�N�X
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

        // �^�X�N�ɒǉ�
        taskManager_->addObject(sea.release());
        taskManager_->addObject(skybox.release());
    }

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneSea::update(f32 deleteTime)
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
void SceneSea::render(RenderMode renderMode)
{
    this->DemoSceneBase::render(renderMode);

    // �I�u�W�F�N�g�`��
    taskManager_->render(renderMode);

    // ���C�g�`��
    lightManager_->render(renderMode);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneSea::cleanup()
{
}

//---------------------------------------------------------------------------
//! ImGui�`��
//---------------------------------------------------------------------------
void SceneSea::showImGuiWindow()
{
    cameraManager_->showImGuiWindow();
    taskManager_->showImGuiWindow();
}