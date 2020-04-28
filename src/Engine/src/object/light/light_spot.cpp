//===========================================================================
//! @file	light_spot.h
//! @brife	�X�|�b�g���C�g
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SpotLight::initialize(s32 index)
{
    lightType_ = LightType::SpotLight;

    isActive_ = true;
    index_    = index;

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SpotLight::update()
{
    if(!isActive_) {
        if(tmpIntensity_ == intensity_) {
            intensity_ = 0;
            return;
        }
        f = true;
    }
    else {
        if(f) {
            intensity_ = tmpIntensity_;
            f          = false;
        }
    }

    matWorld_ = Matrix::translate(position_);
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SpotLight::render()
{
    // �s��X�V(�ʒu�ړ��̂�)
    dxLoadMatrixf(matWorld_);

    // ���C�g���̂̕`��
    drawSphere(0.7f, color_, 16 * 3);

    dxLoadMatrixf(Matrix::identity());
    drawLine(position_, (lookAt_ - position_).normalize(), 7.0f);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SpotLight::cleanup()
{
}

//---------------------------------------------------------------------------
//! �����p�����[�^�[�ݒ�
//---------------------------------------------------------------------------
void SpotLight::setAttenuation(const Vector3& attenuation)
{
    attenuation_ = attenuation;
}

//---------------------------------------------------------------------------
//! �����p�����[�^�[�擾
//---------------------------------------------------------------------------
Vector3 SpotLight::getAttenuation() const
{
    return attenuation_;
}

//---------------------------------------------------------------------------
//!	ImGui
//---------------------------------------------------------------------------
void SpotLight::showImGuiWindow()
{
    std::string index = std::to_string(index_);
    std::string s     = "SpotLight[" + index + "]";

    if(ImGui::TreeNode(s.c_str())) {
        ImGui::Checkbox("Active", &isActive_);

        auto p = this->ObjectBase::getPosition();
        auto l = this->Light::getLookAt();
        auto c = this->Light::getColor();
        auto i = this->Light::getIntensity();

        f32 fp[3] = { p.x_, p.y_, p.z_ };
        f32 fl[3] = { l.x_, l.y_, l.z_ };
        f32 fc[3] = { c.r_, c.g_, c.b_ };
        f32 fi    = i;

        ImGui::DragFloat3("Position", fp, 0.05f, -100.0f, 100.0f);
        ImGui::DragFloat3("LookAt", fl, 0.05f, -100.0f, 100.0f);
        ImGui::SliderFloat3("Color", fc, 0.0f, 1.0f);
        ImGui::SliderFloat("Intensity", &fi, 0.0f, 100.0f);

        this->ObjectBase::setPosition({ fp[0], fp[1], fp[2] });
        this->Light::setLookAt({ fl[0], fl[1], fl[2] });
        this->Light::setColor({ fc[0], fc[1], fc[2], 1.0f });
        this->Light::setIntensity(fi);

        if(ImGui::Button("Remove")) {
            this->isRemove(true);
        }

        ImGui::TreePop();
    }
}