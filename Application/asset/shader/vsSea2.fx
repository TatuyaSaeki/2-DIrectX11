//===========================================================================
//!	@file	vsSea2.fx
//!	@brief	海用頂点シェーダー2
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 波作成用関数
//!	@param	[in]	worldPosition	ワールドポジション
//!	@param	[in]	t				1フレームで進ませる距離
//!	@param	[in]	waveLen			波の波長	
//!	@param	[in]	sharp			波の尖り度
//!	@param	[in]	height			波の高さ			
//!	@param	[in]	browDir			移動させてい方向
//!	@param	[out]	計算後ポジション
//!	@param	[out]	計算後法線
//===========================================================================
void createWave(float3 worldPosition, float t, float waveLen, float sharp, float height, float2 browDir, inout float3 outPos, inout float3 outNormal)
{
	browDir = normalize(browDir);
	const float grav = 9.8;
	float       A = waveLen / 14.0;
	float       pi2_per_L = 2.0 * PI / waveLen;
	float       d = dot(browDir, worldPosition.xz);
	float       th = pi2_per_L * d + sqrt(grav / pi2_per_L) * t;

	float3 pos = float3(0.0, height * A * sin(th), 0.0);
	pos.xz = sharp * A * browDir * cos(th);

	float3 normal = float3(0.0, 0.1, 0.0);
	normal.xz = -browDir * height * cos(th) / (7.0 / PI - sharp * browDir * browDir * sin(th));

	outPos += pos;
	outNormal += normalize(normal);
}

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 position = input.position_;
	float3 worldPosition = 0.0;
	position = mul(matWorld_, position);
	worldPosition = position.xyz;

	float3 outPos = 0.0;
	float3 outNormal = 0.0;

	createWave(worldPosition, 0.2 * waveDistance_, 12.9 * 5.0 * waveFrequency_, waveSharp_, 0.3 * waveHeight_, float2(+0.2, +0.3), outPos, outNormal);
	createWave(worldPosition, 0.4 * waveDistance_, 20.1 * 4.0 * waveFrequency_, waveSharp_, 0.5 * waveHeight_, float2(-0.4, +0.7), outPos, outNormal);
	createWave(worldPosition, 0.3 * waveDistance_, 8.8 * 4.0 * waveFrequency_, waveSharp_, 0.2 * waveHeight_, float2(+0.5, +0.1), outPos, outNormal);
	createWave(worldPosition, 0.1 * waveDistance_, 4.2 * 8.0 * waveFrequency_, waveSharp_, 0.4 * waveHeight_, float2(-0.3, +0.6), outPos, outNormal);

	createWave(worldPosition, 0.1 * waveDistance_, 6.5 * waveFrequency_, waveSharp_ * 0.2, 0.5 * waveHeight_, float2(+0.0, +1.0), outPos, outNormal);
	createWave(worldPosition, 0.6 * waveDistance_, 8.5 * waveFrequency_, waveSharp_ * 0.6, 1.2 * waveHeight_, float2(-0.9, +0.1), outPos, outNormal);
	createWave(worldPosition, 0.2 * waveDistance_, 12.5 * waveFrequency_, waveSharp_ * 0.8, 0.8 * waveHeight_, float2(-0.6, +0.2), outPos, outNormal);
	createWave(worldPosition, 0.4 * waveDistance_, 8.5 * waveFrequency_, waveSharp_ * 0.3, 1.1 * waveHeight_, float2(+0.9, +0.1), outPos, outNormal);

	position.xyz += outPos;
	worldPosition.xyz += outPos;

	float3 normal = normalize(outNormal);

	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	normal = mul(matWorld_, float4(normal, 1.0)).xyz;

	output.position_ = position;
	output.normal_ = normal;
	output.uv_ = input.uv_;
	output.worldPosition_ = worldPosition;

	return output;
}
