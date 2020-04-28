//===========================================================================
//!	@file	blur.h
//!	@brief	�ڂ����t�B���^�[�p�֐�
//===========================================================================

//! 9x9 �����K�E�V�A���t�B���^
//!	@param	[in]	image		�c�܂���e�N�X�`��
//!	@param	[in]	uv			�e�N�X�`��UV�l
//!	@param	[in]	resolution	�e�N�X�`���̉𑜓x
//!	@param	[in]	direction	�c�܂������(�c������ɂ�2��Ă�ł���ς���)
//! @return	�c�܂�������
float4 blur9_reference(Texture2D image, float2 uv, float2 resolution, float2 direction)
{
	float4 color = 0.0;

	float2 pixelHalf   = float2(0.5, 0.5) * direction / resolution;	// �n�[�t�s�N�Z���̂��炵��
	float2 pixelStride = float2(2.0, 2.0) * direction / resolution;	// ���̃s�N�Z���܂ł̕�

	color += image.Sample(ClampSampler, uv + pixelHalf + pixelStride * -2) * (1.0 / 16.0);
	color += image.Sample(ClampSampler, uv + pixelHalf + pixelStride * -1) * (4.0 / 16.0);
	color += image.Sample(ClampSampler, uv + pixelHalf + pixelStride *  0) * (6.0 / 16.0);
	color += image.Sample(ClampSampler, uv + pixelHalf + pixelStride * +1) * (4.0 / 16.0);
	color += image.Sample(ClampSampler, uv + pixelHalf + pixelStride * +2) * (1.0 / 16.0);

	return color;
}

#if 0
float4 blur9(Texture2D image, float2 uv, float2 resolution, float2 direction)
{
	float4 color = 0.0;
	float2 off1 = 1.3846153846 * direction;
	float2 off2 = 3.2307692308 * direction;
	color += image.Sample(LinearSampler, uv) * 0.2270270270;
	color += image.Sample(LinearSampler, uv + (off1 / resolution)) * 0.3162162162;
	color += image.Sample(LinearSampler, uv - (off1 / resolution)) * 0.3162162162;
	color += image.Sample(LinearSampler, uv + (off2 / resolution)) * 0.0702702703;
	color += image.Sample(LinearSampler, uv - (off2 / resolution)) * 0.0702702703;
	return color;
}
#endif
