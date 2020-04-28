//===========================================================================
//!	@file	vsWater.fx
//!	@brief	水用頂点シェーダー
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
	output.uv_       = input.uv_;
	output.normal_   = input.normal_;
	output.tangent_  = input.tangent_;
	output.binormal_ = input.binormal_;

	return output;
}