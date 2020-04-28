//===========================================================================
//!	@file	dsDisplacementMap.fx
//!	@brief	ドメインシェーダー
//===========================================================================
.. / shader /

//===========================================================================
//! ドメインシェーダー
//===========================================================================
[domain("quad")]
DS_INPUT main(HS_CONSTANT_QUAD_INPUT input, float2 UV : SV_DomaInLocation, const OutputPatch<HS_INPUT, 4> patch)
{
	DS_INPUT output;

	float2 topUv    = lerp(patch[0].uv_, patch[1].uv_, UV.x);
	float2 bottomUv = lerp(patch[3].uv_, patch[2].uv_, UV.x);
	float2 uv        = float2(lerp(topUv, bottomUv, UV.y));

	// .Sampleだとダメ。 みっぷマップレベルが必要らしい。(第3引数)
	float3 height = HeightTexture.SampleLevel(LinearSampler, uv, 0).rgb;
	//float height = BaseTexture.SampleLevel(LinearSampler, uv, 0).r;

	float3 topPos		= lerp(patch[0].position_.xyz, patch[1].position_.xyz, UV.x);
	float3 bottomPos	= lerp(patch[3].position_.xyz, patch[2].position_.xyz, UV.x);
	float4 position		= float4(lerp(topPos, bottomPos, UV.y), 1);
	
	position = mul(matWorld_, position);
	position.y += (height.x + height.y + height.z);// height * 3;
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	output.position_ = position;
	output.uv_		 = uv;

	return output;
}
