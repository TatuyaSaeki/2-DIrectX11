//===========================================================================
//!	@file	gpu_command.h
//!	@brief	GPUコマンド
//===========================================================================
#pragma once

//! プリミティブの種類
enum PRIMITIVE_TYPE
{
    PT_POINTS          = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    PT_LINES           = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    PT_LINE_STRIP      = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PT_TRIANGLES       = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    PT_TRIANGLE_STRIP  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    PT_3_CONTROL_POINT = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
    PT_4_CONTROL_POINT = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
};

//! 前方宣言
struct UseShader;

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! @brief 入力レイアウトの設定
//! @param [in] inputLayout 入力レイアウト直接
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout);

//---------------------------------------------------------------------------
//! @brief 入力レイアウトの設定
//! @param [in] vsFileName 頂点シェーダーの名前
//---------------------------------------------------------------------------
void setInputLayout(const std::string& vsFileName);

//---------------------------------------------------------------------------
//! @brief 頂点バッファの設定
//! @param [in] buffer  頂点バッファ
//! @param [in] stride  配列各要素のオフセット値?
//! @param [in] offset  配列オフセット値
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset = 0);

//---------------------------------------------------------------------------
//! @brief インデックスバッファの設定
//! @param [in] buffer  インデックスバッファ
//! @param [in] offset  配列オフセット値
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset = 0);

//---------------------------------------------------------------------------
//! シェーダーの設定
//! 個別設定にするかも
//!	@param	[in]	vs	頂点シェーダー
//!	@param	[in]	ps	ピクセルシェーダー
//!	@param	[in]	gs	ジオメトリシェーダー
//!	@param	[in]	hs	ハルシェーダー
//!	@param	[in]	ds	ドメインシェーダー
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps, ptr<const gpu::ShaderGs> gs = nullptr, ptr<const gpu::ShaderHs> hs = nullptr, ptr<const gpu::ShaderDs> ds = nullptr);

//---------------------------------------------------------------------------
//! シェーダーの設定 (ファイル名指定)
//! 個別設定にするかも
//!	@param	[in]	vs	頂点シェーダー
//!	@param	[in]	ps	ピクセルシェーダー
//!	@param	[in]	gs	ジオメトリシェーダー
//!	@param	[in]	hs	ハルシェーダー
//!	@param	[in]	ds	ドメインシェーダー
//---------------------------------------------------------------------------
void setShader(const std::string& fileNameVs, const std::string& fileNamePs, const std::string* fileNameGs = nullptr, const std::string* fileNameHs = nullptr, const std::string* fileNameDs = nullptr);
void setShader(const std::string& fileNameVs, const std::string* fileNamePs);
void setShader(const UseShader& shaderName);

//---------------------------------------------------------------------------
//! @brief 定数バッファの設定
//! @param [in] slot   GPUスロット番号
//! @param [in] buffer 定数バッファ
//---------------------------------------------------------------------------
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer);

//---------------------------------------------------------------------------
//! @brief テクスチャの設定
//! @param [in] slot    GPUスロット番号
//! @param [in] texture テクスチャ
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture);

//---------------------------------------------------------------------------
//! @brief 描画
//!	@param [in]	type	プリミティブの種類
//!	@param [in]	vertexCount	頂点数
//!	@param [in]	vertexOffet	頂点番号のオフセット
//---------------------------------------------------------------------------
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset = 0);

//---------------------------------------------------------------------------
//! @brief インデックス描画
//!	@param [in]	type	プリミティブの種類
//!	@param [in]	indexCount	インデックス数
//!	@param [in]	indexOffset インデックス番号のオフセット
//!	@param [in]	vertexOffet	頂点番号のオフセット
//---------------------------------------------------------------------------
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset = 0, u32 vertexOffset = 0);

//---------------------------------------------------------------------------
//! @brief 描画ターゲットの設定
//! @param [in] renderTarget レンダーターゲット
//! @param [in] depthStencil 深度ステンシルバッファ
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil = nullptr);

//---------------------------------------------------------------------------
//! @brief 描画ターゲットの設定(MRT用)
//! @param [in] renderTargetCount  マルチターゲットの数
//! @param [in] renderTargets      レンダーターゲット配列
//! @param [in] depthStencil       深度ステンシルバッファ
//---------------------------------------------------------------------------
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil);

//---------------------------------------------------------------------------
//! @brief 描画ターゲットのクリア
//! @param [in] target  クリアしたいバッファ
//! @param [in] color   クリアしたい色
//! @param [in] depth 　デプス     (よくわかってない、また聞きます)
//! @param [in] stencil ステンシル (よくわかってない、また聞きます)
//---------------------------------------------------------------------------
void clearTarget(ptr<gpu::Texture> target, const Vector4& color = Vector4(0.0f, 0.0f, 0.0f, 0.0f), f32 depth = 1.0f, u8 stencil = 0);
;

}   // namespace gpu
