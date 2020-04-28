//===========================================================================
//!	@file	dsWater.fx
//!	@brief	水用ドメインシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ドメインシェーダー
//===========================================================================
[domain("tri")] DS_INPUT main(HS_CONSTANT_TRI_INPUT input, float3                 UV
	: SV_DomaInLocation, const OutputPatch<HS_INPUT, 3> patch) {
	DS_INPUT output = (DS_INPUT)0;

	//=============================================================
	// 頂点情報計算
	//=============================================================
	float4 position = patch[0].position_ * UV.x + patch[1].position_ * UV.y + patch[2].position_ * UV.z;
	float2 uv = patch[0].uv_ * UV.x + patch[1].uv_ * UV.y + patch[2].uv_ * UV.z;
	float3 normal = patch[0].normal_ * UV.x + patch[1].normal_ * UV.y + patch[2].normal_ * UV.z;
	float3 tangent = patch[0].tangent_ * UV.x + patch[1].tangent_ * UV.y + patch[2].tangent_ * UV.z;
	float3 binormal = patch[0].binormal_ * UV.x + patch[1].binormal_ * UV.y + patch[2].binormal_ * UV.z;

	//=============================================================
	// ワールド座標変換
	//=============================================================
	position = mul(matWorld_, position);

	//=============================================================
	// 高さ情報
	//=============================================================
	float2 uv1 = uv;
	float2 uv2 = uv;
	float2 uv3 = uv;
	float2 uv4 = uv;

	uv1.x -= waveDistance_ * 0.04;
	uv1.y += waveDistance_ * 0.03;
	uv2.x += waveDistance_ * 0.09;
	uv2.y += waveDistance_ * 0.03;

	uv3.y += waveDistance_ * 0.1;
	uv4.x += waveDistance_ * 0.05;
	uv4.y += waveDistance_ * 0.07;

	float3 height1 = HeightTexture1.SampleLevel(LinearSampler, uv1, 0).r;
	float3 height2 = HeightTexture1.SampleLevel(LinearSampler, uv2, 0).r;
	float3 height3 = HeightTexture2.SampleLevel(LinearSampler, uv3, 0).r;
	float3 height4 = HeightTexture2.SampleLevel(LinearSampler, uv4, 0).r;

	position.y += (height1.y + height2.y + height3.y + height4.y);
	//position.y += (height1.y + height2.y + height3.y) / 3;

	float3 wPos = position.xyz;

	//=============================================================
	// ビュー、プロジェクション変換
	//=============================================================
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	//=============================================================
	// 屈折用処理
	//=============================================================
	float4 pos = position;
	pos.x = pos.x * 0.5 + pos.w * 0.5;
	pos.y = pos.y * -0.5 + pos.w * 0.5;
	//pos.y = pos.y *  0.5 + pos.w * 0.5;

	//=============================================================
	// 出力情報
	//=============================================================
	output.position_ = position;
	output.uv_ = uv;
	output.worldPosition_ = wPos;
	output.grapPosition_ = pos;

	output.normal_ = mul(matWorld_, float4(normal, 0.0)).xyz;
	output.tangent_ = mul(matWorld_, float4(tangent, 0.0)).xyz;
	output.binormal_ = mul(matWorld_, float4(binormal, 0.0)).xyz;

	return output;
}
