//===========================================================================
//!	@file	psReflectionMap.fx
//!	@brief	環境反射
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0].xyz - input.worldPosition_);
	float3 R = reflect(-V, N);
	//float3 R = refract(-V, N, 0.5);


	return cubeTexture.Sample(LinearSampler, R);// +BaseTexture.Sample(LinearSampler, input.uv_);
}