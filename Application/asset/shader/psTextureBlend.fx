//===========================================================================
//!	@file	psTextureBlend.fx
//!	@brief	テクスチャ合成
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	float4 baseColor = BaseTexture.Sample(LinearSampler, input.uv_);
	float4 subColor = SubTexture.Sample(LinearSampler, input.uv_);

	return baseColor + subColor;
}