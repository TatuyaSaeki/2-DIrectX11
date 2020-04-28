//===========================================================================
//! @file	psFilterGlare_Luminance.fx
//! @breif	グレアフィルター輝度抽出
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! RGBからYUV変換
//===========================================================================
float3	RGBtoYUV(float3 rgb)
{
	float3	yuv;
	yuv.x = dot(rgb, float3(0.299, 0.587, 0.114));
	yuv.y = dot(rgb, float3(-0.169, -0.331, 0.500));
	yuv.z = dot(rgb, float3(0.500, -0.419, -0.081));

	return yuv;
}

//===========================================================================
//! YUVからRGB変換
//===========================================================================
float3	YUVtoRGB(float3 yuv)
{
	float3	rgb;
	rgb.x = dot(yuv, float3(1.0, 0.000, 1.402));
	rgb.y = dot(yuv, float3(1.0, -0.344, -0.714));
	rgb.z = dot(yuv, float3(1.0, 1.772, 0.000));

	return rgb;
}

//===========================================================================
//! ピクセルシェーダー (テクスチャあり)
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float3	color = BaseTexture.Sample(LinearSampler, input.uv_).rgb;

	color = color / (1 + color);

	static const float THRESHOLD = 0.7;
//	static const float THRESHOLD = 100.0;
	float3	yuv = RGBtoYUV(color);

	yuv.x = max(0, yuv.x - THRESHOLD);
	//	yuv.x *= 2.0;	// 輝度スケール
	color *= yuv.x;// YUVtoRGB(yuv);

	return float4(color, 1.0);
}

