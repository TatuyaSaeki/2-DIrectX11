//===========================================================================
//!	@file	camera_manager.cpp
//!	@brief	カメラ管理クラス
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
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
//! 更新
//---------------------------------------------------------------------------
void CameraManager::update()
{
    // カメラ切り替え
    //if(inputKey_->getKeyState(KEY_1, 0x70) == PUSH) {
    //    activeCamera_ = ++activeCamera_ % cameras_.size();
    //}

    // 対応しているカメラ更新
    //if(cameras_[activeCameraIndex_]) {
    //    cameras_[activeCameraIndex_]->update();
    //}

	for (auto& camera : cameras_) {
		camera->update();
	}
}

//---------------------------------------------------------------------------
//! アクティブカメラの定数バッファ転送
//---------------------------------------------------------------------------
void CameraManager::cbTransferActiveCamera()
{
	//=============================================================
    // 定数バッファを更新
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
//! 登録されている全カメラの定数バッファ転送
//---------------------------------------------------------------------------
void CameraManager::cbTransferAllCamera()
{
    s32 cameraSize = s32(cameras_.size());
    if(cameraSize > CAMERA_MAX_COUNT)
        return;

    //=============================================================
    // 定数バッファを更新
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
//! 解放
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
//! カメラ追加
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
//! カメラ追加
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
//! 対応しているカメラのポインタ取得
//---------------------------------------------------------------------------
Camera* CameraManager::getActiveCamera() const
{
    return cameras_[activeCameraIndex_].get();
}

//---------------------------------------------------------------------------
//! 対応しているカメラのビュー行列取得
//---------------------------------------------------------------------------
Matrix CameraManager::getActiveCameraMatView() const
{
    return getActiveCamera()->getMatView();
}

//---------------------------------------------------------------------------
//! 対応しているカメラのプロジェクション行列取得
//---------------------------------------------------------------------------
Matrix CameraManager::getActiveCameraMatProj() const
{
    return getActiveCamera()->getMatProj();
}