//===========================================================================
//!	@file	draw_funcions.h
//!	@brief	描画関数まとめ
//===========================================================================
#pragma once

//---------------------------------------------------------------------------
//!	@brief	グリッド描画
//!	@param	[in]	size	範囲
//---------------------------------------------------------------------------
void gridRender(f32 size);

//---------------------------------------------------------------------------
//!	@brief	球体描画 (Vector4, Color 2種類)
//!	@param	[in]	position	描画したい位置
//!	@param	[in]	radius		半径
//!	@param	[in]	color		色
//!	@param	[in]	divCount	分割数
//---------------------------------------------------------------------------
void drawSphere(const Vector3& position, f32 radius, const Vector4& color, u32 divCount = 16);
void drawSphere(const Vector3& position, f32 radius, const Color& color, u32 divCount = 16);

//---------------------------------------------------------------------------
//!	@brief	球体描画(行列定期用済みの場合こっち) (Vector4, Color 2種類)
//!	@param	[in]	radius		半径
//!	@param	[in]	color		色
//!	@param	[in]	divCount	分割数
//---------------------------------------------------------------------------
void drawSphere(f32 radius, const Vector4& color, u32 divCount = 16);
void drawSphere(f32 radius, const Color& color, u32 divCount = 16);

//---------------------------------------------------------------------------
//! @brief	上の関数でカラー設定してからこっちで本体描画
//!	@param	[in]	radius		半径
//!	@param	[in]	color		色
//!	@param	[in]	divCount	分割数
//---------------------------------------------------------------------------
void drawSphereEx(const Vector3& position, f32 radius, u32 divCount);
void drawSphereEx(f32 radius, u32 divCount);

//---------------------------------------------------------------------------
//! @brief	ポリゴンで球描画
//!	@param	[in]	slices	分割数	
//!	@param	[in]	stacks	分割数
//---------------------------------------------------------------------------
void drawSphare(u32 slices, u32 stacks);

//---------------------------------------------------------------------------
//! @brief	ローカル座標でライン描画
//!	@param	[in]	scale	長さ
//---------------------------------------------------------------------------
void drawLocalLine(f32 scale);

//---------------------------------------------------------------------------
//! @brief	ワールド座標でライン描画
//!	@param	[in]	startPoint	始点位置
//!	@param	[in]	dir			向き
//!	@param	[in]	scale		長さ
//---------------------------------------------------------------------------
void drawLine(const Vector3& startPoint, const Vector3& dir, f32 scale = 1.0f);

//---------------------------------------------------------------------------
//! @brief	円柱描画
//!	@param	[in]	radius		半径
//!	@param	[in]	height		高さ
//!	@param	[in]	color		色
//!	@param	[in]	divCount	分割数
//---------------------------------------------------------------------------
void drawCone(f32 radius, f32 height, const Color& color, u32 divCount = 16);



