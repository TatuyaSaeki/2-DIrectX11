#include "shape_2D.h"
//===========================================================================
//! @file	shape_2D.cpp
//! @brief	2D 当たり判定形状定義
//===========================================================================

//---------------------------------------------------------------------------
//! ShapeType2D
//!	コンストラクタ
//---------------------------------------------------------------------------
ShapeType2D ShapeBase2D::getShapeType() const
{
    return shapeType_;
}

//---------------------------------------------------------------------------
//! Point2D
//!	デフォルトコンストラクタ
//---------------------------------------------------------------------------
Point2D::Point2D()
{
    shapeType_ = ShapeType2D::Point;
}

//---------------------------------------------------------------------------
//! Point2D
//!	コンストラクタ
//---------------------------------------------------------------------------
Point2D::Point2D(const Vector2& position)
: position_(position)
{
    this->Point2D::Point2D();
}

//-----------------------------------------------------------------------
//! Point2D
//! 位置設定
//-----------------------------------------------------------------------
void Point2D::setPosition(const Vector2& position)
{
	position_ = position;
}

//---------------------------------------------------------------------------
//! Point2D
//! 位置取得
//---------------------------------------------------------------------------
Vector2 Point2D::getPosition() const
{
    return position_;
}

//---------------------------------------------------------------------------
//! Circle2D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Circle2D::Circle2D()
{
    shapeType_ = ShapeType2D::Circle;
}

//---------------------------------------------------------------------------
//! Circle2D
//! コンストラクタ
//---------------------------------------------------------------------------
Circle2D::Circle2D(f32 radius)
: radius_(radius)
{
}

//---------------------------------------------------------------------------
//! Circle2D
//! 半径設定
//---------------------------------------------------------------------------
void Circle2D::setRadius(f32 radius)
{
    radius_ = radius;
}

//---------------------------------------------------------------------------
//! Circle2D
//! 半径取得
//---------------------------------------------------------------------------
f32 Circle2D::getRadius() const
{
    return radius_;
}

//---------------------------------------------------------------------------
//! LineSegment2D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
LineSegment2D::LineSegment2D()
{
    shapeType_ = ShapeType2D::LineSegment;
}

//---------------------------------------------------------------------------
//! LineSegment2D
//! コンストラクタ
//---------------------------------------------------------------------------
LineSegment2D::LineSegment2D(const Vector2& start, const Vector2& end)
{
    position_[0] = start;
    position_[1] = end;
}

//---------------------------------------------------------------------------
//! LineSegment2D
//! パラメーターtと渡して実際の座標を求める
//---------------------------------------------------------------------------
Vector2 LineSegment2D::getPosition(f32 t) const
{
    return (position_[1] - position_[0]) * t + position_[0];
}

//---------------------------------------------------------------------------
//! Rect2D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Rect2D::Rect2D()
{
    shapeType_ = ShapeType2D::Rect;
}

//---------------------------------------------------------------------------
//! Rect2D
//! コンストラクタ
//---------------------------------------------------------------------------
Rect2D::Rect2D(const Vector2& position, f32 w, f32 h)
: position_(position)
, w_(w)
, h_(h)
{
    this->Rect2D::Rect2D();
}

//---------------------------------------------------------------------------
//! Rect2D
//! 位置取得
//---------------------------------------------------------------------------
Vector2 Rect2D::getPosition() const
{
    return position_;
}

//---------------------------------------------------------------------------
//! Rect2D
//! 幅取得
//---------------------------------------------------------------------------
f32 Rect2D::getW() const
{
    return w_;
}

//---------------------------------------------------------------------------
//! Rect2D
//! 高さ取得
//---------------------------------------------------------------------------
f32 Rect2D::getH() const
{
    return h_;
}

//-----------------------------------------------------------------------
//! Rect2D
//! サイズ取得
//-----------------------------------------------------------------------
Vector2 Rect2D::getSize() const
{
    return Vector2(w_, h_);
}