//===========================================================================
//! @file	psFilterGlare.fx
//! @breif	グレアフィルター
//===========================================================================
#include "../shader/header/shader.h"

Texture2D Texture1 : register(t0);
Texture2D Texture2 : register(t1);
Texture2D Texture3 : register(t2);
Texture2D Texture4 : register(t3);
Texture2D Texture5 : register(t4);
Texture2D Texture6 : register(t5);

//===========================================================================
//! テクスチャをエルミート補間サンプリング
//===========================================================================
float4 HermiteSample(Texture2D t, SamplerState s, float2 uv, float2 resolution)
{
#if 1
	float2	iuv = uv * resolution + 0.5;

	float2	f = frac(iuv);
	float2	i = floor(iuv);

	f = smoothstep(0.0, 1.0, f);

	uv = (i + f - 0.5) / resolution;
#endif
	return t.Sample(s, uv);
}


//===========================================================================
//! ピクセルシェーダー (テクスチャあり)
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 color = 0;

	float2	offset = float2(-0.5, -0.5);

	color += Texture1.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 2);
	color += Texture2.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 4);
	color += Texture3.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 8);
	color += Texture4.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 16);
	color += Texture5.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 32);
	color += Texture6.Sample(ClampSampler, input.uv_ + offset * float2(1.0 / 1980, 1.0 / 1080) * 64);

	return color;
}
