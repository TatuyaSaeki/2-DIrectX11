//===========================================================================
//!	@file	psCubeMapShadow.fx
//!	@brief	キューブマップ影
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 albedo = BaseTexture.Sample(LinearSampler, input.uv_) + 0.1;
	float3 finalColor = 0.0; // 最終カラー
	albedo = pow(saturate(albedo), 2.2);	// リニア空間に変換

	float3 worldPosition = input.worldPosition_;
	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0] - worldPosition);
	float3 L = normalize(dlLookAt_.xyz - dlPosition_.xyz);
	float3 H = normalize(L + V);
	float3 R = reflect(-V, N);

	float len = length(L);

	Material plMaterial = pointLight(0.5, 0.5, N, L, V, H, len, albedo.rgb);

	finalColor = plMaterial.diffuse_ + plMaterial.specular_;

	float texLightLen;

	float4 plDirection = 0.0;
	plDirection.xyz = plPosition_[0].xyz - worldPosition;
	float distance = length(plDirection);
	plDirection.xyz = plDirection.xyz / distance;

	texLightLen = cubeDepthTexture.Sample(LinearSampler, -plDirection.xyz).r;

	if (texLightLen + 0.0001 < distance)
	{
		finalColor /= 3;
	}

	return float4(finalColor,1.0);
}