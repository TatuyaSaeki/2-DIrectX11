//===========================================================================
//!	@file	vsTexture2D.fx
//!	@brief	スクリーン座標系でのテクスチャ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//!	頂点シェーダー(3D座標 -> 2D座標変換)
//===========================================================================
PS_INPUT_2D main(VS_INPUT_2D input)
{
	PS_INPUT_2D	output = (PS_INPUT_2D)0;

	float4	position = float4(input.position_, 0.0, 1.0);
	float2	screenPosition;

	position = mul(matWorld_, position);

	screenPosition.x = (position.x / viewPortSize.x) * 2 - 1;
	screenPosition.y = 1 - (position.y / viewPortSize.y) * 2;

	output.position_ = float4(screenPosition, 0, 1);
	output.uv_ = input.uv_;

	return output;
}