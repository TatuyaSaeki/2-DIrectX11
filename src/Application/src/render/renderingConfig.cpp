#include "renderingConfig.h"
//===========================================================================
//! @file	renderigConfig.cpp
//!	@brief	描画設定管理
//===========================================================================

//---------------------------------------------------------------------------
//! インスタンス取得
//---------------------------------------------------------------------------
RenderingConfig& RenderingConfig::getInstance()
{
    static RenderingConfig instance;
    return instance;
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool RenderingConfig::initialize()
{
    textureType_    = IBLTextureType::Forest;
    tmpTextureType_ = IBLTextureType::Forest;

    isSkyBox_        = true;
    isIBL_           = true;
    isTonemapping_   = true;
    isShadowmapping_ = true;

    isGlare_       = false;
    isReflection_  = false;
    isToonmapping_ = false;
    isFog_         = false;
    isWireFrame_   = false;

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void RenderingConfig::update()
{
    // Tonemap
    (isTonemapping_) ? setFilter(FilterType::ToneMapping) : removeFilter(FilterType::ToneMapping);

    // Glare
    (isGlare_) ? setFilter(FilterType::GaussianBlur) : removeFilter(FilterType::GaussianBlur);

    if(!isFog_)
        textureType_ = tmpTextureType_;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void RenderingConfig::cleanup()
{
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void RenderingConfig::showImGuiWindow(DemoSceneType sceneType)
{
    switch(sceneType) {
        case DemoSceneType::Light:
            this->showImGuiLight();
            break;

        case DemoSceneType::Sea:
            this->showImGuiSea();
            break;

        case DemoSceneType::Water:
            this->showImGuiWater();
            break;
    }
}

//---------------------------------------------------------------------------
//! ImGui描画(ライト)
//---------------------------------------------------------------------------
void RenderingConfig::showImGuiLight()
{
    ImGui::Begin("RenderingConfig");
    {
        //
        if(ImGui::TreeNode("RenderingConfig")) {
            // Tonemap
            ImGui::Checkbox("Tonemap", &isTonemapping_);

            // Shadowmap
            ImGui::Checkbox("Shadowmap", &isShadowmapping_);

            // Glare
            if(ImGui::Checkbox("Glare", &isGlare_)) {
                if(isGlare_) {
                    isWireFrame_ = false;
                    isFog_       = false;
                }
            }
            // Reflection
            if(ImGui::Checkbox("Reflection", &isReflection_)) {
                isToonmapping_ = false;
                isFog_         = false;
            }
            // Toon
            if(ImGui::Checkbox("ToonShader", &isToonmapping_)) {
                isReflection_ = false;
                isFog_        = false;
                isWireFrame_  = false;
            }
            // Fog
            if(ImGui::Checkbox("Fog", &isFog_)) {
                isToonmapping_ = false;
                isReflection_  = false;
                if(isFog_) {
                    tmpTextureType_ = textureType_;
                }
                textureType_ = IBLTextureType::NullBlack;
            }
            // WireFrame
            if(ImGui::Checkbox("WireFrame", &isWireFrame_)) {
                if(isWireFrame_) {
                    isToonmapping_ = false;
                    isGlare_       = false;
                }
            }

            ImGuiAbridgeIBL();

            ImGui::TreePop();
        }
    }   // END

    if(!isIBL_)
        textureType_ = IBLTextureType::NullBlack;
    else
        textureType_ = tmpTextureType_;

    ImGui::End();
}

//---------------------------------------------------------------------------
//! ImGui描画(海)
//---------------------------------------------------------------------------
void RenderingConfig::showImGuiSea()
{
    ImGui::Begin("RenderingConfig");
    {
        //
        if(ImGui::TreeNode("RenderingConfig")) {
            // Tonemap
            ImGui::Checkbox("Tonemap", &isTonemapping_);

            // WireFrame
            if(ImGui::Checkbox("WireFrame", &isWireFrame_)) {
                if(isWireFrame_) {
                    isToonmapping_ = false;
                }
            }

            ImGuiAbridgeIBL();

            ImGui::TreePop();
        }
    }   // END

    if(!isIBL_)
        textureType_ = IBLTextureType::NullBlack;
    else
        textureType_ = tmpTextureType_;

    ImGui::End();
}

//---------------------------------------------------------------------------
//! ImGui描画(水)
//---------------------------------------------------------------------------
void RenderingConfig::showImGuiWater()
{
    ImGui::Begin("RenderingConfig");
    {
        //
        if(ImGui::TreeNode("RenderingConfig")) {
            // Tone
            ImGui::Checkbox("Tonemap", &isTonemapping_);

            // Glare
            if(ImGui::Checkbox("Glare", &isGlare_)) {
                if(isGlare_) {
                    isWireFrame_ = false;
                }
            }

            // WireFrame
            if(ImGui::Checkbox("WireFrame", &isWireFrame_)) {
                if(isWireFrame_) {
                    isToonmapping_ = false;
                    isGlare_       = false;
                }
            }

            ImGuiAbridgeIBL();

            ImGui::TreePop();
        }
    }   // END

    if(!isIBL_)
        textureType_ = IBLTextureType::NullBlack;
    else
        textureType_ = tmpTextureType_;

    ImGui::End();
}

//---------------------------------------------------------------------------
//! ImGuiの同じ処理まとめてるだけ abridge=短縮
//---------------------------------------------------------------------------
void RenderingConfig::ImGuiAbridgeIBL()
{
    // IBL
    if(ImGui::TreeNode("IBL")) {
        ImGui::Checkbox("IBL", &isIBL_);
        if(ImGui::Button("Wilderness")) {
            textureType_    = IBLTextureType::Wilderness;
            tmpTextureType_ = textureType_;
            isFog_          = false;
            isIBL_          = true;
        }
        if(ImGui::Button("Forest")) {
            textureType_    = IBLTextureType::Forest;
            tmpTextureType_ = textureType_;
            isFog_          = false;
            isIBL_          = true;
        }
        if(ImGui::Button("RuinCastle")) {
            textureType_    = IBLTextureType::RuinCastle;
            tmpTextureType_ = textureType_;
            isFog_          = false;
            isIBL_          = true;
        }
        if(ImGui::Button("BlueSky")) {
            textureType_    = IBLTextureType::BlueSky;
            tmpTextureType_ = textureType_;
            isFog_          = false;
            isIBL_          = true;
        }
        ImGui::TreePop();
    }
}

//---------------------------------------------------------------------------
//! 使用したいIBLテクスチャタイプ設定
//---------------------------------------------------------------------------
void RenderingConfig::setIBLTextureType(IBLTextureType type)
{
    textureType_    = type;
    tmpTextureType_ = type;
}

//---------------------------------------------------------------------------
//! スカイボックス有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isSkyBox(bool flag)
{
    isSkyBox_ = flag;
}

//---------------------------------------------------------------------------
//! IBL有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isIBL(bool flag)
{
    isIBL_ = flag;
}

//---------------------------------------------------------------------------
//! シャドウマップ有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isShadowmapping(bool flag)
{
    isShadowmapping_ = flag;
}

//---------------------------------------------------------------------------
//! 環境反射有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isReflection(bool flag)
{
    isReflection_ = flag;
}

//---------------------------------------------------------------------------
//! トーンマップ有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isTonemapping(bool flag)
{
    isTonemapping_ = flag;
}

//---------------------------------------------------------------------------
//! 霧有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isFog(bool flag)
{
    isFog_ = flag;
}

//---------------------------------------------------------------------------
//! ワイヤーフレーム有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isWireFrame(bool flag)
{
    isWireFrame_ = flag;
}

//---------------------------------------------------------------------------
//! グレアフィルター有効,無効
//---------------------------------------------------------------------------
void RenderingConfig::isGlare(bool flag)
{
    isGlare_ = flag;
}

//---------------------------------------------------------------------------
//! IBLテクスチャタイプ取得
//---------------------------------------------------------------------------
IBLTextureType RenderingConfig::getIBLTextureType() const
{
    return textureType_;
}

//---------------------------------------------------------------------------
//! スカイボックス
//---------------------------------------------------------------------------
bool RenderingConfig::isSkyBox() const
{
    return isSkyBox_;
}

//---------------------------------------------------------------------------
//!< IBL
//---------------------------------------------------------------------------
bool RenderingConfig::isIBL() const
{
    return isIBL_;
}

//---------------------------------------------------------------------------
//! シャドウマップ
//---------------------------------------------------------------------------
bool RenderingConfig::isShadowmapping() const
{
    return isShadowmapping_;
}

//---------------------------------------------------------------------------
//! 環境反射
//---------------------------------------------------------------------------
bool RenderingConfig::isReflection() const
{
    return isReflection_;
}

//---------------------------------------------------------------------------
//! トーンマップ
//---------------------------------------------------------------------------
bool RenderingConfig::isToonmapping() const
{
    return isToonmapping_;
}

//---------------------------------------------------------------------------
//! 霧
//---------------------------------------------------------------------------
bool RenderingConfig::isFog() const
{
    return isFog_;
}

//---------------------------------------------------------------------------
//! ワイヤーフレーム
//---------------------------------------------------------------------------
bool RenderingConfig::isWireFrame() const
{
    return isWireFrame_;
}

//---------------------------------------------------------------------------
//! グレアフィルター
//---------------------------------------------------------------------------
bool RenderingConfig::isGlare() const
{
    return isGlare_;
}
