//===========================================================================
//!	@file	math_color.cpp
//!	@brief	�Z�p
//===========================================================================

//---------------------------------------------------------------------------
//!	�R���X�g���N�^
//---------------------------------------------------------------------------
Color::Color(u8 r, u8 g, u8 b, u8 a)
{
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
}

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Color::Color(const Color& color)
{
    value_ = color.value_;
}

//---------------------------------------------------------------------------
//! operator=
//---------------------------------------------------------------------------
const Color& Color::operator=(const Color& color)
{
    value_ = color.value_;
    return *this;
}