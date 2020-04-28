//===========================================================================
//! @file	camera.cpp
//!	@brief	�J����
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool Camera::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void Camera::update()
{
    Vector3 axisZ = (position_ - lookAt_).normalize();
    //Vector3 axisZ = (lookAt_ - position_).normalize();
    Vector3 axisX = Vector3::cross(worldUp_, axisZ).normalize();
    Vector3 axisY = Vector3::cross(axisZ, axisX);

    // �s��ɔ��f
    matWorld_.m_[0][0] = axisX.x_;
    matWorld_.m_[0][1] = axisX.y_;
    matWorld_.m_[0][2] = axisX.z_;
    matWorld_.m_[0][3] = 0.0f;

    matWorld_.m_[1][0] = axisY.x_;
    matWorld_.m_[1][1] = axisY.y_;
    matWorld_.m_[1][2] = axisY.z_;
    matWorld_.m_[1][3] = 0.0f;

    matWorld_.m_[2][0] = axisZ.x_;
    matWorld_.m_[2][1] = axisZ.y_;
    matWorld_.m_[2][2] = axisZ.z_;
    matWorld_.m_[2][3] = 0.0f;

    matWorld_.m_[3][0] = position_.x_;
    matWorld_.m_[3][1] = position_.y_;
    matWorld_.m_[3][2] = position_.z_;
    matWorld_.m_[3][3] = 1.0f;

    // �s��o�b�t�@�X�V
    dxLoadMatrixf(matWorld_);

    //=============================================================
    // �r���[�s����v�Z
    //=============================================================
    matView_.m_[0][0] = axisX.x_;
    matView_.m_[0][1] = axisY.x_;
    matView_.m_[0][2] = axisZ.x_;
    matView_.m_[0][3] = 0.0f;

    matView_.m_[1][0] = axisX.y_;
    matView_.m_[1][1] = axisY.y_;
    matView_.m_[1][2] = axisZ.y_;
    matView_.m_[1][3] = 0.0f;

    matView_.m_[2][0] = axisX.z_;
    matView_.m_[2][1] = axisY.z_;
    matView_.m_[2][2] = axisZ.z_;
    matView_.m_[2][3] = 0.0f;

    matView_.m_[3][0] = Vector3::dot(position_, axisX) * -1.0f;
    matView_.m_[3][1] = Vector3::dot(position_, axisY) * -1.0f;
    matView_.m_[3][2] = Vector3::dot(position_, axisZ) * -1.0f;
    matView_.m_[3][3] = 1.0f;

    //=============================================================
    // ���e�s��̍쐬
    //=============================================================
    matProj_ = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, aspectRatio_, near_, far_);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void Camera::cleanup()
{
}