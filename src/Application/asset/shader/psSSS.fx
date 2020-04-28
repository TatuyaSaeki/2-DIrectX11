//===========================================================================
//!	@file	psSSS.fx
//!	@brief	SSS
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT_SHADOW input) : SV_Target
{
	float4 baseColor = input.color_;
	float3 finalColor = pow(saturate(baseColor.rgb), 2.2);	// リニア空間に変換

	float3 worldPosition = input.worldPosition_;
	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0] - worldPosition);
	float3 L = normalize(-dLVector[0].xyz);

	//=============================================================
	// 拡散反射 & 鏡面反射
	//=============================================================
	float3 diffuse = diffuseLambertPBR(N, L) * dLColor[0].rgb * baseColor.rgb;
	float3 specular = specularBlinnPhongPBR(N, V, L) * dLColor[0].rgb;

	finalColor = lerp(diffuse, specular, 0.5);

	//=============================================================
	// SSS
	//=============================================================
	float lightIn = 0.0;
	float lightOut = input.position_.w;//input.lightViewPosition_.w;
	float2 texCoord = input.shadowTexCoord_.xy / input.shadowTexCoord_.w;
	lightIn = DepthTexture.Sample(LinearSampler, texCoord).r;

	float thickness = abs(lightOut - lightIn); // 厚み
	float3 transparent = float3(0.0,1.0,0.0) * (1.0 - saturate(thickness / 2.4));

	finalColor /= 4.0 + 2 * transparent * 3.0 / 4.0;

	// sRGBへ変換
	finalColor.rgb = pow(saturate(finalColor.rgb), 1.0 / 2.2);

	return float4(finalColor, 1.0);
}