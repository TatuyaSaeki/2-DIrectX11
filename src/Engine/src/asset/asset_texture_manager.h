//===========================================================================
//! @file	asset_texture_manager.h
//!	@brief	テクスチャ管理クラス
//===========================================================================
#pragma once

//! システムテクスチャ種類
enum SYSTEM_TEXTURE : u32
{
    SYSTEM_TEXTURE_NULL_WHITE = 0,
    SYSTEM_TEXTURE_NULL_BLACK,
    SYSTEM_TEXTURE_NULL_NORMAL,
    SYSTEM_TEXTURE_CUBEMAP_NULL_BLACK,
    //----
    // tmp
    SYSTEM_TEXTURE_TOON,

    SYSTEM_TEXTURE_MAX,
};

//===========================================================================
//! @clsss AssetTextureManager
//===========================================================================
class AssetTextureManager
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    AssetTextureManager() = default;

    //! @brief デストラクタ
    ~AssetTextureManager() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief 初期化
    //! @return true	正常終了
    //! @return false	エラー終了
    //-----------------------------------------------------------------------
    bool initialize();

    //! @brief 解放
    void cleanup();

    //-----------------------------------------------------------------------
    //! @brief システムテクスチャ取得
    //! @param	[in]	type		取得したいシステムテクスチャタイプ
    //! @return	テクスチャのポインタ(受け取る側は、weak_ptr)
    //-----------------------------------------------------------------------
    std::shared_ptr<gpu::Texture> getSystemTexture(SYSTEM_TEXTURE type);

    //-----------------------------------------------------------------------
    //! @brief テクスチャ取得
    //! @param	[in]	fileName	取得したいファイル名
    //! @param	[in]	isCubemap	キューブマップかどうか(作成用に必要)
    //! @return	テクスチャのポインタ(受け取る側は、weak_ptr)
    //-----------------------------------------------------------------------
    std::shared_ptr<gpu::Texture> getTexture(const std::string& fileName, bool isCubemap = false);

private:
    //-----------------------------------------------------------------------
    //! @brief テクスチャ作成
    //! @param	[in]	fileName		取得したいファイル名
    //! @param	[in]	isCubemap		キューブマップかどうか(作成用に必要)
    //! @return	テクスチャのポインタ
    //-----------------------------------------------------------------------
    gpu::Texture* createTexture(const std::string& fileName, bool isCubemap = false);

    //-----------------------------------------------------------------------
    //!	@brief テクスチャの追加
    //! @param	[in]	fileName		取得したいファイル名
    //! @param	[in]	isCubemap		キューブマップかどうか(作成用に必要)
    //! @return テクスチャのポインタ
    //-----------------------------------------------------------------------
    void addTexture(const std::string& fileName, gpu::Texture* assetTexture);

    //@}

private:
    std::shared_ptr<gpu::Texture>                                  systemTextures_[SYSTEM_TEXTURE_MAX];   //!< システムテクスチャ配列                            //!< システムテクスチャ
    std::unordered_map<std::string, std::shared_ptr<gpu::Texture>> assetTextures_;                        //!< テクスチャ
};