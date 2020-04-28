//===========================================================================
//!	@file	vsSea.fx
//!	@brief	海用頂点シェーダー
//===========================================================================
#include "../shader/header/shader.h"

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
	output.normal_ = input.normal_;
	output.uv_ = (input.uv_ + waveDistance_ * 0.025);

	return output;
}
