//===========================================================================
//!	@file	psThinFilm.fx
//!	@brief	薄膜シェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	float4 color	 = 0.0;

	float3 N	= normalize(input.normal_);
	float3 V	= normalize(cameraPosition_[0] - input.worldPosition_);
	float NdotV = saturate(dot(N, V));
	
	float4 thinColor = ThinTexture.Sample(LinearSampler, float2(NdotV, 0.0));
	color.r = (NdotV < 0.25) ? 1.0 : 0.0;
	color.b = (NdotV < 0.25) ? 1.0 : 0.0;
	color += baseColor;
	color += thinColor;

	return color;
}