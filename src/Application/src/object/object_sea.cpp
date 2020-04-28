//===========================================================================
//! @file	object_sea.cpp
//! @brife	�C�I�u�W�F�N�g
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Sea::Sea()
{
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void Sea::update()
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

    ObjectBaseModel::update();
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void Sea::render(RenderMode renderMode)
{
    // �V�[���`��ȊO�Ȃ�`�悵�Ȃ�
    if(renderMode != RenderMode::Scene)
        return;

    this->ObjectBaseModel::render();
}

//---------------------------------------------------------------------------
//! ImGui�`��
//---------------------------------------------------------------------------
void Sea::showImGuiWindow()
{
    ImGui::SetNextWindowSize({ 500, 200 }, ImGuiCond_Once);
    ImGui::Begin("Wave");
    {
        ImGui::SliderFloat(u8"WaveSpeed   (���x)", &wave_.speed_, 0.001f, 0.1f);
        ImGui::SliderFloat(u8"WaveFineness(�ׂ���)", &wave_.fineness_, 0.01f, 1.0f);
        ImGui::SliderFloat(u8"WaveSharp   (�Ƃ���x)", &wave_.sharp_, 0.01f, 1.0f);
        ImGui::SliderFloat(u8"WaveHeight  (����)", &wave_.height_, 0.01f, 1.0f);
    }
    if(ImGui::Button("Reset")) {
        // �g�̐��l������
        initializeWave();
    }

    ImGui::End();
}

//---------------------------------------------------------------------------
//! �g�̐��l������
//---------------------------------------------------------------------------
void Sea::initializeWave()
{
    wave_ = Wave();
}