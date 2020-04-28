//===========================================================================
//!	@file	math_common.h
//!	@brief	�Z�p
//===========================================================================
#pragma once

//! PI
static constexpr f32 PI = 3.14159265f;

//! �p�x���烉�W�A��
inline constexpr f32 DEG_TO_RAD(f32 degree)
{
    return degree * (PI / 180.0f);
}

//! ���W�A������p�x
inline constexpr f32 RAD_TO_DEG(f32 radian)
{
    return radian * (180.0f / PI);
}
