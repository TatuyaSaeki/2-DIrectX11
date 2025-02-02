//===========================================================================
//!	@file	math_common.h
//!	@brief	算術
//===========================================================================
#pragma once

//! PI
static constexpr f32 PI = 3.14159265f;

//! 角度からラジアン
inline constexpr f32 DEG_TO_RAD(f32 degree)
{
    return degree * (PI / 180.0f);
}

//! ラジアンから角度
inline constexpr f32 RAD_TO_DEG(f32 radian)
{
    return radian * (180.0f / PI);
}
