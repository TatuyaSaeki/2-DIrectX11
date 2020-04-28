//===========================================================================
//!	@file	pbr.h
//!	@brief	PBR—pŠÖ”
//===========================================================================

//---------------------------------------------------------------------------
// NormalizeLambertŠgU”½Ë
//---------------------------------------------------------------------------
float3 diffuseLambertPBR(float3 N, float3 L)
{
	// max = 0œZ‰ñ”ğ
	return max(dot(N, L), 0.0001) * (1.0 / PI);
}

//---------------------------------------------------------------------------
// blinn-Phong‹¾–Ê”½Ë
//---------------------------------------------------------------------------
float specularBlinnPhongPBR(float roughness, float3 N, float3 L, float3 V)
{
	float3 H = normalize(L + V);

	float specularPower = exp2(10.0 * (1.0 - roughness) + 1.0);
	float normalizeFactor = (specularPower + 2.0) / (2.0 * PI);

	float specular = pow(saturate(dot(N, H)), specularPower) * normalizeFactor;

	return specular;
}

//---------------------------------------------------------------------------
// PBR—p
//---------------------------------------------------------------------------
float D_BlinnPhong(float roughness, float NdotH)
{
	float specularPower = exp2(10 * (1 - roughness) + 1);

	float normalizeFactor = (specularPower + 2) / (2 * PI);

	return pow(saturate(NdotH), specularPower) * normalizeFactor;
}

float D_GGX(float roughness, float NdotH)
{
	float alpha = roughness * roughness + 0.000001;

	float denom = NdotH * (alpha * alpha - 1.0) + 1.0;   // 0œZ‰ñ”ğ

	float result = alpha / denom;

	return result * result * (1.0 / PI);
}

float G_Smith_Schlick_GGX(float roughness, float NdotV, float NdotL)
{
	float alpha = roughness * roughness + 0.000001;

	//float k = alpha * 0.5;								// ‚à‚Æ‚Ì®
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;   // UE4
	float GV = NdotV / (NdotV * (1.0 - k) + k);
	float GL = NdotL / (NdotL * (1.0 - k) + k);

	return GV * GL;
}

//---------------------------------------------------------------------------
// Schlick‚ÌƒtƒŒƒlƒ‹‹ß—® (Fresnel approximation)
//---------------------------------------------------------------------------
float3 F_Schlick(float3 f0, float NdotV)
{
	return f0 + (1.0 - f0) * pow(1.0 - NdotV, 5.0);
}

//---------------------------------------------------------------------------
//	ƒtƒŒƒlƒ‹‹ß—®
//---------------------------------------------------------------------------
float F_fresnel(float metalness, float NdotV)
{
	// Schlick‚ÌƒtƒŒƒlƒ‹‹ß—®
	return metalness + (1.0 - metalness) * pow(1.0 - NdotV, 5.0);
}

//---------------------------------------------------------------------------
// ‹¾–Ê”½Ë CookTrrance
//---------------------------------------------------------------------------
float3 CookTorrance(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float3 baseColor)
{
	float NdotL = max(dot(N, L), 0.0001);   // 0œZ‰ñ”ğ
	//float NdotV = max(dot(N, V), 0.0001);

	float3 specularColor = lerp(float3(0.04, 0.04, 0.04), baseColor, metalness);
	float3 specular = 0.0;
#if 0
	float D = D_GGX(roughness, dot(N, H));						// ‘e‚³
	float G = G_Smith_Schlick_GGX(roughness, NdotV, NdotL);	// ‰e
	float F = F_fresnel(metalness, NdotV);					// ƒtƒŒƒlƒ‹
	specular = (D * G * F) / (4 * NdotV * NdotL) * NdotL;
#else

	float r4 = roughness * roughness * roughness * roughness + 0.000001;
	float NdotH = dot(N, H);
	float LdotH = dot(L, H);
	float LdotH2 = LdotH * LdotH + 0.000001;

	float denom = NdotH * NdotH * (r4 - 1.0) + 1.0;

	specular =				r4  
	/ //------------------------------------------------------
		(4.0 * PI * denom * denom * LdotH2 * (roughness + 0.5));

	specular *= specularColor * NdotL;
#endif

	return specular;
}