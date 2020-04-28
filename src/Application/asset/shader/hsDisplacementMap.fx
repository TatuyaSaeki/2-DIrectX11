//===========================================================================
//!	@file	hsDisplacementMap.fx
//!	@brief	ハルシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ハルシェーダーコンスタント
//===========================================================================
HS_CONSTANT_QUAD_INPUT HSConstant(InputPatch<VS_INPUT, 4> ip, uint pid : SV_PrimitiveID)
{
	HS_CONSTANT_QUAD_INPUT output;

	float devide = 4;

	output.factor[0] = devide;
	output.factor[1] = devide;
	output.factor[2] = devide;
	output.factor[3] = devide;

	output.inner_factor[0] = devide;//u �c�̕������i���̃��C�������{�Ђ����j
	output.inner_factor[1] = devide;//v

	return output;
}

//===========================================================================
//! ハルシェーダー(四角)
//===========================================================================
[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSConstant")]
HS_INPUT main(InputPatch<VS_INPUT, 4> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)
{
	HS_INPUT output;
	output.position_ = ip[cpid].position_;
	output.uv_       = ip[cpid].uv_;

	return output;
}