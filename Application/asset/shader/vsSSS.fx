//===========================================================================
//!	@file	vsSSS.fx
//!	@brief	SSS
//===========================================================================
#include "../asset/shader/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT_SHADOW main(VS_INPUT input)
{
	PS_INPUT_SHADOW output = (PS_INPUT_SHADOW)0;

	float4 position = input.position_;
	float4 lightViewPosition = 0;
	float4 worldPosition = 0;

	position = lightViewPosition = worldPosition = mul(matWorld_, position);
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	lightViewPosition = mul(matLightView, lightViewPosition);
	lightViewPosition = mul(matLightProj, lightViewPosition);

	float3 normal = input.normal_;
	normal = mul(matWorld_, float4(normal, 0)).xyz;

	output.position_ = position;
	output.normal_ = normal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition.xyz;

	output.lightViewPosition_ = lightViewPosition;
	output.shadowTexCoord_ = mul(matTexture, lightViewPosition);

	return output;
}