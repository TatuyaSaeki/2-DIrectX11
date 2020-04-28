//===========================================================================
//!	@file	model_mqo.h
//!	@brief	MQOモデル
//===========================================================================
#pragma once

//===========================================================================
//!	モデル
//===========================================================================
class ModelMQO
{
public:
    // 面情報
    struct Face
    {
        s32     vertexIndex_[3];   //!< 頂点番号
        s32     material_;         //!< マテリアル番号(-1の場合はマテリアルなし)
        Vector3 normal_[3];        //!< 法線
        f32     u_[3];             //!< テクスチャ座標U
        f32     v_[3];             //!< テクスチャ座標V
    };

    // パーツオブジェクト
    struct Object
    {
        std::string          name_;       //!< 名前
        std::vector<Vector3> vertices_;   //!< 頂点
        std::vector<Face>    faces_;      //!< フェイス

        Vector4 color_;   //!< オブジェクトカラー
        f32     facet_ = 60.0f;
        //! 最適化
        void optimize();
    };

    // マテリアル
    struct Material
    {
        gpu::Texture* texture_ = nullptr;
        Vector4       color_;   //!< マテリアルカラー
    };

    //! @brief コンストラクタ
    ModelMQO();

    //! @brief デストラクタ
    ~ModelMQO();

    //! @brief 読み込み
    bool load(char* fileName, f32 scale = 1.0f);

    //! @brief 描画
    void render();

    //! @brief Objectの個数を取得
    size_t getObjectCount() const;

    //! @brief Objectの取得
    Object* getObject(u32 index) const { return objects_[index]; }

private:
    std::vector<Object*>  objects_;
    std::vector<Material> materials_;
};
