//#include "shape_3D.h"
//===========================================================================
//!	@file	shape_3D.cpp
//! @brief	3D 当たり判定形状
//===========================================================================

//---------------------------------------------------------------------------
//! ShapeTypeBase3D
//! コンストラクタ
//---------------------------------------------------------------------------
ShapeBase3D::ShapeBase3D()
: shapeType_(ShapeType3D::None)
{
}

//---------------------------------------------------------------------------
//! ShapeTypeBase3D
//! 形状取得
//---------------------------------------------------------------------------
ShapeType3D ShapeBase3D::getShapeType() const
{
    return shapeType_;
}

//---------------------------------------------------------------------------
//! Point3D
//! コンストラクタ
//---------------------------------------------------------------------------
Point3D::Point3D()
{
    shapeType_ = ShapeType3D::Point;
}

//---------------------------------------------------------------------------
//! Point3D
//! 位置設定
//---------------------------------------------------------------------------
void Point3D::setPosition(const Vector3& position)
{
    position_ = position;
}

//---------------------------------------------------------------------------
//! Point3D
//! 位置取得
//---------------------------------------------------------------------------
Vector3 Point3D::getPosition() const
{
    return position_;
}

//---------------------------------------------------------------------------
//! Sphere3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Sphere3D::Sphere3D()
{
    shapeType_ = ShapeType3D::Sphare;
};

//---------------------------------------------------------------------------
//! Sphere3D
//! コンストラクタ
//---------------------------------------------------------------------------
Sphere3D::Sphere3D(const Vector3& position, f32 radius)
: radius_(radius)
{
    position_ = position;
    this->Sphere3D::Sphere3D();
}

//---------------------------------------------------------------------------
//! Sphere3D
//! 半径設定
//---------------------------------------------------------------------------
void Sphere3D::setRadius(f32 radius)
{
    radius_ = radius;
}

//---------------------------------------------------------------------------
//! Sphere3D
//! 半径取得
//---------------------------------------------------------------------------
f32 Sphere3D::getRadius() const
{
    return radius_;
}

//---------------------------------------------------------------------------
//! Line3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Line3D::Line3D()
{
    shapeType_ = ShapeType3D::Line;
}

//---------------------------------------------------------------------------
//! Line3D
//! コンストラクタ
//---------------------------------------------------------------------------
Line3D::Line3D(const Vector3& start, const Vector3& end)
{
    position_[0] = start;
    position_[1] = end;
    this->Line3D::Line3D();
}

//---------------------------------------------------------------------------
//! Line3D
//! 位置取得
//---------------------------------------------------------------------------
Vector3 Line3D::getPosition(s32 index) const
{
    if(std::size(position_) <= index)
        return Vector3::ZERO;

    return position_[index];
}

//---------------------------------------------------------------------------
//! LineSegment3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
LineSegment3D::LineSegment3D()
{
    shapeType_ = ShapeType3D::LineSegment;
}

//---------------------------------------------------------------------------
//! LineSegment3D
//! コンストラクタ
//---------------------------------------------------------------------------
LineSegment3D::LineSegment3D(const Vector3& start, const Vector3& end)
{
    this->Line3D::Line3D(start, end);
}

//---------------------------------------------------------------------------
//! LineSegment3D
//! パラメーターtと渡して実際の座標を求める
//---------------------------------------------------------------------------
Vector3 LineSegment3D::getPosition(f32 t) const
{
    return (position_[1] - position_[0]) * t + position_[0];
}

//---------------------------------------------------------------------------
//! Capsule3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Capsule3D::Capsule3D()
{
    shapeType_ = ShapeType3D::Capsule;
}

//---------------------------------------------------------------------------
//! Capsule3D
//! コンストラクタ
//---------------------------------------------------------------------------
Capsule3D::Capsule3D(const Vector3& start, const Vector3& end)
: lineSegment_(start, end)
{
    this->Capsule3D::Capsule3D();
}

//-----------------------------------------------------------------------
//! Capsule3D
//! 線分のパラメーターtと渡して実際の座標を求める
//-----------------------------------------------------------------------
Vector3 Capsule3D::getPosition(f32 t) const
{
    return lineSegment_.getPosition(t);
}

//-----------------------------------------------------------------------
//! Capsule3D
//! 半径取得
//-----------------------------------------------------------------------
f32 Capsule3D::getRadius() const
{
    return radius_;
}

//---------------------------------------------------------------------------
//! Plane3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Plane3D::Plane3D()
{
    shapeType_ = ShapeType3D::Plane;
}

