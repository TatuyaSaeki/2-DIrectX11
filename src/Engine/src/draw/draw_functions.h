//===========================================================================
//!	@file	draw_funcions.h
//!	@brief	�`��֐��܂Ƃ�
//===========================================================================
#pragma once

//---------------------------------------------------------------------------
//!	@brief	�O���b�h�`��
//!	@param	[in]	size	�͈�
//---------------------------------------------------------------------------
void gridRender(f32 size);

//---------------------------------------------------------------------------
//!	@brief	���̕`�� (Vector4, Color 2���)
//!	@param	[in]	position	�`�悵�����ʒu
//!	@param	[in]	radius		���a
//!	@param	[in]	color		�F
//!	@param	[in]	divCount	������
//---------------------------------------------------------------------------
void drawSphere(const Vector3& position, f32 radius, const Vector4& color, u32 divCount = 16);
void drawSphere(const Vector3& position, f32 radius, const Color& color, u32 divCount = 16);

//---------------------------------------------------------------------------
//!	@brief	���̕`��(�s�����p�ς݂̏ꍇ������) (Vector4, Color 2���)
//!	@param	[in]	radius		���a
//!	@param	[in]	color		�F
//!	@param	[in]	divCount	������
//---------------------------------------------------------------------------
void drawSphere(f32 radius, const Vector4& color, u32 divCount = 16);
void drawSphere(f32 radius, const Color& color, u32 divCount = 16);

//---------------------------------------------------------------------------
//! @brief	��̊֐��ŃJ���[�ݒ肵�Ă��炱�����Ŗ{�̕`��
//!	@param	[in]	radius		���a
//!	@param	[in]	color		�F
//!	@param	[in]	divCount	������
//---------------------------------------------------------------------------
void drawSphereEx(const Vector3& position, f32 radius, u32 divCount);
void drawSphereEx(f32 radius, u32 divCount);

//---------------------------------------------------------------------------
//! @brief	�|���S���ŋ��`��
//!	@param	[in]	slices	������	
//!	@param	[in]	stacks	������
//---------------------------------------------------------------------------
void drawSphare(u32 slices, u32 stacks);

//---------------------------------------------------------------------------
//! @brief	���[�J�����W�Ń��C���`��
//!	@param	[in]	scale	����
//---------------------------------------------------------------------------
void drawLocalLine(f32 scale);

//---------------------------------------------------------------------------
//! @brief	���[���h���W�Ń��C���`��
//!	@param	[in]	startPoint	�n�_�ʒu
//!	@param	[in]	dir			����
//!	@param	[in]	scale		����
//---------------------------------------------------------------------------
void drawLine(const Vector3& startPoint, const Vector3& dir, f32 scale = 1.0f);

//---------------------------------------------------------------------------
//! @brief	�~���`��
//!	@param	[in]	radius		���a
//!	@param	[in]	height		����
//!	@param	[in]	color		�F
//!	@param	[in]	divCount	������
//---------------------------------------------------------------------------
void drawCone(f32 radius, f32 height, const Color& color, u32 divCount = 16);



