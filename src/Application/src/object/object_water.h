//===========================================================================
//! @file	object_water.h
//! @brife	水表現用クラス
//===========================================================================
#pragma once

//===========================================================================
//! @class Water
//===========================================================================
class Water : public ObjectBaseModel
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Water();

    //! @brief デストラクタ
    virtual ~Water() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief 初期化
    //! @param [in] buffer	       バッファ用テクスチャ
    //! @param [in] modelPaht      モデルパス
    //! @param [in] scale          モデルのスケール
    //! @param [in] primitiveType  描画時のプリミティブタイプ
    //! @return true		       成功
    //! @return false		       失敗
    //-----------------------------------------------------------------------
    bool initialize(std::shared_ptr<gpu::Texture> buffer, const std::string& modelPath, f32 scale = 1.0f, PRIMITIVE_TYPE primitiveType = PT_TRIANGLES);

    //! @brief 更新
    void update() override;

    //-----------------------------------------------------------------------
    //! @brief 描画
    //! @param [in] 描画したいモード
    //-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief ImGui描画
    void showImGuiWindow() override;

    //@}

private:
    gpu::ConstantBuffer<cbSeaWave> cbWave_;         //!< 定数バッファ
    u32                            gpuSlot_ = 10;   //!< GPUスロット番号

    struct Wave
    {
        f32 speed_        = 0.01f;   //!< 進むスピード
        f32 moveDistance_ = 0.0f;    //!< 進んだ距離
        f32 fineness_     = 0.5f;    //!< 細かさ
        f32 sharp_        = 0.5f;    //!< 尖り度
        f32 height_       = 0.5f;    //!< 高さ
    };
    Wave wave_;   //!< 波用
};