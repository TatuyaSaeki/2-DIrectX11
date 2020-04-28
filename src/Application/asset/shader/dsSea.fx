//===========================================================================
//!	@file	dsSea.fx
//!	@brief	海用ドメインシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ドメインシェーダー
//===========================================================================
[domain("tri")]
DS_INPUT main(HS_CONSTANT_TRI_INPUT input, float3 UV : SV_DomaInLocation, const OutputPatch<HS_INPUT, 3> patch)
{
	DS_INPUT output;

	float2 uv = patch[0].uv_* UV.x + patch[1].uv_ * UV.y + patch[2].uv_ * UV.z;

	float4 height	= NormalTexture.SampleLevel(LinearSampler, uv, 0)/3;

	float4 position = float4(patch[0].position_.xyz * UV.x + patch[1].position_.xyz * UV.y + patch[2].position_.xyz * UV.z, 1.0);

	position.y += (height.x + height.y + height.z) / 3;

	float3 worldPosition = 0;

	position = mul(matWorld_, position);
	worldPosition = position.xyz;
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	output.position_ = position;
	output.uv_		 = uv;
	output.worldPosition_ = worldPosition;

	return output;
}
