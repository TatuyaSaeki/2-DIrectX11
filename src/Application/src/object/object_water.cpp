//===========================================================================
//! @file	object_water.cpp
//! @brife	水表現用クラス
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Water::Water()
{
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Water::initialize(std::shared_ptr<gpu::Texture> buffer, const std::string& modelPath, f32 scale, PRIMITIVE_TYPE primitiveType)
{
    if(!this->ObjectBaseModel::initialize(modelPath, scale, primitiveType))
        return false;
    if(!model_)
        return false;

    model_->setTexture(TEX_ALBEDO, buffer);

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Water::update()
{
    {
        wave_.moveDistance_ += wave_.speed_;

        auto cbWave = cbWave_.begin();
        {
            cbWave->moveDistance_ = wave_.moveDistance_;
            cbWave->fineness_     = wave_.fineness_;
            cbWave->sharp_        = wave_.sharp_;
            cbWave->height_       = wave_.height_;
        }
        cbWave_.end();
        gpu::setConstantBuffer(gpuSlot_, &cbWave_);
    }

    this->ObjectBaseModel::update();
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Water::render(RenderMode renderMode)
{
    // Water2以外なら描画しない
    if(renderMode != RenderMode::Water2)
        return;

    this->ObjectBaseModel::render();
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void Water::showImGuiWindow()
{
    auto p = this->ObjectBase::getPosition();
    auto r = this->ObjectBase::getRotation();
    auto s = this->ObjectBase::getScale();

    f32 fp[3] = { p.x_, p.y_, p.z_ };
    f32 fr[3] = { r.x_, r.y_, r.z_ };
    f32 fs    = s.x_;

    ImGui::DragFloat3("Position", fp, 0.05f);
    ImGui::DragFloat3("Rotation", fr, 0.30f);
    ImGui::DragFloat("Scale", &fs, 0.01f);

    this->ObjectBase::setPosition({ fp[0], fp[1], fp[2] });
    this->ObjectBase::setRotation({ fr[0], fr[1], fr[2] });
    this->ObjectBase::setScale({ fs, fs, fs });
}