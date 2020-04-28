//===========================================================================
//!	@file	psShadowMap.fx
//!	@brief	シャドウマップ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT_SHADOW input) : SV_Target
{
	float4 baseColor = input.color_;
	float3 finalColor = 0.0;

	float shadow = 0.0;

	shadow = shadowSample(input.shadowMapUv_.xyz);

	finalColor = baseColor.rgb * shadow;

	return float4(shadow.xxx, 1);
}