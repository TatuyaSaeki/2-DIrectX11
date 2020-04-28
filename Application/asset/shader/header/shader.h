//===========================================================================
//!	@file	shader.h
//!	@brief	HLSLソースコード
//===========================================================================
#include "../shader/header/constant_buffer.h"
#include "../shader/header/ibl.h"
#include "../shader/header/pbr.h"

//---------------------------------------------------------------------------
//! Lambert拡散反射
//!	@param	[in]	N	正規化法線
//!	@param	[in]	L	正規化ライトへのベクトル
//!	@return	ディフーズ値
//---------------------------------------------------------------------------
float3 diffuseLambert(float3 N, float3 L)
{
    return saturate(dot(N, L));
}

//---------------------------------------------------------------------------
//! blinn-Phong鏡面反射
//!	@param	[in]	N			正規化法線
//!	@param	[in]	L			正規化ライトへのベクトル
//!	@param	[in]	V			正規化カメラへのベクトル
//!	@param	[in]	intensity	ライトの強さ
//!	@return	スペキュラー度合い
//---------------------------------------------------------------------------
float specularBlinnPhong(float3 N, float3 L, float3 V, float intensity)
{
    float3 H = normalize(L + V);

    return pow(saturate(dot(H, N)), intensity);
}

//---------------------------------------------------------------------------
//! 平行光源
//!	@param	[in]	roughness	粗さ
//!	@param	[in]	metalness	金属度
//!	@param	[in]	N			正規化法線
//!	@param	[in]	L			正規化ライトへのベクトル
//!	@param	[in]	V			正規化カメラへのベクトル
//!	@param	[in]	H			L,Vのハーフベクトル
//!	@param	[in]	baseColor	元となる色
//!	@return	ディフーズ、スペキュラー度合い
//---------------------------------------------------------------------------
Material directionalLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float3 baseColor)
{
    Material returnMaterial;

    // Lambert拡散反射
    float3 diffuse = diffuseLambertPBR(N, L);
    // CookTorrance鏡面反射
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    returnMaterial.diffuse_  = diffuse;
    returnMaterial.specular_ = specular;

    return returnMaterial;
}
//---------------------------------------------------------------------------
//! 点光源
//!	@param	[in]	roughness	粗さ
//!	@param	[in]	metalness	金属度
//!	@param	[in]	N			正規化法線
//!	@param	[in]	L			正規化ライトへのベクトル
//!	@param	[in]	V			正規化カメラへのベクトル
//!	@param	[in]	H			L,Vのハーフベクトル
//!	@param	[in]	len			オブジェクトとライトの距離
//!	@return	ディフーズ、スペキュラー度合い
//---------------------------------------------------------------------------
Material pointLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float len, float3 baseColor)
{
    Material returnMaterial;

    float3 diffuse  = diffuseLambertPBR(N, L);
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    float attenuation = saturate(1.0 / (0.0 * 0.0 * len + 0.1 * len * len));

    returnMaterial.diffuse_  = diffuse * attenuation;
    returnMaterial.specular_ = specular * attenuation;

    return returnMaterial;
}

//---------------------------------------------------------------------------
//! スポットライト
//!	@param	[in]	roughness	粗さ
//!	@param	[in]	metalness	金属度
//!	@param	[in]	N			正規化法線
//!	@param	[in]	L			正規化ライトへのベクトル
//!	@param	[in]	V			正規化カメラへのベクトル
//!	@param	[in]	H			L,Vのハーフベクトル
//!	@param	[in]	len			オブジェクトとライトの距離
//!	@param	[in]	L2			正規化オブジェクトとライトの距離
//!	@return	ディフーズ、スペキュラー度合い
//---------------------------------------------------------------------------
Material spotLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float len, float3 baseColor, float3 L2 = 0)
{
    Material returnMaterial;

    float3 diffuse  = diffuseLambertPBR(N, -L);
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    float attenuation = saturate(1.0 / (0.0 * 0.0 * len + 0.1 * len * len));

    returnMaterial.diffuse_  = diffuse * attenuation;
    returnMaterial.specular_ = specular * attenuation;

    // 範囲
    float cos = saturate(dot(L, -L2));
    if(cos < 0.95) {
        returnMaterial.diffuse_ *= pow(cos / 4.0, 30.0 * (0.95 - cos));
    }
    returnMaterial.diffuse_ *= 8;

    return returnMaterial;
}

//---------------------------------------------------------------------------
//! シャドウ用関数
//!	@param	[in]	shadowmapUv		UV値	
//!	@return	その場所のピクセルカラー
//---------------------------------------------------------------------------
float shadowSample(float3 shadowmapUv)
{
    float shadowDepth = DepthTexture.Sample(LinearSampler, shadowmapUv.xy).r;

    float              shadow      = 1.0;
    static const float SHADOW_BIAS = 0.00005;

    shadow = (shadowDepth + SHADOW_BIAS < shadowmapUv.z) ? 0.01 : 1.0;

    return shadow;
}

//---------------------------------------------------------------------------
//! 乱数生成関数
//!	@param	[in]	st	
//!	@return	乱数
//---------------------------------------------------------------------------
float rand(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

//---------------------------------------------------------------------------
//! vogelDisk
//!	@param	[in]	sampleIndex	
//!	@param	[in]	sampleCount
//!	@param	[in]	phi	
//!	@return	
//---------------------------------------------------------------------------
float2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float goldenAngle = 2.4;   // 黄金角

    float r     = sqrt(sampleIndex + 0.5) / sqrt(samplesCount);
    float theta = sampleIndex * goldenAngle + phi;

    float sine, cosine;
    sincos(theta, sine, cosine);

    return float2(r * cosine, r * sine);
}

//-------------------------------------------------------------
//! 法線マッピング
//!	@param	[in]	input	PS_INPUT_SHADOW入力情報
//! @return	計算済み法線
//-------------------------------------------------------------
float3 normalMapping(PS_INPUT_SHADOW input)
{
    float3 normalTexture = NormalTexture.Sample(LinearSampler, input.uv_).rgb;
    normalTexture        = normalize(normalTexture * 2.0 - 1.0);

    // 法線ベクトルの回転行列を作成
    float3 N = normalize(input.normal_);
    float3 T = normalize(input.tangent_);
    float3 B = normalize(input.binormal_);

    float3x3 TBN = { T, B, N };
    //TBN = transpose(TBN);	// 逆行列作成
    N = mul(normalTexture, TBN);

    return normalize(N);
}
