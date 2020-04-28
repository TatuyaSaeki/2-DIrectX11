//===========================================================================
//!	@file	gsCubeMapDepth.fx
//!	@brief	キューブマップ用デプスジオメトリシェーダー
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ジオメトリシェーダー
//===========================================================================
[maxvertexcount(18)]
void main(triangle GS_DEPTH_INPUT input[3], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT output = (PS_INPUT)0;

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 3; ++j) {
			float4 position = input[j].position_;
			position = mul(matView_[i], position);
			position = mul(matProj_[i], position);

			output.position_ = position;
			triStream.Append(output);
		}
		triStream.RestartStrip();
	}
}
