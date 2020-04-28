//===========================================================================
//!	@file	psPointLight.fx
//!	@brief	点光源
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー(点光源)
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{

	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	float3 color     = 0;
	baseColor        = pow(saturate(baseColor), 2.2);	// リニア空間に変換

	float3 worldPosition = input.worldPosition_;

	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0] - worldPosition);
	float3 L = 0.0;
	float3 H = normalize(L + V);
	float3 R = reflect(-V, N);

	float roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	float len = 0.0;

	Material materialPBR;
	Material materialIBL;
	float3 diffuseTerm = 0.0;
	float3 specularTerm = 0.0;

	float3 lightPosition = 0.0;
	float3 lightColor = 0.0;

	for (int i = 0; i < plCount_; ++i) {

		lightPosition	= plPosition_[i].xyz;
		lightColor		= plColor_[i].rgb;

		L		= lightPosition - worldPosition;
		len		= length(L);
		L       = normalize(L);

		materialPBR = pointLight(roughness, metalness, N, L, V, H, len, baseColor.rgb);
		// カラー計算
		diffuseTerm += (lightColor.rgb * 20.0) * baseColor.rgb * materialPBR.diffuse_;
		specularTerm += lightColor.rgb * materialPBR.specular_;

		materialIBL = IBL(roughness, metalness, N, V, R, baseColor.rgb);
		diffuseTerm += materialIBL.diffuse_;
		specularTerm += materialIBL.specular_;
	}

	color += lerp(diffuseTerm, specularTerm, metalness);
	color = pow(saturate(color), 1.0 / 2.2);
	
	return float4(color.rgb, 1.0);
}