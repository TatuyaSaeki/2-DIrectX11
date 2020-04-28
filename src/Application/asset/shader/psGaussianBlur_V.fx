//===========================================================================
//! @file	psGaussianBlur_W.fx
//! @breif	横ガウシアンブラー
//===========================================================================
#include "../shader/header/shader.h"
#include "header/blur.h"

//===========================================================================
// ピクセルシェーダー (テクスチャあり)
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	// 9x9 高速ガウシアンフィルタ
	uint	w;
	uint	h;
	uint mipLevels;
	BaseTexture.GetDimensions(0, w, h, mipLevels);

	return blur9_reference(BaseTexture, input.uv_, float2(w, h), float2(1,0));	// 水平ブラー
}
