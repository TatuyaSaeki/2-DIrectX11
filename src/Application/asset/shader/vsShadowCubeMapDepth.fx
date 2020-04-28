//===========================================================================
//!	@file	vsShadowCubeMapDepth.fx
//!	@brief	全方位シャドウマップ 深度テクスチャ作成
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! 頂点シェーダー
//===========================================================================
GS_DEPTH_INPUT main(VS_INPUT input)
{
	GS_DEPTH_INPUT output = (GS_DEPTH_INPUT)0;

	float4 position = input.position_;

	position = mul(matWorld_, position);

	output.position_ = position;

	return output;
}