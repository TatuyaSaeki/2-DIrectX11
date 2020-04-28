//===========================================================================
//! @file	object_skybox.cpp
//! @brife	スカイボックス
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Skybox::Skybox()
{
    isShowImGui_   = false;
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Skybox::update()
{
    auto nextTextureType = RenderConfig().getIBLTextureType();
    if(currentTextureType_ != nextTextureType) {
        chengeTexture(nextTextureType);
        currentTextureType_ = nextTextureType;
    }

    if(RenderConfig().isSkyBox()) {
        this->ObjectBaseModel::update();
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Skybox::render(RenderMode renderMode)
{
    if(!RenderConfig().isSkyBox())
        return;

    switch(renderMode) {
        case RenderMode::Scene:
        case RenderMode::Reflection:
        case RenderMode::Toon:
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psCubeMap";
            break;

        case RenderMode::Fog:
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psFog";
            break;

        default:
            return;
    }

    this->ObjectBaseModel::render();
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void Skybox::showImGuiWindow()
{
    auto s  = this->getScale();
    f32  fs = s.x_;

    ImGui::DragFloat("Scale", &fs, 0.01f);
    this->ObjectBaseModel::setScale({ fs, fs, fs });
}

//---------------------------------------------------------------------------
//! テクスチャ入れ替え
//---------------------------------------------------------------------------
bool Skybox::chengeTexture(IBLTextureType type)
{
    // テクスチャ作成
    switch(type) {
        case IBLTextureType::Wilderness: {
            model_->setTexture(TEX_CUBE, Asset::getTexture("texture/dds/Wilderness/WildernessCubeHDR.dds", true));
            break;
        }
        case IBLTextureType::Forest: {
            model_->setTexture(TEX_CUBE, Asset::getTexture("texture/dds/Forest/ForestCubeHDR.dds", true));
            break;
        }
        case IBLTextureType::RuinCastle: {
            model_->setTexture(TEX_CUBE, Asset::getTexture("texture/dds/RuinCastle/RuinCastleCube.dds", true));
            break;
        }
		case IBLTextureType::BlueSky: {
			model_->setTexture(TEX_CUBE, Asset::getTexture("texture/dds/BlueSky/BlueSkyEnvHDR.dds", true));
			break;
		}
        case IBLTextureType::NullBlack: {
            model_->setTexture(TEX_CUBE, Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_WHITE));
            break;
        }
        default:
            break;
    }

    return true;
}
