//===========================================================================
//!	@file	psEdge.fx
//!	@brief	エッジ描画
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	float3 color = 0.0;

	return float4(color, 1.0);
}