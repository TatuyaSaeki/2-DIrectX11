//===========================================================================
//! @file	object_floor.cpp
//! @brife	床オブジェクト
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Floor::Floor()
{
    isShowImGui_   = false;
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Floor::update()
{
    ObjectBaseModel::update();
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Floor::render(RenderMode renderMode)
{
    // エッジ, 反射, 霧, なら描画しない
    if(renderMode == RenderMode::Edge ||
       renderMode == RenderMode::Reflection ||
       renderMode == RenderMode::Fog) {
        return;
    }

    switch(renderMode) {
        case RenderMode::Shadow:   // 影
            useShader_.vsName_ = "vsShadowDepth";
            useShader_.psName_ = "nullptr";
            break;

        case RenderMode::Scene:   // シーン描画
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psLight";
            break;

        case RenderMode::Toon:   // トゥーン描画
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psLight";
            break;

        case RenderMode::Fog:   // 霧
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psFog";
            break;

        case RenderMode::Water1:   // 水
        case RenderMode::Water2:   // 水
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psTexture3D";
            break;

        default:
            break;
    }

    // シェーダー設定
    gpu::setShader(useShader_);

    // モデル描画
    model_.get()->render(matWorld_);

    //
    auto d3dContext = device::D3DContext();
    d3dContext->VSSetShader(nullptr, nullptr, 0);
    d3dContext->PSSetShader(nullptr, nullptr, 0);
    d3dContext->GSSetShader(nullptr, nullptr, 0);
    d3dContext->HSSetShader(nullptr, nullptr, 0);
    d3dContext->DSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void Floor::showImGuiWindow()
{
}