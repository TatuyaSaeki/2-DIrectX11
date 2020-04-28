//===========================================================================
//! @file	asset_model.h
//!	@brief	アセットモデル所有クラス
//===========================================================================
#pragma once

//--------------------------------------------------------------------------
//! 三角形の座標とUV座標から接ベクトルを求める
//! @return [out]	1.接ベクトル
//! @return [out]	2.従法線ベクトル
//--------------------------------------------------------------------------
std::tuple<Vector3, Vector3> calcTangent(
    const Vector3& p0, const Vector3& p1, const Vector3& p2,
    const Vector2& uv0, const Vector2& uv1, const Vector2& uv2);

//! 前方宣言
class Mesh;

//===========================================================================
//! @clsss AssetModel
//===========================================================================
class AssetModel : public AssetBase
{
public:
    //--------------------------------------------------------------------------
    //!	@name	初期化
    //--------------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    AssetModel() = default;

    //! @brief デストラクタ
    virtual ~AssetModel() override = default;

    //@}
    //--------------------------------------------------------------------------
    //!	@name	タスク
    //--------------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief 初期化
    //!	@param	[in]	fileName	ファイル名
    //!	@return	true  正常終了
    //!	@return	false エラー終了
	//--------------------------------------------------------------------------
    virtual bool load([[maybe_unused]] const std::string& fileName) override { return true; };

	//--------------------------------------------------------------------------
    //! @brief 初期化(直接指定)
    //!	@param	[in]	fileName	ファイル名
    //!	@param	[in]	vertices	バーテックス配列
    //!	@param	[in]	indices		インデックス配列
    //!	@param	[in]	iSize		インデックス配列サイズ
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
	//--------------------------------------------------------------------------
    bool load(Vertex* vertices, u32* indices, u32 iSize);

    //! @brief 解放
    virtual void cleanup() override{};

    //@}
    //--------------------------------------------------------------------------
    //!	@name	取得
    //--------------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief メッシュ取得
    //!	@return メッシュ配列
	//--------------------------------------------------------------------------
    std::vector<Mesh>& getMeshes() { return meshes_; }

    //@}

protected:
    std::vector<Mesh> meshes_;   //!< メッシュ
};

//===========================================================================
//! @struct Material
//===========================================================================
struct Material
{
    enum class TextureType
    {
        Albedo,
        Normal,
        Roughness,
        Metalness,
        Ao,
        //----
        Max,
    };

    std::shared_ptr<gpu::Texture> texture_[(s32)TextureType::Max] = {};
};

//===========================================================================
//! @clsss Mesh
//===========================================================================
class Mesh
{
public:
    //-----------------------------------------------------------------------
    //! @name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Mesh() = default;

    //! @brief デストラクタ
    virtual ~Mesh() = default;

    //@}
    //-----------------------------------------------------------------------
    //! @name	タスク
    //-----------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief 初期化
    //!	@param	[in]	fileName	ファイル名
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
	//--------------------------------------------------------------------------
    virtual bool load(const std::string& fileName) { return true; };

	//--------------------------------------------------------------------------
    //! @brief 初期化(直接指定)
    //!	@param	[in]	fileName	ファイル名
    //!	@param	[in]	vertices	バーテックス配列
    //!	@param	[in]	indices		インデックス配列
    //!	@param	[in]	iSize		インデックス配列サイズ
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
	//--------------------------------------------------------------------------
    bool load(Vertex* vertices, u32* indices, u32 iSize);

    //@}

protected:
    //-----------------------------------------------------------------------
    //! @name	読み込み
    //-----------------------------------------------------------------------
    //@{

    //! @brief 頂点読み込み
    virtual void loadVertices(){};

    //! @brief 頂点インデックス番号読み込み
    virtual void loadIndices(){};

    //! @brief 法線読み込み
    virtual void loadNormals(){};

    //! @brief テクスチャUV読み込み
    virtual void loadUVs(){};

    //! @brief マテリアル読み込み
    virtual void loadMaterials(){};

    //! @brief バッファ初期化
    virtual bool initializeBuffer();

    //@}

public:
    //-----------------------------------------------------------------------
    //! @name	取得
    //-----------------------------------------------------------------------
    //@{

    //! @brief 頂点情報を取得
    const std::vector<Vertex>& getVertices() const { return vertices_; }

    //! @brief 頂点位置を取得
    const std::vector<Vector3>& getVertexPoints() const { return vertexPoints_; }

    //! @brief 頂点インデックス番号を取得
    const std::vector<u32>& getIndices() const { return indices_; }

    //! @brief 法線を取得
    const std::vector<Vector3>& getNormals() const { return normals_; }

    //! @brief 接ベクトルを取得
    const std::vector<Vector3>& getTangents() const { return tangents_; }

    //! @brief 従法線ベクトルを取得
    const std::vector<Vector3>& getBinormals() const { return binormals_; }

    //! @brief UV座標を取得
    const std::vector<Vector2>& getUVs() const { return uvs_; }

    //! @brief マテリアル一覧を取得
    const std::vector<Material>& getMaterials() const { return materials_; }

    //! @brief マテリアル番号を取得
    const std::vector<s32>& getMaterialIndices() const { return materialIndices_; }

    //! @brief 頂点バッファ取得
    const gpu::Buffer& getVertexBuffer() const { return vertexBuffer_; }

    //! @brief インデックスバッファ取得
    const gpu::Buffer& getIndexBuffer() const { return indexBuffer_; }

    //@}

protected:
    std::vector<Vertex>   vertices_;          //!< 頂点情報
    std::vector<Vector3>  vertexPoints_;      //!< 頂点位置
    std::vector<u32>      indices_;           //!< 頂点インデックス番号
    std::vector<Vector3>  normals_;           //!< 法線
    std::vector<Vector3>  tangents_;          //!< 接線
    std::vector<Vector3>  binormals_;         //!< 従法線
    std::vector<Vector2>  uvs_;               //!< テクスチャUV座標
    std::vector<Material> materials_;         //! マテリアル
    std::vector<s32>      materialIndices_;   //! マテリアル番号

    gpu::Buffer vertexBuffer_;   //!< 頂点バッファ
    gpu::Buffer indexBuffer_;    //!< インデックスバッファ
};
