//===========================================================================
//!	@file	primitive.cpp
//!	@brief	プリミティブ描画
//===========================================================================
namespace {

std::vector<Matrix> matrixStack_;
Matrix              matWorld_ = Matrix::identity();

gpu::Buffer*                   dynamicBuffer_;
std::unique_ptr<gpu::ShaderVs> shaderVs3D_;
std::unique_ptr<gpu::ShaderPs> shaderPsFlat_;
std::unique_ptr<gpu::ShaderPs> shaderPsTexture_;
gpu::ConstantBuffer<cbModel>  cbWorld_;

const u32 MAX_VERTEX_COUNT = 65536* 1024;   // 最大登録頂点数
const u32 BUFFER_SIZE      = u32(sizeof(Vertex)) * MAX_VERTEX_COUNT;

u32            usedCount_ = 0;      // 使用中の頂点数
u32            drawCount_;          // 頂点個数
Vertex         vertex_;             // 頂点の一時領域
PRIMITIVE_TYPE type_ = PT_POINTS;   // 現在のプリミティブの種類
Vertex*        pvertex_;            // 現在マップされている頂点の先頭

}   // namespace

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool PRIM_Initialize()
{
    // 動的頂点バッファの初期化
    dynamicBuffer_ = new gpu::Buffer();
    if(dynamicBuffer_->initialize(BUFFER_SIZE, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER) == false) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void PRIM_Finalize()
{
    shaderVs3D_.reset();
    shaderPsFlat_.reset();
    shaderPsTexture_.reset();

    GM_SAFE_DELETE(dynamicBuffer_);
}

//==============================================================
// OpenGL互換関数
//==============================================================

//---------------------------------------------------------------------------
//! 行列の設定
//---------------------------------------------------------------------------
void dxLoadMatrixf(const Matrix& matWorld)
{
    auto p       = cbWorld_.begin();
    p->matWorld_ = matWorld;
    cbWorld_.end();

    // 内部に退避
    matWorld_ = matWorld;

    gpu::setConstantBuffer(1, cbWorld_);
}

//---------------------------------------------------------------------------
//! 行列スタックへ退避
//---------------------------------------------------------------------------
void dxPushMatrix()
{
    matrixStack_.emplace_back(matWorld_);
}

//---------------------------------------------------------------------------
//! 行列スタックから復帰
//---------------------------------------------------------------------------
void dxPopMatrix()
{
    matWorld_ = matrixStack_.back();
    matrixStack_.pop_back();
    dxLoadMatrixf(matWorld_);   // 再設定
}

//---------------------------------------------------------------------------
//! 描画開始
//!	@param	[in]	type	プリミティブの種類
//---------------------------------------------------------------------------
void dxBegin(PRIMITIVE_TYPE type)
{
    // メモリマップ開始
    D3D11_MAP map = D3D11_MAP_WRITE_NO_OVERWRITE;

    // 満杯近くなら再確保
    if(MAX_VERTEX_COUNT - 65536 * 10 < usedCount_) {
        map        = D3D11_MAP_WRITE_DISCARD;
        usedCount_ = 0;   // 最初から利用
    }

    D3D11_MAPPED_SUBRESOURCE mapped;

    device::D3DContext()->Map(dynamicBuffer_->getD3DBuffer(), 0, map, 0, &mapped);

    // 利用中の先頭アドレス
    pvertex_ = &static_cast<Vertex*>(mapped.pData)[usedCount_];

    // 上と同じ
    //pvertex_ = static_cast<Vertex*> (mapped.pData);
    //++usedCount_;

    type_ = type;
}

//---------------------------------------------------------------------------
//! 頂点キック (Kick)
//---------------------------------------------------------------------------
void dxVertex3f(f32 x, f32 y, f32 z)
{
    // 値の設定
    vertex_.position_.x_ = x;
    vertex_.position_.y_ = y;
    vertex_.position_.z_ = z;

    // 頂点キック
    *pvertex_++ = vertex_;   // 一時ワークから書き出し
    drawCount_++;
}

//---------------------------------------------------------------------------
//! 頂点キック (Kick) Vector3
//---------------------------------------------------------------------------
void dxVertex3fv(const Vector3& pos)
{
    dxVertex3f(pos.x_, pos.y_, pos.z_);
}

//---------------------------------------------------------------------------
//! カラーu8
//---------------------------------------------------------------------------
void dxColor4ub(u8 r, u8 g, u8 b, u8 a)
{
    vertex_.color_ = Color(r, g, b, a);
}

//------------------------------------------------------
//! カラーu8
//------------------------------------------------------
void dxColor4ub(const Color& color)
{
    vertex_.color_ = Color(color.r_, color.g_, color.b_, color.a_);
}

//---------------------------------------------------------------------------
//! カラーf32
//---------------------------------------------------------------------------
void dxColor4f(f32 fr, f32 fg, f32 fb, f32 fa)
{
    u8 r = (u8)std::max(0.0f, std::min(fr * 255.5f, 255.5f));
	u8 g = (u8)std::max(0.0f, std::min(fg * 255.5f, 255.5f));
	u8 b = (u8)std::max(0.0f, std::min(fb * 255.5f, 255.5f));
	u8 a = (u8)std::max(0.0f, std::min(fa * 255.5f, 255.5f));

    vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! カラーf32
//---------------------------------------------------------------------------
void dxColor4f(const Vector4& color)
{
	u8 r = (u8)std::max(0.0f, std::min(color.r_ * 255.5f, 255.5f));
	u8 g = (u8)std::max(0.0f, std::min(color.g_ * 255.5f, 255.5f));
	u8 b = (u8)std::max(0.0f, std::min(color.b_ * 255.5f, 255.5f));
	u8 a = (u8)std::max(0.0f, std::min(color.a_ * 255.5f, 255.5f));

	vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! テクスチャ座標
//---------------------------------------------------------------------------
void dxTexCoord2f(f32 u, f32 v)
{
    vertex_.uv_.x_ = u;
    vertex_.uv_.y_ = v;
}

//---------------------------------------------------------------------------
//! 法線
//---------------------------------------------------------------------------
void dxNormal3fv(const Vector3& v)
{
	// 値の設定
	vertex_.normal_.x_ = v.x_;
	vertex_.normal_.y_ = v.y_;
	vertex_.normal_.z_ = v.z_;
}

//---------------------------------------------------------------------------
//! 描画終了
//---------------------------------------------------------------------------
void dxEnd()
{
    // メモリマッピング終了
    device::D3DContext()->Unmap(dynamicBuffer_->getD3DBuffer(), 0);

    // 描画
    gpu::setVertexBuffer(dynamicBuffer_, sizeof(Vertex));   // 頂点バッファ

    // 描画
    gpu::draw(type_, drawCount_, usedCount_);

    usedCount_ += drawCount_;
    drawCount_ = 0;
}

//---------------------------------------------------------------------------
//! テクスチャの設定
//---------------------------------------------------------------------------
void dxTexture(ptr<gpu::Texture> texture)
{
	auto shaderVs	 = ShaderManager::getShaderVs("vsStandard");
	auto shaderPs	 = ShaderManager::getShaderPs("psStandard");
	auto shaderPsTex = ShaderManager::getShaderPs("psTexture3D");

    gpu::setShader(shaderVs, (texture) ? shaderPsTex : shaderPs);
    gpu::setTexture(0, texture);
}
