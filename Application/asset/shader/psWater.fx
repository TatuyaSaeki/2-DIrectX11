//===========================================================================
//!	@file	psWater.fx
//!	@brief	水用ピクセルシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(DS_INPUT input)
	: SV_Target
{
	//float4 baseColor = float4(0.0, 1.5, 2.8, 1.0);
	float4 baseColor = float4(1.0, 1.0, 1.0, 1.0);
	//float4 baseColor = float4(0,0,1, 1.0);
	baseColor = pow(saturate(baseColor), 2.2);   // リニア空間に変換

	float3   wPos = input.worldPosition_;
	float3   N = 0.0;
	float3   L = normalize(dlPosition_.xyz - wPos);
	float3   V = normalize(cameraPosition_[0] - wPos);
	float3   H = normalize(L + V);
	float3   R = reflect(-V, N);
	Material material;
	material.diffuse_ = 0.0;
	material.specular_ = 0.0;

	//=============================================================
	// 屈折用処理
	//=============================================================
	float2 grapPosition = input.grapPosition_.xy /= input.grapPosition_.w;

	//=============================================================
	// 高さ情報
	//=============================================================
	float2 uv1 = input.uv_;
	float2 uv2 = input.uv_;
	float2 uv3 = input.uv_;
	float2 uv4 = input.uv_;

	// uv移動
	uv1.x -= waveDistance_ * 0.04;
	uv1.y += waveDistance_ * 0.03;
	uv2.x += waveDistance_ * 0.09;
	uv2.y += waveDistance_ * 0.03;

	uv3.y += waveDistance_ * 0.1;
	uv4.x += waveDistance_ * 0.05;
	uv4.y += waveDistance_ * 0.07;

	// 法線テクスチャから取得
	float3 normalTexture1 = NormalTexture1.Sample(LinearSampler, uv1).rgb;
	float3 normalTexture2 = NormalTexture1.Sample(LinearSampler, uv2).rgb;
	float3 normalTexture3 = NormalTexture2.Sample(LinearSampler, uv3).rgb;
	float3 normalTexture4 = NormalTexture2.Sample(LinearSampler, uv4).rgb;
	{
		// バンプ用補正
		normalTexture1 = normalize(normalTexture1 * 2.0 - 1.0);
		normalTexture2 = normalize(normalTexture2 * 2.0 - 1.0);
		normalTexture3 = normalize(normalTexture3 * 2.0 - 1.0);
		normalTexture4 = normalize(normalTexture4 * 2.0 - 1.0);

		// 法線ベクトルの回転行列を作成
		N = normalize(input.normal_);
		float3 T = normalize(input.tangent_);
		float3 B = normalize(input.binormal_);

		float3x3 TBN = { T, B, N };
		float3   tmpN1 = normalize(mul(normalTexture1, TBN));
		float3   tmpN2 = normalize(mul(normalTexture2, TBN));
		float3   tmpN3 = normalize(mul(normalTexture3, TBN));
		float3   tmpN4 = normalize(mul(normalTexture4, TBN));
		N = lerp(tmpN1, tmpN2, 0.5);
		N = lerp(N, tmpN3, 0.9);
		N = lerp(N, tmpN4, 0.5);

		//N = lerp(lerp(tmpN3, tmpN1, 0.1), lerp(tmpN4, tmpN2, 0.1), 0.5);
	}

	float3 distortion1 = refract(-V, N, 0.05);
	float3 distortion2 = refract(-V, N, 0.05);
	//float3 distortion1 = N * 0.1  * 3;
	//float3 distortion2 = N * 0.05 * 3;
	float2 distortion = lerp(distortion1, distortion2, 0.5).xy;
	float3 grapColor = BaseTexture.Sample(ClampSampler, grapPosition.xy + N.xz * 0.10).rgb;
	//float3 grapColor   = BaseTexture.Sample(LinearSampler, grapPosition.xy + distortion*0 + N.xz * 0.2).rgb;
	//return float4(grapColor, 1);
	//float roughness = 0.04;
	//float metalness = 0.98;

	float roughness = 0.1;
	float metalness = 0.8;

	// ライト計算
	Material dlMaterial;
	{
		dlMaterial = directionalLight(roughness, metalness, N, L, V, H, baseColor.rgb);
		material.diffuse_ += dlColor_.rgb * dlMaterial.diffuse_ * dlIntensity_;
		material.specular_ += dlColor_.rgb * dlMaterial.specular_ * dlIntensity_;
	}
	material.diffuse_ += 4.5;

	float3 diffuseTerm = grapColor * material.diffuse_;
	float3 specularTerm = dlMaterial.specular_ * 100;

	// IBL
	Material materialIBL = IBL(0.8, 0.0, N, V, R, float3(0.5, 0.5, 0.5));
	{
		float iblScale = 3.0;
		diffuseTerm += materialIBL.diffuse_ * iblScale;
		specularTerm += materialIBL.specular_ * iblScale;
	}

	float3 finalColor = lerp(diffuseTerm, specularTerm, metalness);

	return float4(finalColor.rgb, 1.0);
}