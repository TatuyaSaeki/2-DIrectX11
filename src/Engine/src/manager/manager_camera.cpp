//===========================================================================
//!	@file	camera_manager.cpp
//!	@brief	�J�����Ǘ��N���X
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool CameraManager::initialize()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    activeCameraIndex_ = 0;

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CameraManager::update()
{
    // �J�����؂�ւ�
    //if(inputKey_->getKeyState(KEY_1, 0x70) == PUSH) {
    //    activeCamera_ = ++activeCamera_ % cameras_.size();
    //}

    // �Ή����Ă���J�����X�V
    //if(cameras_[activeCameraIndex_]) {
    //    cameras_[activeCameraIndex_]->update();
    //}

	for (auto& camera : cameras_) {
		camera->update();
	}
}

//---------------------------------------------------------------------------
//! �A�N�e�B�u�J�����̒萔�o�b�t�@�]��
//---------------------------------------------------------------------------
void CameraManager::cbTransferActiveCamera()
{
	//=============================================================
    // �萔�o�b�t�@���X�V
	//=============================================================
    const auto activeCamera = getActiveCamera();
    if(!activeCamera)
        return;

    auto p = cbCamera_.begin();
    {
        p->matView_[0]        = activeCamera->getMatView();
        p->matProj_[0]        = activeCamera->getMatProj();
        p->cameraPosition_[0] = activeCamera->getPosition();
        p->cameraCount_       = 1;
    }
    cbCamera_.end();

    gpu::setConstantBuffer(gpuSlot_, cbCamera_);
}

//---------------------------------------------------------------------------
//! �o�^����Ă���S�J�����̒萔�o�b�t�@�]��
//---------------------------------------------------------------------------
void CameraManager::cbTransferAllCamera()
{
    s32 cameraSize = s32(cameras_.size());
    if(cameraSize > CAMERA_MAX_COUNT)
        return;

    //=============================================================
    // �萔�o�b�t�@���X�V
	//=============================================================
    auto p = cbCamera_.begin();
    for(s32 i = 1; i < cameraSize; ++i) {
        p->matView_[i - 1]        = cameras_[i]->getMatView();
        p->matProj_[i - 1]        = cameras_[i]->getMatProj();
        p->cameraPosition_[i - 1] = cameras_[i]->getPosition();
        p->cameraCount_           = static_cast<s32>(cameras_.size());
    }
    cbCamera_.end();

    gpu::setConstantBuffer(gpuSlot_, cbCamera_);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void CameraManager::cleanup()
{
    for(auto& camera : cameras_) {
        camera->cleanup();
    }
    cameras_.clear();
}

//---------------------------------------------------------------------------
//! ImGui
//---------------------------------------------------------------------------
void CameraManager::showImGuiWindow()
{
	getActiveCamera()->showImGuiWindow();
}

//---------------------------------------------------------------------------
//! �J�����ǉ�
//---------------------------------------------------------------------------
void CameraManager::addCamera(Camera* const camera)
{
    if(!camera) {
        GM_ASSERT_MESSAGE(false, "Camera Not Registered!!");
        return;
    }

    cameras_.emplace_back(camera);
}

//---------------------------------------------------------------------------
//! �J�����ǉ�
//---------------------------------------------------------------------------
void CameraManager::addCamera(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp)
{
    std::unique_ptr<Camera> camera(new Camera());
    if(!camera) {
        GM_ASSERT_MESSAGE(false, "Camera Not Registered!!");
        return;
    }

    camera->setPosition(position);
    camera->setLookAt(lookAt);
    camera->setWorldUp(worldUp);

    cameras_.emplace_back(camera.release());
}

//---------------------------------------------------------------------------
//! �Ή����Ă���J�����̃|�C���^�擾
//---------------------------------------------------------------------------
Camera* CameraManager::getActiveCamera() const
{
    return cameras_[activeCameraIndex_].get();
}

//---------------------------------------------------------------------------
//! �Ή����Ă���J�����̃r���[�s��擾
//---------------------------------------------------------------------------
Matrix CameraManager::getActiveCameraMatView() const
{
    return getActiveCamera()->getMatView();
}

//---------------------------------------------------------------------------
//! �Ή����Ă���J�����̃v���W�F�N�V�����s��擾
//---------------------------------------------------------------------------
Matrix CameraManager::getActiveCameraMatProj() const
{
    return getActiveCamera()->getMatProj();
}