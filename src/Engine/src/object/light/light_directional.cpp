//===========================================================================
//! @file	light_directional.cpp
//! @brife	平行光源
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool DirectionalLight::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void DirectionalLight::update()
{
    // ワールド行列更新
    matWorld_ = Matrix::translate(position_);
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void DirectionalLight::render()
{
    // 行列更新(位置移動のみ)
    dxLoadMatrixf(matWorld_);

    // ライト自体の描画
    drawSphere(1.0f, color_, 16 * 8);

    dxLoadMatrixf(Matrix::identity());
    drawLine(position_, (lookAt_ - position_).normalize(), 7.0f);
}

//---------------------------------------------------------------------------
//! 解放
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