//===========================================================================
//! @file	model_base.h
//===========================================================================
#pragma once

//===========================================================================
//! @class ModelBase
//===========================================================================
class ModelBase
{
public:
	//----------------------------------------------------------
    //!	@name	初期化
    //----------------------------------------------------------

    //! コンストラクタ
	ModelBase() = default;

    //! デストラクタ
    ~ModelBase() = default;

    //----------------------------------------------------------
    //!	@nam	タスク
    //----------------------------------------------------------

    //! 描画
    void render();

    //! 解放
    void cleanup();

	//! tmp pos
	void setPosition(const Vector3& pos)
	{
		position_ = pos;
	}

private:
    //----------------------------------------------------------
    //!	@name	読み込み関数まとめ
    //----------------------------------------------------------

    //! 頂点座標読み込み
    void loadPoint(FILE* fp);

    //! 頂点番号読み込み
    void loadIndex(FILE* fp);

    //! 法線読み込み
    void loadNroaml();

private:
    std::vector<Vertex>  vertices_;
    std::vector<Vector3> points_;
    std::vector<u32>     indices_;

	u32 pointCount_ = 0;
	u32 indexCount_ = 0;

	std::unique_ptr<gpu::Texture> texture0_;

    gpu::ConstantBuffer<CB_World> cbModel_;
    gpu::Buffer                   vertexBuffer_;
    gpu::Buffer                   indexBuffer_;

	Vector3 position_;
};