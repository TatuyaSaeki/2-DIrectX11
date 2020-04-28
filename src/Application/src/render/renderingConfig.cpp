#include "renderingConfig.h"
//===========================================================================
//! @file	renderigConfig.cpp
//!	@brief	�`��ݒ�Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! �C���X�^���X�擾
//---------------------------------------------------------------------------
RenderingConfig& RenderingConfig::getInstance()
{
    static RenderingConfig instance;
    return instance;
}

//---------------------------------------------------------------------------
//! ������
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
//! �X�V
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
//! ���
//---------------------------------------------------------------------------
void RenderingConfig::cleanup()
{
}

//---------------------------------------------------------------------------
//! ImGui�`��
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
//! ImGui�`��(���C�g)
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
//! ImGui�`��(�C)
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
//! ImGui�`��(��)
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
//! ImGui�̓��������܂Ƃ߂Ă邾�� abridge=�Z�k
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
//! �g�p������IBL�e�N�X�`���^�C�v�ݒ�
//---------------------------------------------------------------------------
void RenderingConfig::setIBLTextureType(IBLTextureType type)
{
    textureType_    = type;
    tmpTextureType_ = type;
}

//---------------------------------------------------------------------------
//! �X�J�C�{�b�N�X�L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isSkyBox(bool flag)
{
    isSkyBox_ = flag;
}

//---------------------------------------------------------------------------
//! IBL�L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isIBL(bool flag)
{
    isIBL_ = flag;
}

//---------------------------------------------------------------------------
//! �V���h�E�}�b�v�L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isShadowmapping(bool flag)
{
    isShadowmapping_ = flag;
}

//---------------------------------------------------------------------------
//! �����˗L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isReflection(bool flag)
{
    isReflection_ = flag;
}

//---------------------------------------------------------------------------
//! �g�[���}�b�v�L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isTonemapping(bool flag)
{
    isTonemapping_ = flag;
}

//---------------------------------------------------------------------------
//! ���L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isFog(bool flag)
{
    isFog_ = flag;
}

//---------------------------------------------------------------------------
//! ���C���[�t���[���L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isWireFrame(bool flag)
{
    isWireFrame_ = flag;
}

//---------------------------------------------------------------------------
//! �O���A�t�B���^�[�L��,����
//---------------------------------------------------------------------------
void RenderingConfig::isGlare(bool flag)
{
    isGlare_ = flag;
}

//---------------------------------------------------------------------------
//! IBL�e�N�X�`���^�C�v�擾
//---------------------------------------------------------------------------
IBLTextureType RenderingConfig::getIBLTextureType() const
{
    return textureType_;
}

//---------------------------------------------------------------------------
//! �X�J�C�{�b�N�X
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
//! �V���h�E�}�b�v
//---------------------------------------------------------------------------
bool RenderingConfig::isShadowmapping() const
{
    return isShadowmapping_;
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool RenderingConfig::isReflection() const
{
    return isReflection_;
}

//---------------------------------------------------------------------------
//! �g�[���}�b�v
//---------------------------------------------------------------------------
bool RenderingConfig::isToonmapping() const
{
    return isToonmapping_;
}

//---------------------------------------------------------------------------
//! ��
//---------------------------------------------------------------------------
bool RenderingConfig::isFog() const
{
    return isFog_;
}

//---------------------------------------------------------------------------
//! ���C���[�t���[��
//---------------------------------------------------------------------------
bool RenderingConfig::isWireFrame() const
{
    return isWireFrame_;
}

//---------------------------------------------------------------------------
//! �O���A�t�B���^�[
//---------------------------------------------------------------------------
bool RenderingConfig::isGlare() const
{
    return isGlare_;
}
