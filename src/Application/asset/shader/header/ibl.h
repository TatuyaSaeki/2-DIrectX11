//===========================================================================
//!	@file	ibl.h
//!	@brief	IBL用関数
//===========================================================================

//!	IBL計算用関数
//! @param	[in]	roughness	表面の粗さ
//! @param	[in]	metalness	金属質
//! @param	[in]	N			正規化法線
//! @param	[in]	V			正規化カメラへのベクトル
//! @param	[in]	R			reflect(-V, N);
//! @param	[in]	baseColor	色
//!	@return	ディフーズ、スペキュラー結果
Material IBL(float roughness, float metalness, float3 N, float3 V, float3 R, float3 baseColor)
{
	Material material;

	float3 iblDiffuse = iblDiffuseTexture.Sample(LinearSampler, R).rgb;
	float3 iblSpecular = iblSpecularTexture.SampleLevel(LinearSampler, R, roughness * 7.0).rgb;

	float  NdotV = max(dot(N, V), 0.00001);
	float3 specularColor = lerp(float3(0.04, 0.04, 0.04), baseColor, metalness);

	// Environment BRDF (各方向によってフレネル値が異なるための補正式)
	// SIGGRAPH 2015 "Optimizing PBR" より
	float3 environmentBRDF = 1 - max(roughness, NdotV);
	environmentBRDF = environmentBRDF * environmentBRDF * environmentBRDF + specularColor;


	iblSpecular *= environmentBRDF;
	iblDiffuse *= baseColor;
	//iblDiffuse *= baseColor.rgb * (1 - metalness);
	//iblSpecular *= environmentBRDF;

	material.diffuse_ = iblDiffuse.x;
	material.specular_ = iblSpecular;

	return material;
}