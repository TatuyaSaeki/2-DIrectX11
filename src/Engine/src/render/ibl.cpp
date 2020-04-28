//===========================================================================
//! @file	ibl.cpp
//!	@brief	IBL�Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool IBL::initialize(s32 resolution)
{
    // �𑜓x
    resolution_ = resolution;

    // �e�N�X�`���Z�b�g
    //gpu::setTexture(gpuSlotDiffuse_, iblDiffuseTexture_);
    //gpu::setTexture(gpuSlotSpecular_, iblSpecularTexture_);

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void IBL::cleanup()
{
    iblSpecularTexture_.reset();
    iblDiffuseTexture_.reset();
}

//---------------------------------------------------------------------------
//! �J�n
//---------------------------------------------------------------------------
void IBL::begin()
{
    // �e�N�X�`������ւ�
    if(currentTextureType_ != nextTextureType_) {
        this->chengeTexture(nextTextureType_);
        currentTextureType_ = nextTextureType_;
    }

    // NullBlack�Ȃ�
    if(currentTextureType_ == IBLTextureType::NullBlack) {
        gpu::setTexture(TEX_IBL_DIFFUSE, Asset::getSystemTexture(SYSTEM_TEXTURE_CUBEMAP_NULL_BLACK));
        gpu::setTexture(TEX_IBL_SPECULAR, Asset::getSystemTexture(SYSTEM_TEXTURE_CUBEMAP_NULL_BLACK));
    }
    else {
        gpu::setTexture(TEX_IBL_DIFFUSE, iblDiffuseTexture_.lock());
        gpu::setTexture(TEX_IBL_SPECULAR, iblSpecularTexture_.lock());
    }
}

//---------------------------------------------------------------------------
//! �I��
//---------------------------------------------------------------------------
void IBL::end()
{
}

//---------------------------------------------------------------------------
//! ImGui
//---------------------------------------------------------------------------
void IBL::showImGuiWindow()
{
}

//---------------------------------------------------------------------------
//! IBL�e�N�X�`���^�C�v�ݒ�
//---------------------------------------------------------------------------
void IBL::setTextureType(IBLTextureType type)
{
    nextTextureType_ = type;
}

//---------------------------------------------------------------------------
//! �e�N�X�`������ւ�
//---------------------------------------------------------------------------
bool IBL::chengeTexture(IBLTextureType type)
{
    //std::shared_ptr<gpu::Texture> tmpDiffuseTexture;
    //std::shared_ptr<gpu::Texture> tmpSpecularTexture;

    // �e�N�X�`���쐬
    switch(type) {
        case IBLTextureType::Wilderness: {
            iblDiffuseTexture_  = Asset::getTexture("texture/dds/Wilderness/WildernessDiffuseHDR.dds");
            iblSpecularTexture_ = Asset::getTexture("texture/dds/Wilderness/WildernessSpecularHDR.dds");

            break;
        }
        case IBLTextureType::Forest: {
            iblDiffuseTexture_  = Asset::getTexture("texture/dds/Forest/ForestDiffuseHDR.dds");
            iblSpecularTexture_ = Asset::getTexture("texture/dds/Forest/ForestSpecularHDR.dds");

            break;
        }
        case IBLTextureType::RuinCastle: {
            iblDiffuseTexture_  = Asset::getTexture("texture/dds/RuinCastle/RuinCastleDiffuse.dds");
            iblSpecularTexture_ = Asset::getTexture("texture/dds/RuinCastle/RuinCastleSpecular.dds");
            break;
        }
		case IBLTextureType::BlueSky: {
			iblDiffuseTexture_  = Asset::getTexture("texture/dds/BlueSky/BlueSkyDiffuseHDR.dds");
			iblSpecularTexture_ = Asset::getTexture("texture/dds/BlueSky/BlueSkySpecularHDR.dds");
			break;
		}
        default:
            break;
    }

    return true;
}