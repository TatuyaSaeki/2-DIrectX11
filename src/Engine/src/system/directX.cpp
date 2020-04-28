//===========================================================================
//! @file	directX.cpp
//!	@brief	DirectX11
//===========================================================================

//---------------------------------------------------------------------------
//! 僨僗僩儔僋僞
//---------------------------------------------------------------------------
DirectX11::~DirectX11()
{
    cleanup();
}

//---------------------------------------------------------------------------
//! 弶婜壔
//!	@param	[in]	width	暆
//!	@param	[in]	height	崅偝
//!	@param	[in]	hWnd	昤夋懳徾偺僂傿儞僪僂僴儞僪儖
//!	@return	true	惓忢廔椆	
//!	@return	false	僄儔乕廔椆
//---------------------------------------------------------------------------
bool DirectX11::setup(u32 width, u32 height, HWND hWnd)
{
    HRESULT hr                = S_OK;
    u32     createDeviceFlags = 0;
#if 1
    //=============================================================
    // 僨僶僢僌儗僀儎乕僨僶僀僗傪巊梡
    //=============================================================
    // 乮僷僼僅乕儅儞僗偲堷偒姺偊偵僄儔乕徻嵶儊僢僙乕僕弌椡傪峴偆乯
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //-------------------------------------------------------------
    // 僨僶僀僗嶌惉偺偨傔偺僆僾僔儑儞愝掕
    //-------------------------------------------------------------
    {
        DXGI_SWAP_CHAIN_DESC desc{};
        DXGI_MODE_DESC&      buffer = desc.BufferDesc;

        buffer.Width                   = width;                                  // 夋柺夝憸搙(暆)
        buffer.Height                  = height;                                 // 夋柺夝憸搙(崅偝)
        buffer.RefreshRate.Numerator   = 60;                                     // 儕僼儗僢僔儏儗乕僩(暘巕) 60.0fps
        buffer.RefreshRate.Denominator = 1;                                      // 儕僼儗僢僔儏儗乕僩(暘曣)
        buffer.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;             // 僺僋僙儖僼僅乕儅僢僩
        buffer.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // 塮憸僗僉儍儞儔僀儞偺巜掕
        buffer.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;          // 夋憸僗働乕儕儞僌
        desc.SampleDesc.Count          = 1;                                      // 儅儖僠僒儞僾儖傾儞僠僄僀儕傾僔儞僌偺僒儞僾儖悢(1񎎚�8)
        desc.SampleDesc.Quality        = 0;                                      // 丂丂丂丂丂丂丂丂乂丂丂丂丂丂丂丂丂偺昳幙(0乣)
        desc.BufferCount               = 3;                                      // 僼儘儞僩僶僢僼傽傕娷傔偨僶僢僼傽悢 2:僟僽儖僶僢僼傽 3:僩儕僾儖僶僢僼傽
        desc.OutputWindow              = hWnd;                                   // 弌椡懳徾偺僂傿儞僪僂僴儞僪儖
        desc.Windowed                  = true;                                   // true:僂傿儞僪僂儌乕僪 false:僼儖僗僋儕乕儞儌乕僪
        desc.SwapEffect                = DXGI_SWAP_EFFECT_DISCARD;               // 夋柺峏怴儌乕僪

        //---- 僶僢僋僶僢僼傽偺梡搑
        desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |           // 僥僋僗僠儍
                           DXGI_USAGE_RENDER_TARGET_OUTPUT |   // 昤夋僞乕僎僢僩
                           DXGI_USAGE_BACK_BUFFER;             // 僶僢僋僶僢僼傽

        //---- 僶僢僋僶僢僼傽偺摦嶌僼儔僌
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // 夝憸搙曄峏傪嫋壜

        //-------------------------------------------------------------
        // D3D僨僶僀僗嶌惉
        // 摨帪偵僶僢僋僶僢僼傽僗儚僢僾僠僃僀儞偲Immediate僐儞僥僉僗僩偑嶌惉偝傟傞
        //-------------------------------------------------------------
        //---- D3D僨僶僀僗弶婜壔偺帋峴弴彉(Hardware仺Software)
        static const std::array<D3D_DRIVER_TYPE, 3> driverTypes = { {
            D3D_DRIVER_TYPE_HARDWARE,    // 僴乕僪僂僃傾GPU
            D3D_DRIVER_TYPE_WARP,        // 僜僼僩僂僃傾WARP僨僶僀僗 (Windows Advanced Rasterization Platform)
            D3D_DRIVER_TYPE_REFERENCE,   // 儕僼傽儗儞僗儔僗僞儔僀僓
        } };

        //---- D3D僨僶僀僗偱僒億乕僩偡傞婡擻儗儀儖丅嶌惉帋峴弴彉
        static const std::array<D3D_FEATURE_LEVEL, 2> featureLevels = { {
            D3D_FEATURE_LEVEL_11_0,   // DirectX 11.0 (僥僙儗乕僞乕, ComputeShader, 抶墑僐儞僥僉僗僩)
            D3D_FEATURE_LEVEL_10_0,   // DirectX 10.0	(僕僆儊僩儕僔僃乕僟乕, 僗僩儕乕儉弌椡)
        } };

        // 僴乕僪僂僃傾僒億乕僩偟偰偄傞GPU偐傜弴偵嶌惉傪帋峴偟偰偄偔
        for(auto it = driverTypes.begin(); it != driverTypes.end(); ++it) {
            driverType_ = *it;

            hr = D3D11CreateDeviceAndSwapChain(
                nullptr,                                   //	[in]	懳徾偺僨傿僗僾儗僀傾僟僾僞GPU
                driverType_,                               //	[in]	僨僶僀僗偺庬椶
                nullptr,                                   //	[in]	僜僼僩僂僃傾儔僗僞儔僀僓
                createDeviceFlags,                         //	[in]	僆僾僔儑儞僼儔僌
                &featureLevels[0],                         //	[in]	帋峴偡傞婡擻儗儀儖偺攝楍
                static_cast<UINT>(featureLevels.size()),   //	[in]	丂丂丂丂乂丂丂丂丂丂攝楍悢
                D3D11_SDK_VERSION,                         //	[in]	SDK僶乕僕儑儞 (D3D11_SDK_VERSION屌掕)
                &desc,                                     //	[in]	僗儚僢僾僠僃僀儞偺愝掕僷僞儊乕僞
                &d3dSwapChain_,                            //	[out]	嶌惉偝傟偨僗儚僢僾僠僃僀儞
                &d3dDevice_,                               //	[out]	嶌惉偝傟偨Direct3D僨僶僀僗
                &featureLevel_,                            //	[out]	嶌惉偝傟偨僨僶僀僗偺婡擻儗儀儖
                &d3dImmediateContext_);                    //	[out]	嶌惉偝傟偨僀儈僨傿僄僀僩僐儞僥僉僗僩

            //---- 惉岟偟偨応崌偼偦傟埲崀偺帋峴偼峴傢偢偵嶌惉偝傟偨僨僶僀僗傪棙梡丅
            if(hr == S_OK) {
                break;
            }
        }
        // 偡傋偰幐攕偟偨応崌偼僄儔乕
        if(hr != S_OK) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! 夝曻
//---------------------------------------------------------------------------
void DirectX11::cleanup()
{
    //---- 僨僶僀僗僐儞僥僉僗僩偵僙僢僩偝傟偰偄傞僆僽僕僃僋僩傪偡傋偰夝彍偡傞丅
    if(d3dImmediateContext_) {
        d3dImmediateContext_->ClearState();
    }
}
