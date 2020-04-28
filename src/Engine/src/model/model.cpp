//===========================================================================
//! @file	model.cpp
//!	@brief	
//===========================================================================

//---------------------------------------------------------------------------
//! �ǂݍ���
//---------------------------------------------------------------------------
bool Model::load(const std::string& fileName, [[maybe_unused]] const Vector3& scale, PRIMITIVE_TYPE primitiveType)
{
    // ���f���擾
    assetModel_ = Asset::getModel(fileName);
	if (assetModel_.expired()) {
		return false;
	}

    primitiveType_ = primitiveType;

    return true;
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void Model::render(const Matrix& matWorld)
{
    dxLoadMatrixf(matWorld);

    // ���b�V���P�ʂŕ`��
    for(auto& mesh : assetModel_.lock()->getMeshes()) {
        auto materials = mesh.getMaterials();

		// tmp
        if(materials.size() != 0) {
            gpu::setTexture(TEX_ALBEDO,		materials[0].texture_[(s32)Material::TextureType::Albedo]);
            gpu::setTexture(TEX_ROUGHNESS,	materials[0].texture_[(s32)Material::TextureType::Roughness]);
            gpu::setTexture(TEX_METALNESS,	materials[0].texture_[(s32)Material::TextureType::Metalness]);
            gpu::setTexture(TEX_NORMAL,		materials[0].texture_[(s32)Material::TextureType::Normal]);
            gpu::setTexture(TEX_AO,			materials[0].texture_[(s32)Material::TextureType::Ao]);
        }
        else {
            // �O������̃e�N�X�`��
            if(!useTexture_.empty()) {
                for(auto& texture : useTexture_) {
                    gpu::setTexture(texture.gpuSlot_, texture.texture_);
                }
            }
            // �V�X�e���e�N�X�`��
            else {
                gpu::setTexture(TEX_ALBEDO,		Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_WHITE));
                gpu::setTexture(TEX_ROUGHNESS,	Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_WHITE));
                gpu::setTexture(TEX_METALNESS,	Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_WHITE));
                gpu::setTexture(TEX_NORMAL,		Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_NORMAL));
                gpu::setTexture(TEX_AO,			Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_NULL_WHITE));
                //gpu::setTexture(4, materials[0].texture_[(s32)Material::TextureType::Ao]);
            }
        }
        gpu::setVertexBuffer(mesh.getVertexBuffer(), sizeof(Vertex));
        gpu::setIndexBuffer(mesh.getIndexBuffer());
        gpu::drawIndexed(primitiveType_, u32(mesh.getIndices().size()));
    }

    gpu::setTexture(TEX_ALBEDO, nullptr);
    gpu::setTexture(TEX_ROUGHNESS, nullptr);
    gpu::setTexture(TEX_METALNESS, nullptr);
    gpu::setTexture(TEX_NORMAL, nullptr);
    gpu::setTexture(TEX_AO, nullptr);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void Model::cleanup()
{
    if(!assetModel_.expired()) {
        assetModel_.lock()->cleanup();
    }
    assetModel_.reset();
}

//---------------------------------------------------------------------------
//! �e�N�X�`���ݒ�
//---------------------------------------------------------------------------
bool Model::setTexture(s32 slot, std::shared_ptr<gpu::Texture> texture)
{
    if(!texture)
        return false;

    bool flag = false;

    for(auto& t : useTexture_) {
        if(t.gpuSlot_ == slot) {
            t.texture_ = texture;
            t.gpuSlot_ = slot;
            flag       = true;
            break;
        }
    }

    if(!flag) {
        useTexture_.emplace_back(UseTexture(texture, slot));
    }

    return true;
}

