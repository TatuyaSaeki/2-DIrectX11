//===========================================================================
//!	@file	vsDisplancementMap.fx
//!	@brief	ディスプレイスメントマッピング用
//===========================================================================
#include "header/shader.h"../shader/

//===========================================================================
//! 頂点シェーダー
//===========================================================================
VS_INPUT main(VS_INPUT input)
{
	VS_INPUT output = (VS_INPUT)0;

	float4 position = input.position_;
	//position = mul(matWorld_, position );
	//position = mul(matView_[0], position );
	//position = mul(matProj_[0], position );

	output.position_ = position;
	output.uv_ = input.uv_;

	return output;
}
