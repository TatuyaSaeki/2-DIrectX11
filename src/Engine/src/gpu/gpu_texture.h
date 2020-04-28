//===========================================================================
//!	@file	gpu_texture.h
//!	@brief	テクスチャ
//===========================================================================
#pragma once

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//===========================================================================
//! @class	Texture
//===========================================================================
class Texture
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Texture() = default;

    //! @brief デストラクタ
    ~Texture() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief 初期化(D3DResource指定)
    //!	@param	[in]	d3dResource	ID3D11Resourceのポインタ
    //!	@param	[in]	cubemap		キューブマップかどうか
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
    //-----------------------------------------------------------------------
    bool initialize(ID3D11Resource* d3dResource, bool cubemap = false);

    //@}
    //-----------------------------------------------------------------------
    //!	@name	取得
    //-----------------------------------------------------------------------
    //@{

    u32         getWidth() const { return width_; }           // 幅取得
    u32         getHeight() const { return height_; }         // 高さ取得
    u32         getMipLevels() const { return mipLevels_; }   // ミップレベル取得
    DXGI_FORMAT getFormat() const { return format_; }         // 形式取得

    ID3D11Resource*            getD3DResource() const { return d3dResource_.Get(); }   // テクスチャメモリー
    ID3D11ShaderResourceView*  getD3DSrv() const { return d3dSrv_.Get(); }             // SRV
    ID3D11RenderTargetView*    getD3DRtv() const { return d3dRtv_.Get(); }             // RTV
    ID3D11DepthStencilView*    getD3DDsv() const { return d3dDsv_.Get(); }             // DSV
    ID3D11UnorderedAccessView* getD3DUav() const { return d3dUav_.Get(); }             // UAV

    //@}

private:
    u32         width_     = 0;                     //!< 幅
    u32         height_    = 0;                     //!< 高さ
    u32         depth_     = 0;                     //!< 奥行き
    u32         mipLevels_ = 0;                     //!< ミップレベル数
    DXGI_FORMAT format_    = DXGI_FORMAT_UNKNOWN;   //!< ピクセルフォーマット
    u32         bindFlags_ = 0;                     //!< 関連付け情報(D3D11_BIND_FLAGの組み合わせ)

    com_ptr<ID3D11Resource>            d3dResource_;   //!< テクスチャメモリー
    com_ptr<ID3D11ShaderResourceView>  d3dSrv_;        //!< SRV
    com_ptr<ID3D11RenderTargetView>    d3dRtv_;        //!< RTV
    com_ptr<ID3D11DepthStencilView>    d3dDsv_;        //!< DSV
    com_ptr<ID3D11UnorderedAccessView> d3dUav_;        //!< UAV
};

//-----------------------------------------------------------------------
//! @brief  テクスチャ作成（ファイル名指定）
//!	@param	[in]	filePath	ファイル名
//!	@param	[in]	cubemap		キューブマップかどうか
//!	@return	テクスチャのポインタ, 失敗nullptr
//-----------------------------------------------------------------------
gpu::Texture* createTexture(const std::string& filePath, bool cubemap = false);

//-----------------------------------------------------------------------
//! @brief  テクスチャを作成(ID3D11Resource指定)
//!	@param	[in]	filePath	ファイル名
//!	@param	[in]	cubemap		キューブマップかどうか
//!	@return	テクスチャのポインタ, 失敗nullptr
//-----------------------------------------------------------------------
gpu::Texture* createTexture(ID3D11Resource* d3dResouce, bool cubemap = false);

//-----------------------------------------------------------------------
//! @brief  描画ターゲット作成
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	format	ピクセル形式
//!	@param	[in]	cubemap	キューブマップかどうか
//!	@return	テクスチャのポインタ
//-----------------------------------------------------------------------
gpu::Texture* createRenderTarget(u32 width, u32 height, DXGI_FORMAT format, bool cubemap = false);

}   // namespace gpu
