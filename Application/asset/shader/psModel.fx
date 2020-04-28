//===========================================================================
//!	@file	psModel.fx
//!	@brief	モデル描画(PBR,IBL)
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT_SHADOW input) : SV_Target
{
	float4 albedo = BaseTexture.Sample(LinearSampler, input.uv_);
	albedo = pow(saturate(albedo), 2.2);	// リニア空間に変換

	float3 color = albedo.rgb;
	float3 finalColor = 0.0;

	float3 worldPosition = input.worldPosition_;
	float3 N = normalize(input.normal_);
	float3 V = normalize(cameraPosition_[0] - worldPosition);
	float3 L = normalize(dlLookAt_ - dlPosition_.xyz);
	float3 H = normalize(L + V);
	float3 R = reflect(-V, N);

	//=============================================================
	// 影
	//=============================================================
	float shadow = 0.0;

	shadow = shadowSample(input.shadowMapUv_.xyz);
	color = shadow;

	//=============================================================
	// ライト
	//=============================================================

	float roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	// PBR
	Material materialPBR = directionalLight(roughness, metalness, N, -L, V, H, albedo.rgb);
	// IBL
	Material materialIBL = IBL(roughness, metalness, N, V, R, albedo.rgb);

	// カラー計算
	float3 diffuseTerm = dlColor_.rgb * albedo.rgb * materialPBR.diffuse_;
	float3 specularTerm = dlColor_.rgb * materialPBR.specular_;

	// IBL追加
	diffuseTerm += materialIBL.diffuse_;
	specularTerm += materialIBL.specular_;

	// 最終カラー
	finalColor = lerp(diffuseTerm, specularTerm, metalness);

	finalColor *= color;

	// リニア空間戻す(Engineでやってる)
	//finalColor = pow(saturate(finalColor), 1.0 / 2.2);

	return float4(finalColor, 1.0);
}