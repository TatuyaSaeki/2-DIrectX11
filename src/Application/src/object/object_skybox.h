//===========================================================================
//! @file	object_skybox.h
//! @brife	スカイボックス
//===========================================================================
#pragma once

//===========================================================================
//! @class Skybox
//===========================================================================
class Skybox : public ObjectBaseModel
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Skybox();

    //! @brief デストラクタ
    virtual ~Skybox() override = default;

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

    //-----------------------------------------------------------------------
    //! @brief  テクスチャ入れ替え
    //! @param  [in] type 入れ替えたいIBLテクスチャタイプ
    //! @return true  正常終了
    //! @return false エラー終了
    //-----------------------------------------------------------------------
    bool chengeTexture(IBLTextureType type);

    //@}

private:
    IBLTextureType currentTextureType_ = IBLTextureType::None;   //!< 前のテクスチャタイプ
};