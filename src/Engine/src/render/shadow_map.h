//===========================================================================
//!	@file	shadow_map.h
//!	@brief	シャドウマップ管理
//===========================================================================
#pragma once

//===========================================================================
//! @class	ShadowMap
//===========================================================================
class ShadowMap
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    ShadowMap() = default;

    //! @brief デストラクタ
    ~ShadowMap() = default;

    //@}
    //---------------------------------------------------------------------------
    //!	@name	タスク
    //---------------------------------------------------------------------------
    //@{

    //! @brief 初期化
    bool initialize(s32 resolution = 4096);

    //! @brief 解放
    void finalize();

    //! @brief シャドウパスの開始
    void begin(const Light& light);

    //! @brief シャドウパスの終了
    void end();

    //@}
    //---------------------------------------------------------------------------
    //!	@name	取得
    //---------------------------------------------------------------------------
    //@{

    //! @brief デプステクスチャ取得
	gpu::Texture* getDepthTexture();

    //@}

private:
    gpu::ConstantBuffer<cbShadow> cbShadow_;            //!< 影用定数バッファ
	std::unique_ptr<gpu::Texture> depthTexture_;        //!< 影用デプステクスチャ
    u32                           gpuSlot     = 8;      //!< GPUスロット番号
    s32                           resolution_ = 2048;   //!< 解像度
};
