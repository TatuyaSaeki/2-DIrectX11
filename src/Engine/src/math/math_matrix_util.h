//===========================================================================
//! @file	math_matrix_util.h
//! @brief	�s��̃��[�e�B���e�B
//===========================================================================
#pragma once

//! �^����ꂽ�s����X�N���[�����W�ɂ���
//!	@param	[in]	matWorld	�X�N���[�����W�ɂ��郏�[���h�s��
//!	@param	[in]	width       ��
//!	@param	[in]	height      ����
//!	@return	[out]	�X�N���[�����W
Matrix matrixConvert2D(const Matrix& matWorld, f32 width, f32 height);
