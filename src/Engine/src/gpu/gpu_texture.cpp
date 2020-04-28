//===========================================================================
//!	@file	gpu_texture.cpp
//!	@brief	�e�N�X�`��
//===========================================================================

//---- DirectXTex
#include <DirectXTex/DirectXTex/DirectXTex.h>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Release/DirectXTex.lib")
#endif

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! ������(D3DResource�w��)
//---------------------------------------------------------------------------
bool Texture::initialize(ID3D11Resource* d3dResource, bool cubemap)
{
    d3dResource_.Attach(d3dResource);

    // ��ޕ������ă_�E���L���X�g
    D3D11_RESOURCE_DIMENSION type;
    d3dResource->GetType(&type);
    switch(type) {
        case D3D11_RESOURCE_DIMENSION_UNKNOWN:
            return false;
        case D3D11_RESOURCE_DIMENSION_BUFFER:
            return false;
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:   // 1D�e�N�X�`��
        {
            auto                 d3dTexture = static_cast<ID3D11Texture1D*>(d3dResource);
            D3D11_TEXTURE1D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = 1;
            depth_     = 1;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;
            bindFlags_ = desc.BindFlags;
            break;
        }
        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:   // 2D�e�N�X�`��
        {
            auto                 d3dTexture = static_cast<ID3D11Texture2D*>(d3dResource);
            D3D11_TEXTURE2D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = desc.Height;
            depth_     = 1;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;   // (desc.Format == DXGI_FORMAT_R32_TYPELESS) ? DXGI_FORMAT_D32_FLOAT : desc.Format;
            bindFlags_ = desc.BindFlags;
            break;
        }
        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:   // 3D�e�N�X�`��
        {
            auto                 d3dTexture = static_cast<ID3D11Texture3D*>(d3dResource);
            D3D11_TEXTURE3D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = desc.Height;
            depth_     = desc.Depth;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;
            bindFlags_ = desc.BindFlags;
            break;
        }
    }

    auto d3dDevice = device::D3DDevice();

    // �r���[����������
    // �V�F�[�_�[���\�[�X�r���[
    if(bindFlags_ & D3D11_BIND_SHADER_RESOURCE) {
        D3D11_SHADER_RESOURCE_VIEW_DESC  v{};
        D3D11_SHADER_RESOURCE_VIEW_DESC* viewDesc = nullptr;

        auto format = format_;

        if(format == DXGI_FORMAT_R32_TYPELESS) {
            format = DXGI_FORMAT_R32_FLOAT;

            v.Format                    = format;
            v.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
            v.Texture2D.MostDetailedMip = 0;
            v.Texture2D.MipLevels       = mipLevels_;

            viewDesc = &v;
        }
        if(cubemap) {
            v.Format                    = format;
            v.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURECUBE;
            v.Texture2D.MostDetailedMip = 0;
            v.Texture2D.MipLevels       = mipLevels_;

            viewDesc = &v;
        }

        if(FAILED(d3dDevice->CreateShaderResourceView(d3dResource, viewDesc, &d3dSrv_))) {
            return false;
        }
    }
    // �����_�[�^�[�Q�b�g
    if(bindFlags_ & D3D11_BIND_RENDER_TARGET) {
        if(FAILED(d3dDevice->CreateRenderTargetView(d3dResource, nullptr, &d3dRtv_))) {
            return false;
        }
    }
    //�@�f�v�X�X�e���V��
    if(bindFlags_ & D3D11_BIND_DEPTH_STENCIL) {
        D3D11_DEPTH_STENCIL_VIEW_DESC  v{};
        D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc = nullptr;

        auto format = format_;

        if(format == DXGI_FORMAT_R32_TYPELESS) {
            format = DXGI_FORMAT_D32_FLOAT;

            v.Format             = format;
            v.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
            v.Texture2D.MipSlice = 0;

            viewDesc = &v;
        }

        if(FAILED(d3dDevice->CreateDepthStencilView(d3dResource, viewDesc, &d3dDsv_))) {
            return false;
        }
    }

    // ����I�[�_�[
    if(bindFlags_ & D3D11_BIND_UNORDERED_ACCESS) {
        if(FAILED(d3dDevice->CreateUnorderedAccessView(d3dResource, nullptr, &d3dUav_))) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! �e�N�X�`���쐬�i�t�@�C�����w��j
//---------------------------------------------------------------------------
gpu::Texture* createTexture(const std::string& fileName, bool cubemap)
{
	// �p�X�ǉ�
	const std::string addFilePath = "../../Application/asset/";
	std::string       filePath = fileName;

	if (fileName.find_first_of(".")) {
		filePath = addFilePath + fileName;
	}

	HRESULT         hr;
	std::vector<u8> textureDDS;

    //================================================================
    // DDS�t�@�C������������ɓǂݍ���
    //================================================================
    FILE* fp;
    if(fopen_s(&fp, filePath.c_str(), "rb") != 0) {
        MessageBox(NULL, (filePath + "\n���̃t�@�C�����Ŏ��s���o���Ȃ��B").c_str(), "�G���[", MB_OK);
        return nullptr;
    }

    //---- �t�@�C���T�C�Y���擾
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // �t�@�C���T�C�Y
    fseek(fp, 0, SEEK_SET);

    //---- ���[�N�̈���m�ۂ��ēǂݍ���
    textureDDS.resize(fileSize);   // ���[�N�̈�̊m��
    if(textureDDS.empty()) {
        fclose(fp);
        return false;
    }
    fread(&textureDDS[0], fileSize, 1, fp);   // �ǂݍ���
    fclose(fp);

    //================================================================
    // �e�N�X�`�����쐬
    //================================================================
    u32  cpuAccessFlags = 0;
    u32  bindFlags      = D3D11_BIND_SHADER_RESOURCE;
    u32  miscFlags      = 0;
    bool forceSRGB      = false;

    // �o�͌��ʂ��󂯎��ϐ�
    com_ptr<ID3D11Resource> d3dResource;
    DirectX::DDS_ALPHA_MODE ddsAlphaMode;

    //---- �e�N�X�`������
    hr = DirectX::CreateDDSTextureFromMemoryEx(
        device::D3DDevice(),     // [in]	D3D�f�o�C�X
        textureDDS.data(),       // [in]	DDS�t�@�C���̉摜�f�[�^�̐擪�A�h���X
        textureDDS.size(),       // [in]	DDS�t�@�C���̃t�@�C���T�C�Y
        0,                       // [in]	��������e�N�X�`���̍ő�T�C�Y(0�w��Ŏ����ݒ�)
        D3D11_USAGE_IMMUTABLE,   // [in]	�p�r
        bindFlags,               // [in]	�֘A�t�������Ώ�
        cpuAccessFlags,          // [in]	CPU�A�N�Z�X���t���O
        miscFlags,               // [in]	���̑��I�v�V�����t���O
        forceSRGB,               // [in]	sRGB���[�h�ɐݒ� true:�����ݒ� false:�ݒ肵�Ȃ�
        &d3dResource,            // [out]	�󂯎��ID3D11Texture2D�|�C���^
        nullptr,                 // [out]	�󂯎��ID3D11ShaderResourceView�|�C���^
        &ddsAlphaMode);          // [out]	�ǂݍ��܂ꂽDDS�̃A���t�@���[�h

    if(hr != S_OK) {
        u32 loadFlags = forceSRGB ? DirectX::WIC_LOADER_FORCE_SRGB : DirectX::WIC_LOADER_IGNORE_SRGB;

        hr = DirectX::CreateWICTextureFromMemoryEx(
            device::D3DDevice(),     // [in]	D3D�f�o�C�X
            textureDDS.data(),       // [in]	DDS�t�@�C���̉摜�f�[�^�̐擪�A�h���X
            textureDDS.size(),       // [in]	DDS�t�@�C���̃t�@�C���T�C�Y
            0,                       // [in]	��������e�N�X�`���̍ő�T�C�Y(0�w��Ŏ����ݒ�)
            D3D11_USAGE_IMMUTABLE,   // [in]	�p�r
            bindFlags,               // [in]	�֘A�t�������Ώ�
            cpuAccessFlags,          // [in]	CPU�A�N�Z�X���t���O
            miscFlags,               // [in]	���̑��I�v�V�����t���O
            loadFlags,               // [in]	sRGB���[�h�ɐݒ� true:�����ݒ� false:�ݒ肵�Ȃ�
            &d3dResource,            // [out]	�󂯎��ID3D11Texture2D�|�C���^
            nullptr);                // [out]	�󂯎��ID3D11ShaderResourceView�|�C���^
        if(hr != S_OK) {
            // TGA�t�@�C���̓ǂݍ���
            DirectX::ScratchImage scratchImage;
            DirectX::TexMetadata  metaData;

            if(DirectX::LoadFromTGAMemory(
                   textureDDS.data(),
                   textureDDS.size(),
                   &metaData,
                   scratchImage) != S_OK) {
                return nullptr;
            }

            if(DirectX::CreateTexture(
                   device::D3DDevice(),
                   scratchImage.GetImages(), scratchImage.GetImageCount(),
                   metaData,
                   &d3dResource) != S_OK) {
                return nullptr;
            }
        }
    }

    //================================================================
    // �e�N�X�`���N���X�̏�����
    //================================================================
    return gpu::createTexture(d3dResource.Detach(), cubemap);
}

//---------------------------------------------------------------------------
//! �e�N�X�`�����쐬(ID3D11Resource�w��)
//---------------------------------------------------------------------------
gpu::Texture* createTexture(ID3D11Resource* d3dResource, bool cubemap)
{
    std::unique_ptr<gpu::Texture> p(new gpu::Texture);
    if(!p) {
        return nullptr;
    }

    if(!p->initialize(d3dResource, cubemap)) {
        return nullptr;
    }
    return p.release();   // unique_ptr�̏��L��������
}

//---------------------------------------------------------------------------
//! �`��^�[�Q�b�g�쐬
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	format	�s�N�Z���`��
//---------------------------------------------------------------------------
gpu::Texture* createRenderTarget(u32 width, u32 height, DXGI_FORMAT format, bool cubemap)
{
    //================================================================
    // �ݒ�
    //================================================================
    D3D11_TEXTURE2D_DESC desc;

    u32 bindFlags = D3D11_BIND_SHADER_RESOURCE;

    if(format == DXGI_FORMAT_D32_FLOAT) {
        bindFlags |= D3D11_BIND_DEPTH_STENCIL;
    }
    else {
		bindFlags |= D3D11_BIND_RENDER_TARGET;
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;	// UAV�Ή�
    }

    u32 arraySize = 1;
    u32 miscFlags = 0;
    if(cubemap) {
        arraySize = 6;
        miscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
    }

    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = 1;
    desc.ArraySize          = arraySize;
    desc.Format             = (format == DXGI_FORMAT_D32_FLOAT) ? DXGI_FORMAT_R32_TYPELESS : format;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage              = D3D11_USAGE_DEFAULT;
    desc.BindFlags          = bindFlags;
    desc.CPUAccessFlags     = 0;
    desc.MiscFlags          = miscFlags;

    //=============================================================
    // �쐬
    //=============================================================
    com_ptr<ID3D11Texture2D> d3dTexture2D;
    if(device::D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D) != S_OK) {
        return nullptr;
    }

    return gpu::createTexture(d3dTexture2D.Detach(), cubemap);
}

}   // namespace gpu
