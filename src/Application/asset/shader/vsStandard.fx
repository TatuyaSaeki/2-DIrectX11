//===========================================================================
//!	@file	vsStandard.fx
//!	@brief	通常シェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//!	頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// 頂点シェーダー出力結果はピクセルシェーダーに使われる

	//=============================================================
	// 頂点の座標変換
	//=============================================================
	float4	position = input.position_;

	position = mul(matWorld_, position);
	float3 worldPosition = position.xyz;

	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	//// スクリーン座標をUV座標に変換
	//shadowPosition.xy = shadowPosition.xy * float2(0.5, -0.5) + 0.5;

	// 法線のワールド変換,正規化
	float3 normal = normalize(mul(matWorld_, float4(input.normal_, 0.0))).xyz;

	// 接線ワールド変換
	float3 tangent = mul(matWorld_, float4(input.tangent_, 0.0)).xyz;

	// 従法線ワールド変換
	float3 binormal = mul(matWorld_, float4(input.binormal_, 0.0)).xyz;

	//=============================================================
	// 出力
	//=============================================================
	output.position_ = position;		// 座標は投影後のxyzw座標値 (GPUによって頂点シェーダー実行後に自動でxyz÷wが実行される)
	output.normal_ = normal;
	output.tangent_ = tangent;
	output.binormal_ = binormal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;	    // UVを加工しない場合はそのまま代入でスルーさせる
	output.worldPosition_ = worldPosition;

	return output;
}
