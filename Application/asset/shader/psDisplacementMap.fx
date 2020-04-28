//===========================================================================
//! @file	psDisplacementMap.h
//!	@brief	�f�B�X�v���C�X�����g�}�b�s���O(�n�C�g�}�b�v)
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! �s�N�Z���V�F�[�_�[
//===========================================================================
float4 main(DS_INPUT input) : SV_Target
{
	//return float4(1,0,0,1);
	float4 color = HeightTexture.Sample(LinearSampler, input.uv_);

	return color;
}