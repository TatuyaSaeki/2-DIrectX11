//===========================================================================
//! @file	light_directional.cpp
//! @brife	���s����
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool DirectionalLight::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void DirectionalLight::update()
{
    // ���[���h�s��X�V
    matWorld_ = Matrix::translate(position_);
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void DirectionalLight::render()
{
    // �s��X�V(�ʒu�ړ��̂�)
    dxLoadMatrixf(matWorld_);

    // ���C�g���̂̕`��
    drawSphere(1.0f, color_, 16 * 8);

    dxLoadMatrixf(Matrix::identity());
    drawLine(position_, (lookAt_ - position_).normalize(), 7.0f);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void DirectionalLight::cleanup()
{
    //Light::cleanup();
}

//---------------------------------------------------------------------------
//!	ImGui
//---------------------------------------------------------------------------
void DirectionalLight::showImGuiWindow()
{
    auto p = this->ObjectBase::getPosition();
    auto l = this->Light::getLookAt();
    auto c = this->Light::getColor();
    auto i = this->Light::getIntensity();

    f32 fp[3] = { p.x_, p.y_, p.z_ };
    f32 fl[3] = { l.x_, l.y_, l.z_ };
    f32 fc[3] = { c.r_, c.g_, c.b_ };
    f32 fi    = i;

    ImGui::DragFloat3("Position", fp, 0.2f, -30.0f, +30.0f);
    //ImGui::DragFloat3("LookAt", fl, 1.0f);
    ImGui::SliderFloat3("Color", fc, 0.0f, 1.0f);
    ImGui::SliderFloat("Intensity", &fi, 0.0f, 100.0f);

    this->ObjectBase::setPosition({ fp[0], fp[1], fp[2] });
    this->Light::setLookAt({ fl[0], fl[1], fl[2] });
    this->Light::setColor({ fc[0], fc[1], fc[2], 1.0f });
    this->Light::setIntensity(fi);
}