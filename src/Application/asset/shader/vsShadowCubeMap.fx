//===========================================================================
//!	@file	vsShadowCubeMap.fx
//!	@brief	キューブマップシャドウ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 position = input.position_;

	//=============================================================
	// 頂点の座標変換
	//=============================================================
	float3 worldPosition = 0;
	position = mul(matWorld_, position);
	worldPosition = position.xyz;
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	//=============================================================
	// 法線の座標変換
	//=============================================================
	float3 normal = input.normal_;
	normal = mul(matWorld_, float4(normal, 0)).xyz;

	output.position_ = position;
	output.normal_ = normal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition;

	return output;
}