//===========================================================================
//!	@file	psToneMap.fx
//!	@brief	トーンマッピング
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! トーンマッピングに必要
//===========================================================================
static const float A = 0.15;
static const float B = 0.50;
static const float C = 0.10;
static const float D = 0.20;
static const float E = 0.02;
static const float F = 0.30;
static const float W = 11.2;

//===========================================================================
//! �A���`���[�f�b�h2�ō̗p����Ă��g�[���}�b�s���O
//===========================================================================
float3 Uncharted2Tonemap(float3 x)
{
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

//===========================================================================
//! ACESFilm
//===========================================================================
float3 ACESFilm(float3 x)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float3 color = BaseTexture.Sample(LinearSampler, input.uv_).rgb;

#if 1
	// Uncharted2 Filmic Tonemapping
	// https://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting
	float ExposureBias = 2.0f;
	float3 curr = Uncharted2Tonemap(color * ExposureBias);

	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	color = curr * whiteScale;
#elif 0
	// ACES Filmic Tonemapping
	// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
	color = ACESFilm(color);
#else
	// Reinhard Tonemapping
	color = color / (1 + color);
#endif
	color = pow(saturate(color), 1.0 / 2.2);	// �K���}�␳

	return float4(color, 1);
}