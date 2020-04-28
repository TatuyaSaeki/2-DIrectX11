//===========================================================================
//! @file	model.h
//!	@brief
//===========================================================================
#pragma once

//===========================================================================
//! @class Model
//===========================================================================
class Model
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Model() = default;

    //! @brief デストラクタ
    virtual ~Model() = default;

    //@}
    //---------------------------------------------------------------------------
    //!	@name	タスク
    //---------------------------------------------------------------------------
    //@{

	//---------------------------------------------------------------------------
    //! @brief 読み込み
	//! @param [in] fileName      ファイル名
	//! @param [in] scale         モデルのスケール
	//! @param [in] primitiveType プリミティブタイプ
	//! @return true  正常終了
	//! @return false エラー終了
	//---------------------------------------------------------------------------
    virtual bool load(const std::string& fileName, [[maybe_unused]] const Vector3& scale = Vector3::ONE, PRIMITIVE_TYPE primitiveType = PRIMITIVE_TYPE::PT_TRIANGLES);

	//---------------------------------------------------------------------------
    //! @brief 描画
	//! @param [in] matWorld ワールド行列
	//---------------------------------------------------------------------------
    virtual void render(const Matrix& matWorld);

    //! @brief 解放
    virtual void cleanup();

    //@}
    //---------------------------------------------------------------------------
    //!	@name	設定
    //---------------------------------------------------------------------------
    //@{

	//---------------------------------------------------------------------------
    //! @brief テクスチャ設定
	//! @param [in] slot    GPUスロット番号
	//! @param [in] texture テクスチャ
	//---------------------------------------------------------------------------
    bool setTexture(s32 slot, std::shared_ptr<gpu::Texture> texture);

    //@}

protected:
    //! 使用するテクスチャ(外部からの指定用)
    struct UseTexture
    {
        UseTexture() = default;
        UseTexture(std::shared_ptr<gpu::Texture> texture, s32 gpuSlot)
        : texture_(texture)
        , gpuSlot_(gpuSlot)
        {
        }

        std::shared_ptr<gpu::Texture> texture_;
        s32                           gpuSlot_;
    };

    std::weak_ptr<AssetModel> assetModel_;      //!< 外部から受け取る
    std::vector<UseTexture>   useTexture_;      //!< 使用するシェーダー
    PRIMITIVE_TYPE            primitiveType_;   //!< 描画時のプリミティブタイプ
};