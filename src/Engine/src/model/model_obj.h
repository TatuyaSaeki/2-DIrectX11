//===========================================================================
//! @file	model_obj.h
//! @brief	Objモデル
//===========================================================================
#pragma once

//===========================================================================
//! @class ObjModel
//===========================================================================
class ObjModel
{
public:
	void tmpInitialize(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount);

    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
	//@{

    //! @brief コンストラクタ
    ObjModel() = default;

    //! @brief デストラクタ
    ~ObjModel() = default;

	//@}
	//-----------------------------------------------------------------------
    //!	@nam	タスク
	//-----------------------------------------------------------------------
	//@{

	bool load(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount, const char* texturePath=nullptr);

    //! @brief 初期化(読み込み)
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	[out]	true		正常終了
	//!	@return	[out]	false		エラー終了
    bool load(const std::string& filePath);

    //! @brief 描画
    void render();

    //! @brief 解放
    void cleanup();

	//@}

	//! tmp pos
	void setPosition(const Vector3& pos)
	{
		position_ = pos;
	}

	Vector3 getPosition() const
	{
		return position_;
	}

private:
    //-------------------------------------------------------------------------------
    //!	@name	読み込み関数まとめ
    //-------------------------------------------------------------------------------

    //! @brief 頂点座標読み込み
    void loadPoint(FILE* fp);

    //! @brief 頂点番号読み込み
    void loadIndex(FILE* fp);

    //! @brief 法線読み込み
    void loadNroaml();

private:
    std::vector<Vertex>  vertices_;
    std::vector<Vector3> points_;
    std::vector<u32>     indices_;

	u32 pointCount_ = 0;
	u32 indexCount_ = 0;

	std::unique_ptr<gpu::Texture> texture0_;

    gpu::ConstantBuffer<cbModel> cbModel_;
    gpu::Buffer                   vertexBuffer_;
    gpu::Buffer                   indexBuffer_;

	Vector3 position_;
};