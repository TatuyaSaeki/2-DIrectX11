//===========================================================================
//!	@file	psSeaNormal.fx
//! @brief	海用動的法線計算
//===========================================================================
#include "../shader/header/shader.h"

static const float2 shiftX = { (1.0f + 0.0f) / 128.0f, (0.0f + 0.0f) / 128.0f };
static const float2 shiftZ = { (0.0f + 0.0f) / 128.0f, (1.0f + 0.0f) / 128.0f };
static const float2 shift3 = { (-1.0f + 0.0f) / 128.0f, (0.0f + 0.0f) / 128.0f };
static const float2 shift4 = { (0.0f + 0.0f) / 128.0f, (-1.0f + 0.0f) / 128.0f };

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 color = 1.0;

	float3 texX = (2.0*NormalTexture.Sample(LinearSampler, input.uv_ + shiftX) - 1.0).rgb;
	float3 texZ = (2.0*NormalTexture.Sample(LinearSampler, input.uv_ + shiftZ) - 1.0).rgb;
	float3 tex3 = (2.0*NormalTexture.Sample(LinearSampler, input.uv_ + shift3) - 1.0).rgb;
	float3 tex4 = (2.0*NormalTexture.Sample(LinearSampler, input.uv_ + shift4) - 1.0).rgb;

	float3 du = { 1,0.5*(texX.x - tex3.x),0 };
	float3 dv = { 0,0.5*(texZ.x - tex4.x),1 };

	color.xyz = 0.5*normalize(cross(du, dv)) + 0.5;

	return color;
}