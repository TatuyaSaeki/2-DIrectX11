//===========================================================================
//! @file	light_point.cpp
//! @brife	点光源
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool PointLight::initialize(s32 index)
{
    lightType_ = LightType::PointLight;

    isActive_ = true;
    index_    = index;

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void PointLight::update()
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
//! 描画
//---------------------------------------------------------------------------
void PointLight::render()
{
    if(!isActive_)
        return;

    // 行列更新(位置移動のみ)
    dxLoadMatrixf(matWorld_);

    // ライト自体の描画
    drawSphere(0.2f, color_, 16 * 3);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void PointLight::cleanup()
{
}

//---------------------------------------------------------------------------
//! 減衰パラメーター設定
//---------------------------------------------------------------------------
void PointLight::setAttenuation(const Vector3& attenuation)
{
    attenuation_ = attenuation;
}

//---------------------------------------------------------------------------
//! 減衰パラメーター取得
//---------------------------------------------------------------------------
Vector3 PointLight::getAttenuation() const
{
    return attenuation_;
}

//---------------------------------------------------------------------------
//!	ImGui
//---------------------------------------------------------------------------
void PointLight::showImGuiWindow()
{
    std::string index = std::to_string(index_);
    std::string s     = "PointLight [" + index + "]";

    if(ImGui::TreeNode(s.c_str())) {
        ImGui::Checkbox("Active", &isActive_);

        auto p = this->ObjectBase::getPosition();
        auto c = this->Light::getColor();
        auto i = this->Light::getIntensity();

        f32 fp[3] = { p.x_, p.y_, p.z_ };
        f32 fc[3] = { c.r_, c.g_, c.b_ };
        f32 fi    = i;

		ImGui::DragFloat3("Position", fp, 0.05f, -100.0f, 100.0f);
        ImGui::SliderFloat3("Color", fc, 0.0f, 1.0f);
        ImGui::SliderFloat("Intensity", &fi, 0.0f, 100.0f);

        this->ObjectBase::setPosition({ fp[0], fp[1], fp[2] });
        this->Light::setColor({ fc[0], fc[1], fc[2], 1.0f });
        this->Light::setIntensity(fi);

		if (ImGui::Button("Remove")) {
			this->Light::isRemove(true);
		}

        ImGui::TreePop();
    }
}