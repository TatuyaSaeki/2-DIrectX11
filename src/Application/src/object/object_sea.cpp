//===========================================================================
//! @file	object_sea.cpp
//! @brife	海オブジェクト
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Sea::Sea()
{
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! 更新
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
//! 描画
//---------------------------------------------------------------------------
void Sea::render(RenderMode renderMode)
{
    // シーン描画以外なら描画しない
    if(renderMode != RenderMode::Scene)
        return;

    this->ObjectBaseModel::render();
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void Sea::showImGuiWindow()
{
    ImGui::SetNextWindowSize({ 500, 200 }, ImGuiCond_Once);
    ImGui::Begin("Wave");
    {
        ImGui::SliderFloat(u8"WaveSpeed   (速度)", &wave_.speed_, 0.001f, 0.1f);
        ImGui::SliderFloat(u8"WaveFineness(細かさ)", &wave_.fineness_, 0.01f, 1.0f);
        ImGui::SliderFloat(u8"WaveSharp   (とがり度)", &wave_.sharp_, 0.01f, 1.0f);
        ImGui::SliderFloat(u8"WaveHeight  (高さ)", &wave_.height_, 0.01f, 1.0f);
    }
    if(ImGui::Button("Reset")) {
        // 波の数値初期化
        initializeWave();
    }

    ImGui::End();
}

//---------------------------------------------------------------------------
//! 波の数値初期化
//---------------------------------------------------------------------------
void Sea::initializeWave()
{
    wave_ = Wave();
}