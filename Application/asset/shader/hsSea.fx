//===========================================================================
//!	@file	hsSea.fx
//!	@brief	海用ハルシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ハルシェーダーコンスタント
//===========================================================================
HS_CONSTANT_TRI_INPUT HSConstant(InputPatch<VS_INPUT, 3> ip, uint pid : SV_PrimitiveID)
{
	HS_CONSTANT_TRI_INPUT output;

	float devide = 64;

	output.factor[0] = devide;
	output.factor[1] = devide;
	output.factor[2] = devide;

	output.inner_factor = devide;

	return output;
}

//===========================================================================
//! ハルシェーダー
//===========================================================================
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
HS_INPUT main(InputPatch<VS_INPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)
{
	HS_INPUT output;
	output.position_ = ip[cpid].position_;
	output.uv_       = ip[cpid].uv_;

	return output;
}