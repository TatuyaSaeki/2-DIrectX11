//===========================================================================
//!	@file	directX.h
//!	@brief	DirectX11
//===========================================================================
#pragma once

//! ブレンドモード種類
enum BLEND_MODE
{
    BLEND_MODE_OFF,     //!< 無効
    BLEND_MODE_BLEND,   //!< 通常ブレンド
    BLEND_MODE_ADD,     //!< 加算
    //----
    BLEND_MODE_MAX,   //!< ブレンドモード個数
};

//===========================================================================
//!	@class DirectX11
//===========================================================================
class DirectX11
{
public:
    //! @brief シングルトン
    static DirectX11& getInstance()
    {
        static DirectX11 instance;
        return instance;
    }

private:
    //-----------------------------------------------------------------------
    //!	@name	初期化
	//-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    DirectX11() = default;

public:
    //! @brief デストラクタ
    virtual ~DirectX11();

    //@}
	//-----------------------------------------------------------------------
    //! @name	タスク
	//-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief 初期化
    //!	@param	[in]	width	幅
    //!	@param	[in]	height	高さ
    //!	@param	[in]	hWnd	描画対象のウィンドウハンドル
    //!	@return	true	正常終了	（成功）
    //!	@return	false	エラー終了	（失敗）
	//-----------------------------------------------------------------------
    bool setup(u32 width, u32 height, HWND hWnd);

    //! @brief 解放
    void cleanup();

    //@}

	//-----------------------------------------------------------------------
    //! @name	取得
	//-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief D3Dデバイスを取得
	//! @return d3dDeviceのポインタ
	//-----------------------------------------------------------------------
    ID3D11Device* getD3DDevice() const { return d3dDevice_.Get(); }

	//-----------------------------------------------------------------------
    //! @brief D3Dコンテキストを取得
	//! @return d3dImmediateContextのポインタ
	//-----------------------------------------------------------------------
    ID3D11DeviceContext* getD3DContext() const { return d3dImmediateContext_.Get(); }

	//-----------------------------------------------------------------------
	//! @brief スワップチェイン取得
	//! @return d3dSwapChainのポインタ
	//-----------------------------------------------------------------------
    IDXGISwapChain* getD3DSwapChain() const { return d3dSwapChain_.Get(); }

    ////! ラスタライズ取得
    //ID3D11RasterizerState* getD3DRastarizeState() const { return d3dRasterizeState_.Get(); }
    //
    ////! サンプラー取得
    //ID3D11SamplerState* getD3DSamplerLinear() const { return d3dSamplerLinear_.Get(); }
    //
    //// ブレンドステートを取得
    //ID3D11BlendState* getBlendState(BLEND_MODE mode) const { return d3dBlendState_[mode].Get(); }

    //@}

private:
    D3D_DRIVER_TYPE   driverType_   = D3D_DRIVER_TYPE_UNKNOWN;   //!< 作成されたデバイスの種類		(HW, WARP, REF)
    D3D_FEATURE_LEVEL featureLevel_ = D3D_FEATURE_LEVEL_11_0;    //!< 作成されたデバイスの機能レベル	(DX9,DX10,DX11)

    com_ptr<ID3D11Device>        d3dDevice_;             //!< D3Dデバイス
    com_ptr<ID3D11DeviceContext> d3dImmediateContext_;   //!< イミディエイトコンテキスト
    com_ptr<IDXGISwapChain>      d3dSwapChain_;          //!< スワップチェイン
};

namespace device {

inline ID3D11Device* D3DDevice()
{
    return DirectX11::getInstance().getD3DDevice();
}

inline ID3D11DeviceContext* D3DContext()
{
    return DirectX11::getInstance().getD3DContext();
}
}   // namespace device