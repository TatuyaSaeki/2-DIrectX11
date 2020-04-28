//===========================================================================
//!	@file	psSea3.fx
//!	@brief	海用ピクセルシェーダー3
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = HeightTexture1.Sample(LinearSampler, input.uv_);
	return baseColor;

	input.grapPosition_.xy /= input.grapPosition_.w;

	float3 N = 0;
	{
		float3 normalTexture = NormalTexture1.Sample(LinearSampler, input.uv_).rgb;
		normalTexture = normalize(normalTexture * 2.0 - 1.0);

		// 法線ベクトルの回転行列を作成
		float3 N = normalize(input.normal_);
		float3 T = normalize(input.tangent_);
		float3 B = normalize(input.binormal_);

		float3x3 TBN = { T, B, N };
		//TBN = transpose(TBN);	// �t�s��쐬
		N = mul(normalTexture, TBN);

		//return float4(N, 1);
	}

	float3 worldPosition = input.worldPosition_;
	float3 V = normalize(cameraPosition_[0] - worldPosition);
	float3 L = normalize(dlPosition_.xyz - dlLookAt_);  // 光源の方向 L
	float3 H = normalize(L + V);

	Material dlMaterial = directionalLight(0.5, 0.5, N, L, V, H, baseColor.rgb);

	dlMaterial.diffuse_ = (dlColor_.rgb * dlIntensity_) * dlMaterial.diffuse_;

	return float4(dlMaterial.diffuse_.rgb, 1.0);
}