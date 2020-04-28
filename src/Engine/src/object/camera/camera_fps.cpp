//===========================================================================
//!	@file	camera_fps.cpp
//!	@brief	一人称カメラ
//===========================================================================

//---------------------------------------------------------------------------
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
CameraFPS::CameraFPS()
{
    cameraType_  = CameraType::Fps;
    sensitivity_ = 0.5f;
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool CameraFPS::initialize()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    return Camera::initialize();
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void CameraFPS::update()
{
    //=============================================================
    // マウスで視点回転
    //=============================================================
    POINT pt;
    GetCursorPos(&pt);
    const f32 pointX = static_cast<f32>(pt.x);
    const f32 pointY = static_cast<f32>(pt.y);

    if(oldPoint_.lengthSq() == 0.0f) {
        oldPoint_.x_ = pointX;
        oldPoint_.y_ = pointY;
    }

    f32 diffX = (pointX - oldPoint_.x_) * (sensitivity_ * 0.005f) * -1.0f;
    f32 diffY = (pointY - oldPoint_.y_) * (sensitivity_ * 0.005f) * -1.0f;

    // 現在の位置を保存（次のフレームで利用）
    oldPoint_.x_ = pointX;
    oldPoint_.y_ = pointY;

    //=============================================================
    // カメラの向きに移動
    //=============================================================

    // 移動ベクトル
    Vector3 move{};

    // ローカルでカメラの Z 方向取得
    Vector3 frontdir = matWorld_.axisZ();
    frontdir         = frontdir.normalize();

    // ローカルでカメラの X 方向取得
    Vector3 rightdir = matWorld_.axisX();
    rightdir         = rightdir.normalize();

    // tmp
#if 0
    static s32 a = 0;
    auto       b = getMouseScroll();
    if(a != b) {
        Vector3 axisZ = matWorld_.axisZ();
        if(a < b) {
            position_ -= axisZ * 10.0f;
        }
        else {
            position_ += axisZ * 10.0f;
        }
        a = b;
    }
#endif

    f32 moveSpeed = moveSpeed_;

    if(inputKey_->getKeyState(KEY_LSHIFT, 16) == ON)
        moveSpeed *= 2.0f;

    if(inputKey_->getKeyState(KEY_W, 87) == ON)
        move -= frontdir * moveSpeed;
    if(inputKey_->getKeyState(KEY_S, 83) == ON)
        move += frontdir * moveSpeed;
    if(inputKey_->getKeyState(KEY_A, 65) == ON)
        move -= rightdir * moveSpeed;
    if(inputKey_->getKeyState(KEY_D, 68) == ON)
        move += rightdir * moveSpeed;

    // 水平方向の回転・垂直方向の回転
    bool flag1 = false;

    if((GetKeyState(VK_MBUTTON) & 0x8000)) {
        flag1 = true;
    }

    // カメラ中心
    if((GetKeyState(VK_RBUTTON) & 0x8000)) {
        if((GetKeyState(VK_LBUTTON) & 0x8000)) {
            flag1 = true;
        }
        // 注視点中心
        else if((GetKeyState(VK_CONTROL) & 0x8000)) {
            Vector3 axisX = matWorld_.axisX();
            Matrix  rot   = Matrix::rotateAxis(axisX, diffY) * Matrix::rotateY(diffX);
            Vector3 dir   = (position_ - lookAt_).transform(rot);
            position_     = lookAt_ + dir;   // lookAt再計算
        }
        else {
            // 回転
            Vector3 axisX = matWorld_.axisX();
            Matrix  rot   = Matrix::rotateAxis(axisX, diffY) * Matrix::rotateY(diffX);
            Vector3 dir   = (lookAt_ - position_).transform(rot);
            lookAt_       = position_ + dir;   // lookAt再計算
        }
    }

    // 平行移動
    if(flag1) {
        //moveSpeed_    = 8.0f;
        Vector3 upDir = matWorld_.axisY().normalize();
        move += (rightdir * diffX) * (moveSpeed * 100.0f) * -1.0f;
        move += (upDir * diffY) * (moveSpeed * 100.0f) * 1.0f;
    }

    position_ += move;
    lookAt_ += move;

    Camera::update();
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void CameraFPS::cleanup()
{
    Camera::cleanup();
}

//---------------------------------------------------------------------------
//! ImGui
//---------------------------------------------------------------------------
void CameraFPS::showImGuiWindow()
{
    ImGui::Begin("Object");
    if(ImGui::TreeNode("Camera")) {
        ImGui::SliderFloat("MoveSpeed", &moveSpeed_, 0.00001f, 1.0f);
        ImGui::SliderFloat("Sensitivity", &sensitivity_, 0.001f, 1.0f);
        ImGui::TreePop();
    }

    ImGui::End();
}