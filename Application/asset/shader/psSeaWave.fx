//===========================================================================
//!	@file	psSeaWave.fx
//!	@brief	海用波動的計算
//===========================================================================
#include "../shader/header/shader.h"

static const float2 shift0 = { 0.0f / 128.0f,  0.0f / 128.0f };
static const float2 shift1 = { 1.0f / 128.0f,  0.0f / 128.0f };
static const float2 shift2 = { 0.0f / 128.0f,  1.0f / 128.0f };
static const float2 shift3 = { -1.0f / 128.0f,  0.0f / 128.0f };
static const float2 shift4 = { 0.0f / 128.0f, -1.0f / 128.0f };

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 color = 1.0;

	float3 tex0 = BaseTexture.Sample(LinearSampler, input.uv_ + shift0).rgb*SubTexture.Sample(LinearSampler, input.uv_ + shift0).rgb;
	float3 tex1 = BaseTexture.Sample(LinearSampler, input.uv_ + shift1).rgb*SubTexture.Sample(LinearSampler, input.uv_ + shift1).rgb;
	float3 tex2 = BaseTexture.Sample(LinearSampler, input.uv_ + shift2).rgb*SubTexture.Sample(LinearSampler, input.uv_ + shift2).rgb;
	float3 tex3 = BaseTexture.Sample(LinearSampler, input.uv_ + shift3).rgb*SubTexture.Sample(LinearSampler, input.uv_ + shift3).rgb;
	float3 tex4 = BaseTexture.Sample(LinearSampler, input.uv_ + shift4).rgb*SubTexture.Sample(LinearSampler, input.uv_ + shift4).rgb;

	color.x = tex0.x + tex0.y - 0.5f;								// 高さ変化
	color.y = tex0.g												// 元の速度
				+ tex1.r + tex2.r + tex3.r + tex4.r - 4.0f*tex0.r; // 加速度
	color.z = 0;

	return color;
}