//===========================================================================
//!	@file	psRoundShadow.fx
//!	@brief	丸影
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float3 color = 0;
	float3 baseColor = float3(1, 1, 1);//PS_Texture(input).rgb;
	float3 N = normalize(input.normal_);
	float3 worldPosition = input.worldPosition_;
	float3 lightVector = normalize(dLVector[0].xyz);
	float3 ambient = float3(0.01, 0.01, 0.01);

	// 拡散反射
	float3 diffuse = diffuseLambert(N, lightVector);

	color += diffuse + ambient;
	color *= 0.5; // 明るすぎるから

	//if (modelCount == 1) color = float3(1,0,0);
	//if (modelCount == 2) color = float3(0, 1, 0);

	for (int i = 0; i < modelCount; ++i) {
		float3 vToCasater = normalize(modelPosition[i].xyz - worldPosition);
		float3 vCasterToLight = lightVector;

		float cos = saturate(dot(vToCasater, vCasterToLight));
		if (cos > 0.99) {
			color *= pow(cos / 3, 12 * (cos - 0.98));
		}
	}

	return float4(color, 1.0);
}






















//------------------------------------------------------------------------------------------------
// VS(シャドウマップ 深度テクスチャ作成)
//------------------------------------------------------------------------------------------------
PS_INPUT VS_Shadowmap_Depth(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 position = input.position_;

	position = mul(matWorld_, position);
	position = mul(matLight, position);
	position = mul(matProj_[0], position);

	output.position_ = position;

	return output;
}

// テクスチャ行列
static const matrix matTexture =
{
	0.5,  0.0, 0.0, 0.0,
	0.0, -0.5, 0.0, 0.0,
	0.0,  0.0, 1.0, 0.0,
	0.5,  0.5, 0.0, 1.0
};

//------------------------------------------------------------------------------------------------
// VS(シャドウマップ 深度テクスチャ作成)
//------------------------------------------------------------------------------------------------
PS_INPUT_SHADOW VS_Shadowmap(VS_INPUT input)
{
	PS_INPUT_SHADOW output = (PS_INPUT_SHADOW)0;

	float4 position = input.position_;
	float4 lightViewPosition = 0;
	float4 worldPosition = 0;

	position = lightViewPosition = worldPosition = mul(matWorld_, position);
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	lightViewPosition = mul(matLight, lightViewPosition);
	lightViewPosition = mul(matProj_[0], lightViewPosition);

	float3 normal = input.normal_;
	normal = mul(matWorld_, float4(normal, 0)).xyz;

	output.position_ = position;
	output.normal_ = normal;
	output.color_ = input.color_;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition.xyz;

	output.lightViewPosition_ = lightViewPosition;
	output.shadowTexCoord_ = mul(matTexture, lightViewPosition);

	return output;
}

//------------------------------------------------------------------------------------------------
// PS(シャドウマップ 深度テクスチャ作成)
//------------------------------------------------------------------------------------------------
float4 PS_Shadowmap(PS_INPUT_SHADOW input) : SV_Target
{
	float4 baseColor = input.color_;
	float3 color = 0.0;

	// 拡散反射
	float3 N = normalize(input.normal_);
	float3 L = normalize(dLVector[0].xyz);
	float  diffuse = diffuseLambert(N, L);
	float3 diffuseTerm = baseColor.rgb * diffuse;// +float3(0.1, 0.1, 0.1);

	color += diffuseTerm;

	// 影の処理
	//float2 texCoord			= input.shadowTexCoord_.xy / input.shadowTexCoord_.w;	// 同時座標系に変換
	//float depthValue		= DepthTexture.Sample(DepthSampler, texCoord.xy).r;		// 深度テクスチャからZ値を取得
	//float lightDepthValue	= input.lightViewPosition_.z / input.lightViewPosition_.w;	// ライトビューでのZ値
	//
	//color = (lightDepthValue < depthValue) ? 0 : color;


//	float shadwoThreshold = 1.0;
//	float3 shadowColor = float3(0.25,0.25,0.25);
//	shadwoThreshold = DepthTexture.Sample(DepthSampler, texCoord.xy).r;		// 深度テクスチャからZ値を取得
//	shadowColor = lerp(shadowColor,float3(1,1,1), shadwoThreshold);
//	color = shadowColor;


	float4 shadowPosition = mul(matLight, float4(input.worldPosition_,1.0));
	shadowPosition = mul(matProj_[0], shadowPosition);
	float2 shadowUv = shadowPosition.xy * float2(0.5, -0.5) + float2(0.5, 0.5);

	float depth = DepthTexture.Sample(LinearSampler, shadowUv).r;
	float shadow = (depth < shadowPosition.z - 0.0001) ? 0.0 : 1.0;
	color *= shadow;

	return float4(color, 1);
}