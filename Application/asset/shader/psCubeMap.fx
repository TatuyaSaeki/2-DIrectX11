//===========================================================================
//!	@file	psCubeMap.fx
//!	@brief	キューブマップ
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
// ピクセルシェーダー
//===========================================================================
float4 main(PS_INPUT input) : SV_Target
{
	return cubeTexture.Sample(LinearSampler, input.worldPosition_);
}