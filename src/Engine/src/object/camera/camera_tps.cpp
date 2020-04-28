//===========================================================================
//! @file	camera_tps.cpp
//!	@brief	�O�l�̃J����
//===========================================================================

//---------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------------
CameraTPS::CameraTPS()
{
	cameraType_ = CameraType::Tps;
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool CameraTPS::initialize()
{
    if(!Camera::initialize()) {
        return false;
    }

    length_      = 30.0f;
    sensitivity_ = 0.15f;

    POINT pt;
    GetCursorPos(&pt);
    oldPoint_ = { static_cast<f32>(pt.x), static_cast<f32>(pt.y) };

	rotation_.y_ = 150.0f;

	offset_ = Vector3(5.0f, 1.0f, -10.0f);

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CameraTPS::update()
{
    POINT pt;
    GetCursorPos(&pt);
    const f32 pointX = static_cast<f32>(pt.x);
    const f32 pointY = static_cast<f32>(pt.y);

    // 1�t���[���̍����擾
    f32 diffX = (pointX - oldPoint_.x_) * sensitivity_;
    f32 diffY = (pointY - oldPoint_.y_) * sensitivity_;

    // ���݂̈ʒu��ۑ��i���̃t���[���ŗ��p�j
    oldPoint_.x_ = pointX;
    oldPoint_.y_ = pointY;

    // �ۑ��p
    Vector3 newPosition    = position_;
    Vector3 newLookAt      = lookAt_;
    Vector3 targetPosition = target_->getPosition();

	// ��]�ʕێ�
	rotation_.x_ += diffY;
	rotation_.y_ += diffX;
	rotation_.z_ -= diffY;

	// tmp
	static bool flag = false;

    // �}�E�X�̉E�{�^����������Ă�����
    if((GetKeyState(VK_RBUTTON) & 0x8000)) {
        auto targetRot = target_->getRotation();

        Matrix rot =
            Matrix::rotateX(DEG_TO_RAD(rotation_.x_)) *
            Matrix::rotateY(DEG_TO_RAD(targetRot.y_));

        auto rotPosition = (Matrix::translate(offset_) * rot).translate();

        newPosition = targetPosition + rotPosition;
        newLookAt   = newPosition + rot.axisZ().normalize();

		flag = true;
    }
    else
    {
	if(flag) { 
		flag = false;
		rotation_.y_ =  target_->getRotation().y_ - 180.0f;
	}
	
 


        const Vector3 rot = {
            sinf(DEG_TO_RAD(rotation_.y_)),
            sinf(DEG_TO_RAD(rotation_.x_)),
            cosf(DEG_TO_RAD(rotation_.y_))
        };

        newPosition = targetPosition + rot * length_;
        newLookAt   = targetPosition;
    }

    // ���f
    lookAt_   = newLookAt;
    position_ = newPosition;

    Camera::update();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void CameraTPS::cleanup()
{
	target_ = nullptr;

    Camera::cleanup();
}

//---------------------------------------------------------------------------
//! �����_�ɂ���I�u�W�F�N�g�ݒ�
//---------------------------------------------------------------------------
void CameraTPS::setTarget(ObjectBase* const target)
{
	if (target_) {
		target_.reset();
	}
	
	target_.reset(target);
}