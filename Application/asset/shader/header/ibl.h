//===========================================================================
//!	@file	ibl.h
//!	@brief	IBL�p�֐�
//===========================================================================

//!	IBL�v�Z�p�֐�
//! @param	[in]	roughness	�\�ʂ̑e��
//! @param	[in]	metalness	������
//! @param	[in]	N			���K���@��
//! @param	[in]	V			���K���J�����ւ̃x�N�g��
//! @param	[in]	R			reflect(-V, N);
//! @param	[in]	baseColor	�F
//!	@return	�f�B�t�[�Y�A�X�y�L�����[����
Material IBL(float roughness, float metalness, float3 N, float3 V, float3 R, float3 baseColor)
{
	Material material;

	float3 iblDiffuse = iblDiffuseTexture.Sample(LinearSampler, R).rgb;
	float3 iblSpecular = iblSpecularTexture.SampleLevel(LinearSampler, R, roughness * 7.0).rgb;

	float  NdotV = max(dot(N, V), 0.00001);
	float3 specularColor = lerp(float3(0.04, 0.04, 0.04), baseColor, metalness);

	// Environment BRDF (�e�����ɂ���ăt���l���l���قȂ邽�߂̕␳��)
	// SIGGRAPH 2015 "Optimizing PBR" ���
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