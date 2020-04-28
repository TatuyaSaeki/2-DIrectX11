//===========================================================================
//!	@file	vsEdge.fx
//!	@brief	エッジ描画(法線膨らませてるだけ)
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 position = input.position_;
	float4 worldPosition = 0;

	float3 normal = input.normal_;
	normal = mul(matWorld_, float4(normal, 0)).xyz;

	position.xyz += normalize(normal) * 0.01;

	position = worldPosition = mul(matWorld_, position);
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	output.position_ = position;
	output.normal_ = normal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition.xyz;

	return output;
}