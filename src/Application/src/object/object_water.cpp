//===========================================================================
//! @file	object_water.cpp
//! @brife	���\���p�N���X
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Water::Water()
{
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! ������
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
//! �X�V
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
//! �`��
//---------------------------------------------------------------------------
void Water::render(RenderMode renderMode)
{
    // Water2�ȊO�Ȃ�`�悵�Ȃ�
    if(renderMode != RenderMode::Water2)
        return;

    this->ObjectBaseModel::render();
}

//---------------------------------------------------------------------------
//! ImGui�`��
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