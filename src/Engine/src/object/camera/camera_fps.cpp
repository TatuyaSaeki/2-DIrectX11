//===========================================================================
//!	@file	camera_fps.cpp
//!	@brief	��l�̃J����
//===========================================================================

//---------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------------
CameraFPS::CameraFPS()
{
    cameraType_  = CameraType::Fps;
    sensitivity_ = 0.5f;
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool CameraFPS::initialize()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    return Camera::initialize();
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CameraFPS::update()
{
    //=============================================================
    // �}�E�X�Ŏ��_��]
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

    // ���݂̈ʒu��ۑ��i���̃t���[���ŗ��p�j
    oldPoint_.x_ = pointX;
    oldPoint_.y_ = pointY;

    //=============================================================
    // �J�����̌����Ɉړ�
    //=============================================================

    // �ړ��x�N�g��
    Vector3 move{};

    // ���[�J���ŃJ������ Z �����擾
    Vector3 frontdir = matWorld_.axisZ();
    frontdir         = frontdir.normalize();

    // ���[�J���ŃJ������ X �����擾
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

    // ���������̉�]�E���������̉�]
    bool flag1 = false;

    if((GetKeyState(VK_MBUTTON) & 0x8000)) {
        flag1 = true;
    }

    // �J�������S
    if((GetKeyState(VK_RBUTTON) & 0x8000)) {
        if((GetKeyState(VK_LBUTTON) & 0x8000)) {
            flag1 = true;
        }
        // �����_���S
        else if((GetKeyState(VK_CONTROL) & 0x8000)) {
            Vector3 axisX = matWorld_.axisX();
            Matrix  rot   = Matrix::rotateAxis(axisX, diffY) * Matrix::rotateY(diffX);
            Vector3 dir   = (position_ - lookAt_).transform(rot);
            position_     = lookAt_ + dir;   // lookAt�Čv�Z
        }
        else {
            // ��]
            Vector3 axisX = matWorld_.axisX();
            Matrix  rot   = Matrix::rotateAxis(axisX, diffY) * Matrix::rotateY(diffX);
            Vector3 dir   = (lookAt_ - position_).transform(rot);
            lookAt_       = position_ + dir;   // lookAt�Čv�Z
        }
    }

    // ���s�ړ�
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
//! ���
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