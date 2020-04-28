//===========================================================================
//! @file	math_matrix_util.h
//! @brief	行列のユーティリティ
//===========================================================================
#pragma once

//! 与えられた行列をスクリーン座標にする
//!	@param	[in]	matWorld	スクリーン座標にするワールド行列
//!	@param	[in]	width       幅
//!	@param	[in]	height      高さ
//!	@return	[out]	スクリーン座標
Matrix matrixConvert2D(const Matrix& matWorld, f32 width, f32 height);
