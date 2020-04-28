//===========================================================================
//!	@file	psDirectionalLight.fx
//!	@brief	平行光源
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー(平行光源)
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	//float4 baseColor = input.color_;
	float3 color     = 0.0;
	baseColor		 = pow(saturate(baseColor), 2.2);	// リニア空間に変換

	float3 wPos = input.worldPosition_;

	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0] - wPos);
	float3 L = normalize((dlPosition_.xyz) - wPos);
	float3 H = normalize(L + V);
	float3 R = reflect(-V, N);

	float roughness = 0.5;//RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float metalness = 0.5;//MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	// PBR 平行光源 
	Material materialPBR = directionalLight(roughness, metalness, N, L, V, H, baseColor.rgb);

	//float3 diffuse = diffuseLambertPBR(N, L);

	// IBL
	Material materialIBL = IBL(roughness, metalness, N, V, R, baseColor.rgb);

	// �J���[�v�Z
    float3 diffuseTerm  = dlColor_.rgb * baseColor.rgb * materialPBR.diffuse_.r;
	float3 specularTerm = dlColor_.rgb * materialPBR.specular_;
	
	//diffuseTerm += materialIBL.diffuse_;
	//specularTerm += materialIBL.specular_;

	// 最終カラー
	//		lerp = 線形補完(1,2,3)     引数3 = 引数1の割合
	color = lerp(diffuseTerm, specularTerm, metalness);

	// toneMapping適用済みなのでいらない
	//color = pow(saturate(color), 1.0 / 2.2);

	return float4(color,1.0);
}