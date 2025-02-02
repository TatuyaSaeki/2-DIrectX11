//===========================================================================
//! @file	object_sea.h
//! @brife	海オブジェクト
//===========================================================================
#pragma once

//===========================================================================
//! @class Sea
//===========================================================================
class Sea : public ObjectBaseModel
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
	Sea();

    //! @brief デストラクタ
    virtual ~Sea() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //! @brief 更新
    void update() override;

	//-----------------------------------------------------------------------
	//! @brief 描画
	//! @param [in] 描画したいモード
	//-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief ImGui描画
    void showImGuiWindow() override;

    //! @brief 波の数値初期化
    void initializeWave();

    //@}

private:
    //std::unique_ptr<ObjectBaseModel> floor_;          //!< 床用モデル
    gpu::ConstantBuffer<cbSeaWave>   cbWave_;         //!< 定数バッファ
    u32                              gpuSlot_ = 10;   //!< GPUスロット番号

    struct Wave
    {
        f32 speed_        = 0.01f;   //!< 進むスピード
        f32 moveDistance_ = 0.0f;    //!< 進んだ距離
        f32 fineness_     = 0.5f;    //!< 細かさ
        f32 sharp_        = 0.5f;    //!< 尖り度
        f32 height_       = 0.5f;    //!< 高さ
    };
    Wave wave_;   //!< 波用

    //std::shared_ptr<gpu::Texture> buffer_;
};