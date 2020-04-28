//===========================================================================
//!
//!	@file	shader.fx
//!	@brief	HLSL�\�[�X�R�[�h
//!
//===========================================================================
#include "header/shader.h"

//---------------------------------------------------------------------------
// VS(�o���v�}�b�s���O)
//---------------------------------------------------------------------------
PS_INPUT main(VS_INPUT_BUMP input)
{
	PS_INPUT	output = (PS_INPUT)0;	// ���_�V�F�[�_�[�o�͌��ʂ̓s�N�Z���V�F�[�_�[�Ɏg����

	//-------------------------------------------------------------
	// ���_�̍��W�ϊ�
	//-------------------------------------------------------------
	float4	position = input.position_;
	float4 worldPosition = 0;
	position = worldPosition = mul(matWorld, position);

	float3 L = normalize((dlLookAt-dlPosition.xyz)-worldPosition);
	float3 V = noramlize(cameraPosition-worldPosition);
	float3 H = normalize(L+V);

	matrix WIT = matWorld * matInverse * matTranspose;
	float3 T = mul(tangent, (float3x3)WIT);
	float3 N = mul(normalize(input.normal_), (float3x3)WIT);
	float3 B = cross(N, T) * tangent.w;

	float3x3 tangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);

	//-------------------------------------------------------------
	// �o��
	//-------------------------------------------------------------
	output.position_		= position;		// ���W�͓��e���xyzw���W�l (GPU�ɂ���Ē��_�V�F�[�_�[���s��Ɏ�����xyz��w�����s�����)
	output.normal_			= normal;
	output.color_			= input.color_;
	output.uv_				= input.uv_;	    // UV�����H���Ȃ��ꍇ�͂��̂܂ܑ���ŃX���[������
	output.worldPosition_	= worldPosition;

	output.light_			= mul(L, tangentToObject);
	output.halfVector		= mul(H, tangentToObject);

	return output;
}