//---------------------------------------------------------------------------
//! Plane3D
//! コピーコストラクタ
//---------------------------------------------------------------------------
Plane3D::Plane3D(const Plane3D& other)
{
    normal_   = other.normal_;
    distance_ = other.distance_;

    this->Plane3D::Plane3D();
}
//---------------------------------------------------------------------------
//! Plane3D
//! コンストラクタ (法線ベクトル と 代表点1点)
//---------------------------------------------------------------------------
Plane3D::Plane3D(const Vector3& normal, const Vector3& p)
{
    this->initialize(normal, p);

    this->Plane3D::Plane3D();
}

//---------------------------------------------------------------------------
//! Plane3D
//! コンストラクタ (代表点3点)
//---------------------------------------------------------------------------
Plane3D::Plane3D(const Vector3& a, const Vector3& b, const Vector3& c)
{
    Vector3 dirA = c - b;
    Vector3 dirB = a - b;

    Vector3 normal = Vector3::cross(dirA, dirB);

    this->initialize(normal, a);

    this->Plane3D::Plane3D();
}

//---------------------------------------------------------------------------
//! Plane3D
//! 初期化 (法線ベクトル と 代表点1点)
//---------------------------------------------------------------------------
void Plane3D::initialize(const Vector3& normal, const Vector3& p)
{
    // ABC = 面法線ベクトル
    normal_ = normal.normalize();

    // Dを求める
    // Ax + By + Cz + D = 0
    //				  D = -(Ax + By + Cz)
    d_ = -Vector3::dot(normal_, p);
}

//---------------------------------------------------------------------------
//! 法線取得
//---------------------------------------------------------------------------
Vector3 Plane3D::getNormal() const
{
	return normal_;
}

//---------------------------------------------------------------------------
//! Triangle3D
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Triangle3D::Triangle3D()
{
    shapeType_ = ShapeType3D::Triangle;
}

//---------------------------------------------------------------------------
//! Triangle3D
//! コピーコンストラクタ
//---------------------------------------------------------------------------
Triangle3D::Triangle3D(const Triangle3D& other)
: Plane3D(other)
{
    position_[0] = other.position_[0];
    position_[1] = other.position_[1];
    position_[2] = other.position_[2];

    this->Triangle3D::Triangle3D();
}

//---------------------------------------------------------------------------
//! Triangle3D
//! コンストラクタ (代表点3点)
//---------------------------------------------------------------------------
Triangle3D::Triangle3D(const Vector3& a, const Vector3& b, const Vector3& c)
: Plane3D(a, b, c)
{
    this->Triangle3D::Triangle3D();
}

//---------------------------------------------------------------------------
//! Triangle3D
//! 位置取得
//---------------------------------------------------------------------------
Vector3 Triangle3D::getPosition(s32 index) const
{
    if(std::size(position_) <= index)
        return Vector3::ZERO;

    return position_[index];
}

//---------------------------------------------------------------------------
//! Rectangle
//! デフォルトコンストラクタ
//---------------------------------------------------------------------------
Rectangle::Rectangle()
{
    shapeType_ = ShapeType3D::Rectangle;
}

//---------------------------------------------------------------------------
//! Rectangle
//! コンストラクタ (代表点4点)
//---------------------------------------------------------------------------
Rectangle::Rectangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
{
    vertexPosition_[0] = a;
    vertexPosition_[1] = b;
    vertexPosition_[2] = c;
    vertexPosition_[3] = d;

    this->Rectangle::Rectangle();
}

//--------------------------------------------------------------------------
//! Box3D
//! デフォルトコンストラクタ
//--------------------------------------------------------------------------
Box3D::Box3D()
{
    shapeType_ = ShapeType3D::Box;
}

//--------------------------------------------------------------------------
//! Box3D
//! コンストラクタ
//--------------------------------------------------------------------------
Box3D::Box3D(const Vector3& min, const Vector3& max)
{
    position_[0] = min;
    position_[1] = max;

    this->Box3D::Box3D();
}

//! サイズ設定
void Box3D::setSize(const Vector3& min, const Vector3& max)
//: this->Box3D::Box3D(min, max)
{
    //Box3D(min, max);
    position_[0] = min;
    position_[1] = max;
}

//--------------------------------------------------------------------------
//! 位置取得
//--------------------------------------------------------------------------
Vector3 Box3D::getPosition(s32 index) const
{
    if(std::size(position_) <= index)
        return Vector3::ZERO;

    return position_[index];
}