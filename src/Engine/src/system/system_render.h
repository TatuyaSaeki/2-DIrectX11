//===========================================================================
//! @file	system_render.h
//!	@brief	DirectX11描画システム管理
//===========================================================================
#pragma once

//===========================================================================
//!	レンダリングシステム
//===========================================================================
class SystemRender final
{
public:
    //! @brief シングルトン 実態取得
    static SystemRender& getInstance()
    {
        static SystemRender instance;
        return instance;
    }

private:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    SystemRender() = default;

public:
    //! @brief デストラクタ
    ~SystemRender() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief 初期化
    //!	@param	[in]	width	幅
    //!	@param	[in]	height	高さ
    //!	@return	[out]	true	正常終了
    //!	@return	[out]	false	エラー終了
	//-----------------------------------------------------------------------
    bool setup(u32 width, u32 height);

    //! @brief 解放
    void cleanup();

    //@}
    //-----------------------------------------------------------------------
    //!	@name	制御
    //-----------------------------------------------------------------------
    //@{

    //! @brief 描画開始
    void begin();

    //! @brief 描画終了
    void end();

    //! @brief シーン描画開始
    void beginScene();

    //! @brief シーン描画終了
    void endScene();

public:
	//-----------------------------------------------------------------------
    //!	@brief 画面更新
    //! @param	[in]	interval	画面更新周期 1:60fps 2:30fps 3:20fps 4:15fps
	//-----------------------------------------------------------------------
    void swap(s32 interval);

    //! @brief 全画面描画
    void put();

    //! @brief バッファ切り替え
    void swapRenderBuffer();

    //@}
    //-----------------------------------------------------------------------
    //! @name 設定
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief カリング設定
    //!	@param	[in]	fileMode		描画モード
    //!	@param	[in]	cullingState	カリング設定
	//-----------------------------------------------------------------------
    void settingCullingState(D3D11_FILL_MODE fileMode, D3D11_CULL_MODE cullingState = D3D11_CULL_MODE::D3D11_CULL_NONE);

    //@}
    //-----------------------------------------------------------------------
    //! @name 取得
    //-----------------------------------------------------------------------
    //@{

    //! @brief バックバッファ取得
    gpu::Texture* getBackBuffer() const { return backBuffer_.get(); }

    //! @brief デプスステンシル取得
    gpu::Texture* getDepthStencil() const { return depthStencil_.get(); }

	//-----------------------------------------------------------------------
    //! @brief 描画バッファを取得
    //! index=0: 現在のバッファ
    //! index=1: もうひとつのワークバッファ
	//! @return バッファのポインタ
	//-----------------------------------------------------------------------
    gpu::Texture* getRenderBuffer(u32 index = 0, bool isHDR = true) const
    {
        // HDR描画区間の阿合はHDRテクスチャ、SDR描画区間はbackBuffer;
        u32 x = currentBuffer_ ^ index;
        return (isHDR) ? hdrBuffer_[x].get() : sdrBuffer_[x].get();
    }

	//-----------------------------------------------------------------------
    //! @brief ワーク用HDRバッファを取得
	//! @param [in] inxex 配列番号
	//! @param [in] inxex ミップレベル(最大7)
	//! @return バッファのポインタ
	//-----------------------------------------------------------------------
    gpu::Texture* getHDRWorkBuffer(u32 index, u32 mip = 0) const
    {
        return hdrWorkBuffer_[index][mip].get();
    }

    //!
    FilterManager* getFilterManager() const { return filterManager_.get(); }

	//! @brief IBLテクスチャタイプ設定
	void setIBLTextureType(IBLTextureType type) { ibl_.setTextureType(type); }

    //! @brief ブレンドステートを取得
    ID3D11BlendState* getBlendState(BLEND_MODE mode) const { return d3dBlendState_[mode].Get(); }

    s32 getWidth() const { return width_; }
    s32 getHeight() const { return height_; }

    //@}

private:
    s32                           width_;
    s32                           height_;
    std::unique_ptr<gpu::Texture> backBuffer_;                      //!< バックバッファ
    std::unique_ptr<gpu::Texture> depthStencil_;                    //!< デプスバッファ
    std::unique_ptr<gpu::Texture> hdrBuffer_[2];                    //!< 描画用HDRバッファ
    std::unique_ptr<gpu::Texture> sdrBuffer_[2];                    //!< 描画用SDRバッファ
    std::unique_ptr<gpu::Texture> hdrWorkBuffer_[2][7];             //!< ワーク用HDRバッファ
    u32                           currentBuffer_ = 0;               //!< 現在のバッファのインデックス番号
    com_ptr<ID3D11BlendState>     d3dBlendState_[BLEND_MODE_MAX];   //!< ブレンドステート

    com_ptr<ID3D11RasterizerState> d3dRasterizeState_;             //!< ラスタライズ設定
    com_ptr<ID3D11SamplerState>    d3dSamplerState_LinearWrap_;    //!< サンプラー
    com_ptr<ID3D11SamplerState>    d3dSamplerState_LinearClamp_;   //!< サンプラー

    std::unique_ptr<FilterManager> filterManager_;   //!< フィルタ関連管理
    IBL                            ibl_;             //!< IBL
};

//===========================================================================
// アクセスインターフェイス
//===========================================================================
inline SystemRender* GmRender()
{
    return &SystemRender::getInstance();
}

inline void appFilterCleanup()
{
    GmRender()->getFilterManager()->appFilterCleanup();
}

inline void setFilter(FilterType filterType)
{
    GmRender()->getFilterManager()->setFilter(filterType);
}

inline void removeFilter(FilterType filterType)
{
    GmRender()->getFilterManager()->removeFilter(filterType);
}