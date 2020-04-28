//===========================================================================
//!	@file	gpu_texture.cpp
//!	@brief	テクスチャ
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
//! 初期化(D3DResource指定)
//---------------------------------------------------------------------------
bool Texture::initialize(ID3D11Resource* d3dResource, bool cubemap)
{
    d3dResource_.Attach(d3dResource);

    // 種類分けしてダウンキャスト
    D3D11_RESOURCE_DIMENSION type;
    d3dResource->GetType(&type);
    switch(type) {
        case D3D11_RESOURCE_DIMENSION_UNKNOWN:
            return false;
        case D3D11_RESOURCE_DIMENSION_BUFFER:
            return false;
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:   // 1Dテクスチャ
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
        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:   // 2Dテクスチャ
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
        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:   // 3Dテクスチャ
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

    // ビューを自動生成
    // シェーダーリソースビュー
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
    // レンダーターゲット
    if(bindFlags_ & D3D11_BIND_RENDER_TARGET) {
        if(FAILED(d3dDevice->CreateRenderTargetView(d3dResource, nullptr, &d3dRtv_))) {
            return false;
        }
    }
    //　デプスステンシル
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

    // あんオーダー
    if(bindFlags_ & D3D11_BIND_UNORDERED_ACCESS) {
        if(FAILED(d3dDevice->CreateUnorderedAccessView(d3dResource, nullptr, &d3dUav_))) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! テクスチャ作成（ファイル名指定）
//---------------------------------------------------------------------------
gpu::Texture* createTexture(const std::string& fileName, bool cubemap)
{
	// パス追加
	const std::string addFilePath = "../../Application/asset/";
	std::string       filePath = fileName;

	if (fileName.find_first_of(".")) {
		filePath = addFilePath + fileName;
	}

	HRESULT         hr;
	std::vector<u8> textureDDS;

    //================================================================
    // DDSファイルをメモリ上に読み込み
    //================================================================
    FILE* fp;
    if(fopen_s(&fp, filePath.c_str(), "rb") != 0) {
        MessageBox(NULL, (filePath + "\nこのファイル名で実行が出来ない。").c_str(), "エラー", MB_OK);
        return nullptr;
    }

    //---- ファイルサイズを取得
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // ファイルサイズ
    fseek(fp, 0, SEEK_SET);

    //---- ワーク領域を確保して読み込み
    textureDDS.resize(fileSize);   // ワーク領域の確保
    if(textureDDS.empty()) {
        fclose(fp);
        return false;
    }
    fread(&textureDDS[0], fileSize, 1, fp);   // 読み込み
    fclose(fp);

    //================================================================
    // テクスチャを作成
    //================================================================
    u32  cpuAccessFlags = 0;
    u32  bindFlags      = D3D11_BIND_SHADER_RESOURCE;
    u32  miscFlags      = 0;
    bool forceSRGB      = false;

    // 出力結果を受け取る変数
    com_ptr<ID3D11Resource> d3dResource;
    DirectX::DDS_ALPHA_MODE ddsAlphaMode;

    //---- テクスチャ生成
    hr = DirectX::CreateDDSTextureFromMemoryEx(
        device::D3DDevice(),     // [in]	D3Dデバイス
        textureDDS.data(),       // [in]	DDSファイルの画像データの先頭アドレス
        textureDDS.size(),       // [in]	DDSファイルのファイルサイズ
        0,                       // [in]	生成するテクスチャの最大サイズ(0指定で自動設定)
        D3D11_USAGE_IMMUTABLE,   // [in]	用途
        bindFlags,               // [in]	関連付けされる対象
        cpuAccessFlags,          // [in]	CPUアクセス許可フラグ
        miscFlags,               // [in]	その他オプションフラグ
        forceSRGB,               // [in]	sRGBモードに設定 true:強制設定 false:設定しない
        &d3dResource,            // [out]	受け取るID3D11Texture2Dポインタ
        nullptr,                 // [out]	受け取るID3D11ShaderResourceViewポインタ
        &ddsAlphaMode);          // [out]	読み込まれたDDSのアルファモード

    if(hr != S_OK) {
        u32 loadFlags = forceSRGB ? DirectX::WIC_LOADER_FORCE_SRGB : DirectX::WIC_LOADER_IGNORE_SRGB;

        hr = DirectX::CreateWICTextureFromMemoryEx(
            device::D3DDevice(),     // [in]	D3Dデバイス
            textureDDS.data(),       // [in]	DDSファイルの画像データの先頭アドレス
            textureDDS.size(),       // [in]	DDSファイルのファイルサイズ
            0,                       // [in]	生成するテクスチャの最大サイズ(0指定で自動設定)
            D3D11_USAGE_IMMUTABLE,   // [in]	用途
            bindFlags,               // [in]	関連付けされる対象
            cpuAccessFlags,          // [in]	CPUアクセス許可フラグ
            miscFlags,               // [in]	その他オプションフラグ
            loadFlags,               // [in]	sRGBモードに設定 true:強制設定 false:設定しない
            &d3dResource,            // [out]	受け取るID3D11Texture2Dポインタ
            nullptr);                // [out]	受け取るID3D11ShaderResourceViewポインタ
        if(hr != S_OK) {
            // TGAファイルの読み込み
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
    // テクスチャクラスの初期化
    //================================================================
    return gpu::createTexture(d3dResource.Detach(), cubemap);
}

//---------------------------------------------------------------------------
//! テクスチャを作成(ID3D11Resource指定)
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
    return p.release();   // unique_ptrの所有権を解除
}

//---------------------------------------------------------------------------
//! 描画ターゲット作成
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	format	ピクセル形式
//---------------------------------------------------------------------------
gpu::Texture* createRenderTarget(u32 width, u32 height, DXGI_FORMAT format, bool cubemap)
{
    //================================================================
    // 設定
    //================================================================
    D3D11_TEXTURE2D_DESC desc;

    u32 bindFlags = D3D11_BIND_SHADER_RESOURCE;

    if(format == DXGI_FORMAT_D32_FLOAT) {
        bindFlags |= D3D11_BIND_DEPTH_STENCIL;
    }
    else {
		bindFlags |= D3D11_BIND_RENDER_TARGET;
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;	// UAV対応
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
    // 作成
    //=============================================================
    com_ptr<ID3D11Texture2D> d3dTexture2D;
    if(device::D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D) != S_OK) {
        return nullptr;
    }

    return gpu::createTexture(d3dTexture2D.Detach(), cubemap);
}

}   // namespace gpu
