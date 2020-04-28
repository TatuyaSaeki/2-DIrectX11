//===========================================================================
//!	@file	psSea.fx
//!	@brief	海用ピクセルシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(DS_INPUT input) : SV_Target
{
	float4 baseColor = NormalTexture.Sample(LinearSampler, input.uv_);
	float3 finalColor = 0.0;

	float roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	float3 worldPos = input.worldPosition_;
	float3 N = (NormalTexture.SampleLevel(LinearSampler, input.uv_, 0) * 2 - 1).xyz;
	float3 V = normalize(cameraPosition_[0] - worldPos);
	float3 L = normalize(dlPosition_.xyz - dlLookAt_.xyz);
	float3 H = normalize(L + V);
	float3 R = reflect(-V, N);

	Material dlMaterial = directionalLight(0.6, 0.8, N, L, V, H, baseColor.rgb);

	float3 diffuseTerm = baseColor.rgb * dlMaterial.diffuse_ * dlColor_.rgb;
	float3 specularTerm = dlMaterial.specular_ * dlColor_.rgb;

	Material materialIBL = IBL(0.6, 0.8, N, V, R, baseColor.rgb);
	diffuseTerm += materialIBL.diffuse_;
	specularTerm += materialIBL.specular_;

	float f = F_fresnel(0.5, dlMaterial.diffuse_.x);

	finalColor = lerp(diffuseTerm, specularTerm, f);

	return float4(finalColor, 1);
}