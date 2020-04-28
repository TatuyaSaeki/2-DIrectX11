//===========================================================================
//!	@file	vsShadowMap.fx
//! @brief	シャドウマップ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT_SHADOW main(VS_INPUT input)
{
	PS_INPUT_SHADOW output = (PS_INPUT_SHADOW)0;

	float4 position = input.position_;

	//=============================================================
	// 頂点の座標変換
	//=============================================================
	float3 worldPosition = 0;
	position = mul(matWorld_, position);
	worldPosition = position.xyz;
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	// シャドウ用座標変換
	float4 shadowPosition = float4(worldPosition, 1.0);
	shadowPosition = mul(matLightView_, shadowPosition);
	shadowPosition = mul(matLightProj_, shadowPosition);

	// スクリーン座標をUV座標に変換
	shadowPosition.xy = shadowPosition.xy * float2(0.5, -0.5) + 0.5;

	//=============================================================
	// 座標変換
	//=============================================================

	// 法線のワールド変換,正規化
	float3 normal = normalize(mul(matWorld_, float4(input.normal_, 0.0))).xyz;

	// 接線ワールド変換
	float3 tangent = mul(matWorld_, float4(input.tangent_, 0.0)).xyz;

	// 従法線ワールド変換
	float3 binormal = mul(matWorld_, float4(input.binormal_, 0.0)).xyz;

	output.position_ = position;
	output.normal_ = normal;
	output.tangent_ = tangent;
	output.binormal_ = binormal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition;
	output.shadowMapUv_ = shadowPosition;

	return output;
}