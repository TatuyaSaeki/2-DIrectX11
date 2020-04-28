//===========================================================================
//! @file	demo_scene_base.cpp
//!	@brief	�f���V�[���̃x�[�X
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
//! �R���X�g���N�^
//---------------------------------------------------------------------------
DemoSceneBase::DemoSceneBase()
{
    //=============================================================
    // �L�[������
    //=============================================================
    if(!initializeKeyInput()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | keyInitialize");
        return;
    }

    //=============================================================
    // �^�X�N�}�l�[�W���[������
    //=============================================================
    if(!initializeTaskManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | initializeTaskManager");
        return;
    }

    //=============================================================
    // �J�����}�l�[�W���[������
    //=============================================================
    if(!initializeCameraManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | cameraManagerInitialize");
        return;
    }
    //taskManager_->addManager(cameraManager_.get());

    //=============================================================
    // ���C�g�}�l�[�W���[������
    //=============================================================
    if(!initializeLightManager()) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base | initializelightManager");
        return;
    }
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
DemoSceneBase::~DemoSceneBase()
{
    this->cleanup();
}

//---------------------------------------------------------------------------
//! �L�[������
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeKeyInput()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_) {
        GM_ASSERT_MESSAGE(false, "demo_scene_base 33�s��");
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! �J�����}�l�[�W���[������
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
//! ���C�g�}�l�[�W���[������
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
//! �^�X�N�}�l�[�W��������
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
//! �J����������
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeCameraFps(const Vector3& position, const Vector3& lookAt)
{
    //=============================================================
    // FPS�J����������
    //=============================================================
    auto worldCamera = new CameraFPS();
    if(!worldCamera)
        return false;
    if(!worldCamera->initialize())
        return false;

    // �ʒu�ݒ�
    worldCamera->setPosition(position);
    worldCamera->setLookAt(lookAt);

    // �J�����ǉ�
    cameraManager_->addCamera(worldCamera);

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g������
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeLight()
{
    return true;
}

//---------------------------------------------------------------------------
//! ���f��������
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeModel()
{
    return true;
}

//---------------------------------------------------------------------------
//! �e�N�X�`��������
//---------------------------------------------------------------------------
bool DemoSceneBase::initializeTexture()
{
    return true;
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void DemoSceneBase::render(RenderMode renderMode)
{
    // �S���ʃV���h�E�}�b�v�p(������)
    if(renderMode == RenderMode::Shadow) {
        cameraManager_->cbTransferAllCamera();
        return;
    }

    // �V�[���J�����̂�
    cameraManager_->cbTransferActiveCamera();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void DemoSceneBase::cleanup()
{
    taskManager_->cleanup();
    cameraManager_->cleanup();
    lightManager_->cleanup();
}

//---------------------------------------------------------------------------
//! ���s�����擾
//---------------------------------------------------------------------------
Light* DemoSceneBase::getDirectionalLight() const
{
    return lightManager_->getDirectionalLight();
}

//---------------------------------------------------------------------------
//! ���݂̃f���V�[���^�C�v�擾
//---------------------------------------------------------------------------
DemoSceneType DemoSceneBase::getDemoSceneType() const
{
    return demoSceneType_;
}