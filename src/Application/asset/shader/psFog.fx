//===========================================================================
//!	@file	psFog.fx
//!	@brief	霧
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	float4 color = 0.0;

	float z = input.position_.z * input.position_.w;	// カメラからの距離
	float f = 0.0;
#if 0
	// 線形
	float start = 0.0;
	float end = 10.0;
	f = saturate((end - z) / (end - start));
#else 
	// 指数
	// こっちは境界線が見えない
	float density = 0.06;
	float end = 1.7354;	// 上げると濃くなる
	f = saturate(pow(end, -z * density));
#endif
	//						(1.0 - f) = これがないと霧に色がつかない
	color = f * baseColor + (1.0 - f) * float4(1.0, 1.0, 1.0, 1.0);

	return color;
}