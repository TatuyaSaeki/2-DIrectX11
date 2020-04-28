//===========================================================================
//!	@file	vsSphericalTexture.fx
//!	@brief	球面座標テクスチャ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// 頂点シェーダー出力結果はピクセルシェーダーに使われる

	//================================================================
	// 頂点の座標変換
	//================================================================
	float4	position = input.position_;

	position = mul(matWorld_, position);
	float3 worldPosition = position.xyz;

	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	// 法線のワールド変換
	float3 N = mul(matWorld_, float4(input.normal_, 0.0)).xyz;
	// カメラ座標に変換 球面座標用
	//float3 vN = mul(matView_[0], float4(N, 0.0)).xyz;

	//================================================================
	// 出力
	//================================================================
	output.position_ = position;		// 座標は投影後のxyzw座標値 (GPUによって頂点シェーダー実行後に自動でxyz÷wが実行される)
	output.normal_ = N;
	output.color_ = input.color_;
	output.uv_.x = N.x * 0.5 + 0.5;
	output.uv_.y = N.y * 0.5 + 0.5;
	output.worldPosition_ = worldPosition;

	return output;
}