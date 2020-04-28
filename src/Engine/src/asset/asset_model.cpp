//===========================================================================
//! @file	asset_model.cpp
//!	@brief	アセットモデル所有クラス
//===========================================================================

//--------------------------------------------------------------------------
// 三角形の座標とUV座標から接ベクトルを求める
//--------------------------------------------------------------------------
std::tuple<Vector3, Vector3> calcTangent(
    const Vector3& p0, const Vector3& p1, const Vector3& p2,
    const Vector2& uv0, const Vector2& uv1, const Vector2& uv2)
{
    // 5次元→3次元頂点に
    Vector3 cp0[3] = {
        { p0.x_, uv0.x_, uv0.y_ },
        { p0.y_, uv0.x_, uv0.y_ },
        { p0.z_, uv0.x_, uv0.y_ },
    };
    Vector3 cp1[3] = {
        { p1.x_, uv1.x_, uv1.y_ },
        { p1.y_, uv1.x_, uv1.y_ },
        { p1.z_, uv1.x_, uv1.y_ },
    };
    Vector3 cp2[3] = {
        { p2.x_, uv2.x_, uv2.y_ },
        { p2.y_, uv2.x_, uv2.y_ },
        { p2.z_, uv2.x_, uv2.y_ },
    };

    // 平面パラメータからUV軸座標算出
    f32 u[3], v[3];
    for(int i = 0; i < 3; ++i) {
        Vector3 v1  = cp1[i] - cp0[i];
        Vector3 v2  = cp2[i] - cp1[i];
        Vector3 abc = Vector3::cross(v1, v2);

        if(abc.x_ == 0.0f) {
            // やばいす！
            // ポリゴンかUV上のポリゴンが縮退してます！
            //GM_ASSERT_MESSAGE(false, "三角形かUV座標が縮退しています.");
            return std::make_tuple(Vector3::ZERO, Vector3::ZERO);
        }

        u[i] = -abc.y_ / abc.x_;
        v[i] = -abc.z_ / abc.x_;
    }

    Vector3 tangent  = Vector3{ u[0], u[1], u[2] }.normalize();
    Vector3 binormal = Vector3{ v[0], v[1], v[2] }.normalize() * -1.0f;

    return std::make_tuple(tangent, binormal);
}

//---------------------------------------------------------------------------
//! 初期化(直接指定)
//---------------------------------------------------------------------------
bool AssetModel::load(Vertex* vertices, u32* indices, u32 iSize)
{
    Mesh mesh{};
    if(!mesh.load(vertices, indices, iSize))
        return false;

    meshes_.emplace_back(mesh);

    return true;
}

//===========================================================================
//! Mesh
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化(直接指定)
//---------------------------------------------------------------------------
bool Mesh::load(Vertex* vertices, u32* indices, u32 iSize)
{
    std::vector<Vertex> vertex(iSize);
    std::vector<u32>    index(iSize);

    for(size_t i = 0; i < iSize; ++i) {
        vertex[i].position_ = vertices[i].position_;
        vertex[i].normal_   = vertices[i].normal_;
        vertex[i].color_    = vertices[i].color_;
        vertex[i].uv_       = vertices[i].uv_;

        index[i] = indices[i];
    }
    vertices_ = std::move(vertex);
    indices_  = std::move(index);

    if(!initializeBuffer())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! バッファ初期化
//---------------------------------------------------------------------------
bool Mesh::initializeBuffer()
{
    // 頂点バッファ初期化
    if(!vertexBuffer_.initialize(sizeof(Vertex) * vertices_.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, vertices_.data()))
        return false;

    // インデックスバッファ初期化
    if(!indexBuffer_.initialize(sizeof(u32) * indices_.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, indices_.data()))
        return false;

    return true;
}