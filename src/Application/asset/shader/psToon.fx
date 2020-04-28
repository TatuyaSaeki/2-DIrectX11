//===========================================================================
//!	@file	psToon.fx
//!	@brief	トゥーンシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT_SHADOW input)
: SV_Target
{
    float4 baseColor  = BaseTexture.Sample(LinearSampler, input.uv_);
    float3 finalColor = 0.0;                             // �ŏI�J���[
    baseColor         = pow(saturate(baseColor), 2.2);   // ���j�A��Ԃɕϊ�

    float3 worldPosition = input.worldPosition_;
    float3 N             = normalMapping(input);
    float3 V             = normalize(cameraPosition_[0] - worldPosition);
    float3 L             = normalize(dlPosition_.xyz - dlLookAt_);   // �����̕��� L
    float3 H             = normalize(L + V);
    float3 R             = reflect(-V, N);

    float roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
    float metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;
    float AO        = AoTexture.Sample(LinearSampler, input.uv_).r;

    Material material;
    material.diffuse_  = 0;
    material.specular_ = 0;

    //=============================================================
	// 平行光源
    //=============================================================
    Material dlMaterial;
    dlMaterial.diffuse_  = 0;
    dlMaterial.specular_ = 0;

    dlMaterial = directionalLight(roughness, metalness, N, L, V, H, baseColor.rgb);

	float toonColor		= ToonTexture.Sample(LinearSampler, float2(dlMaterial.diffuse_.r, 1.0)).r;
    material.diffuse_	= (dlColor_.rgb * dlIntensity_) * dlMaterial.diffuse_;
    material.specular_	= (dlColor_.rgb * dlIntensity_) * dlMaterial.specular_;

	//=============================================================
	// 影
	//=============================================================
#if 1
    static const int   SAMPLE_COUNT = 16;
    static const float scale        = 4.0;

    float phi = rand(input.position_.xy) * 2.0 * PI;

    float shadow = 0.0;
    for(int k = 0; k < SAMPLE_COUNT; k++) {
        float2 uv = input.shadowMapUv_.xy;
        uv += vogelDiskSample(k, SAMPLE_COUNT, phi) * float2(1.0 / 4096.0, 1.0 / 4096.0) * scale;
        shadow += shadowSample(float3(uv, input.shadowMapUv_.z));
    }
    shadow *= 1.0 / SAMPLE_COUNT;

	// 通常の陰影と合成
    shadow = min(shadow, dlMaterial.diffuse_.r);

    material.diffuse_ *= shadow;
    material.specular_ *= shadow;
#endif

    //=============================================================
   // 点光源
    //=============================================================
    for(int i = 0; i < plCount_; ++i) {
        L         = plPosition_[i].xyz - worldPosition;
        float len = length(L);
        L         = normalize(L);
        H         = normalize(L + V);

        Material plMaterial = pointLight(roughness, metalness, N, L, V, H, len, baseColor.rgb);
        // toon
        float NdotL = diffuseLambertPBR(N, L).r;
        toonColor   = ToonTexture.Sample(LinearSampler, float2(plMaterial.diffuse_.r, 1.0)).r;

        material.diffuse_ += (plColor_[i].rgb * plIntensity_[i].x) * plMaterial.diffuse_;
        material.specular_ += (plColor_[i].rgb * plIntensity_[i].x) * plMaterial.specular_;
    }

    //=============================================================
	// スポットライト
    //=============================================================
    for(int j = 0; j < slCount_; ++j) {
        float3 L2 = 0;
        L         = (slPosition_[j].xyz) - worldPosition;
        L2        = normalize(slLookAt_[j].xyz - slPosition_[j].xyz);

        float len = length(L);
        L         = normalize(L);
        H         = normalize(L + V);

        Material slMaterial = spotLight(roughness, metalness, N, L, V, H, len, baseColor.rgb, L2);
        // toon
        float NdotL = diffuseLambertPBR(N, L).r;
        toonColor   = ToonTexture.Sample(LinearSampler, float2(slMaterial.diffuse_.r, 1.0)).r * 50.0;

        material.diffuse_ += (slColor_[j].rgb * slIntensity_[j].x) * slMaterial.diffuse_;
        material.specular_ += (slColor_[j].rgb * slIntensity_[j].x) * slMaterial.specular_;
    }

    //=============================================================
	// IBL
	//=============================================================
#if 1

    Material materialIBL = IBL(roughness, metalness, N, V, R, baseColor.rgb);
    {
        float iblScale = 0.5;
        material.diffuse_ += materialIBL.diffuse_ * iblScale * dlIntensity_;
        material.specular_ += materialIBL.specular_ * iblScale * metalness;
    }

#endif

	// AO適用
    float3 NdotV = max(dot(N, V), 0.00001);
    material.diffuse_ *= AO;                                                  // AmbientOcclusion
    material.specular_ *= saturate((NdotV + AO) * (NdotV + AO) - 1.0 + AO);   // SpecularOvvlustion�ߎ�(from Tri-ace)

    //=============================================================
	// 最終合成
    //=============================================================
    float3 diffuseTerm  = material.diffuse_ * baseColor.rgb * (toonColor*30);
    float3 specularTerm = material.specular_ * toonColor;

	//		lerp = 線形補完(1,2,3)     引数3 = 引数1の割合
    finalColor = lerp(diffuseTerm, specularTerm, metalness);
    //finalColor = diffuseTerm + specularTerm;

    return float4(finalColor, 1.0);
}
