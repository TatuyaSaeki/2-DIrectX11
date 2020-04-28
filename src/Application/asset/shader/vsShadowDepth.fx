//===========================================================================
//!	@file	vsShadowDepth.fx
//! @brief	シャドウマップ 深度テクスチャ作成
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 position = input.position_;

	position = mul(matWorld_, position);
	position = mul(matLightView_, position);
	position = mul(matLightProj_, position);

	output.position_ = position;

	return output;
}