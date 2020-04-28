//===========================================================================
//! @file	collision_2D.cpp
//!	@brief	2D �����蔻��
//===========================================================================

//---------------------------------------------------------------------------
//! 2D �_vs�_
//---------------------------------------------------------------------------
bool Collision2D::isHit(const Point2D& point1, const Point2D& point2)
{
    Vector2 p1 = point1.getPosition();
    Vector2 p2 = point2.getPosition();

    Vector2 p = p1 - p2;

    if(fabs(sqrtf(p.lengthSq())) < 0.0005f) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! 2D �_vs�~
//---------------------------------------------------------------------------
bool Collision2D::isHit(const Point2D& point, const Circle2D& circle, f32* pLength)
{
    const Vector2 p1 = point.getPosition();
    const Vector2 c1 = circle.getPosition();

    Vector2 p = p1 - c1;

    f32 lenghtSq = p.lengthSq();
    f32 radius   = circle.getRadius();

    if(pLength != nullptr) {
        *pLength = sqrtf(lenghtSq);
    }

    if(lenghtSq <= radius * radius) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! 2D �~vs�~
//---------------------------------------------------------------------------
bool Collision2D::isHit(const Circle2D& circle1, const Circle2D& circle2)
{
    Vector2 c0 = circle1.getPosition();
    Vector2 c1 = circle2.getPosition();

    Vector2 p = c0 - c1;

    f32 lenght   = sqrtf(p.lengthSq());
    f32 radius   = circle1.getRadius() + circle2.getRadius();
    f32 radiusSq = radius * radius;

    if(lenght <= radiusSq) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! 2D ����vs����
//---------------------------------------------------------------------------
bool Collision2D::isHit(const LineSegment2D& line1, const LineSegment2D& line2, Vector2* outHitPosition)
{
    Vector2 startPos1 = line1.getPosition(0.0f);
    Vector2 endPos1   = line1.getPosition(1.0f);

    Vector2 startPos2 = line2.getPosition(0.0f);
    Vector2 endPos2   = line2.getPosition(1.0f);

    Vector2 v0 = endPos1 - startPos1;
    Vector2 v1 = endPos2 - startPos2;

    if(Vector2::cross(v0, v1) == 0.0f) {
        return false;
    }

    Vector2 v2     = v1 - v0;
    f32     cross0 = Vector2::cross(v2, v0);
    f32     cross1 = Vector2::cross(v2, v1);

    f32 t0 = cross1 / cross1;
    f32 t1 = cross0 / cross1;

    // �N�����v
    const f32 eps = 0.00001f;
    if(t0 + eps < 0 || t0 - eps > 1 || t1 + eps < 0 || t1 - eps > 1) {
        return false;
    }

    // �����_
    if(!outHitPosition) {
        Vector2 hitPosition = startPos1 + v0;
        hitPosition.x_      = hitPosition.x_ * t0;
        hitPosition.y_      = hitPosition.y_ * t0;
        *outHitPosition     = hitPosition;
    }
    return false;
}

//---------------------------------------------------------------------------
//! 2D ��`vs��`
//---------------------------------------------------------------------------
bool Collision2D::isHit(const Rect2D& rect1, const Rect2D& rect2)
{
    Vector2 p1 = rect1.getPosition();
    Vector2 p2 = rect2.getPosition();

    Vector2 size1 = rect1.getSize();
    Vector2 size2 = rect2.getSize();

    f32 px = fabs(p1.x_ - p2.x_);
    f32 py = fabs(p1.y_ - p2.y_);

    if(px < (size1.x_ + size2.x_) * 0.5f) {
        if(py < (size1.y_ + size2.y_) * 0.5f) {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//! 2D�̉~ �� 3D���W��2D���W�ɂ������� (Matrix)
//---------------------------------------------------------------------------
bool Collision2D::isHit(const Circle2D& circle, const Matrix& matWorld, const Matrix& matView, const Matrix& matProj, f32* outLenght, Vector2* outScreenPosition)
{
    Matrix world = matWorld;
    //Matrix view  = matView;
    //Matrix proj  = matProj;

    // �������o����̓䂷����
    //Matrix mat = matWorld0 * matView * matProj;
    Matrix mat = world * matView * matProj;

    // �X�N���[���s��쐬
    mat = mat * conversion2D(static_cast<f32>(GmRender()->getWidth()), static_cast<f32>(GmRender()->getHeight()));

    f32 nearZ = 0.01f;
    f32 farZ  = 10000.0f;

    f32 x = mat.m_[3][0];
    f32 y = mat.m_[3][1];
    f32 w = mat.m_[3][3];

    // �J�����`����ɓ����Ă��邩 (���s)
    if(!(w >= nearZ && w <= farZ)) {
        return false;
    }

    x = x / w;
    y = y / w;

	Point2D p{ {x,y} };

    // �_vs�~
    if(isHit(p, circle, outLenght)) {
        if(outScreenPosition) {
            *outScreenPosition = p.getPosition();
        }
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//! 2D�̉~ �� 3D���W��2D���W�ɂ������� (Vector3)
//---------------------------------------------------------------------------
bool Collision2D::isHit([[maybe_unused]] const Circle2D& circle0, [[maybe_unused]] const Vector3& position0, [[maybe_unused]] const Matrix& matView, [[maybe_unused]] const Matrix& matProj)
{
    return false;
}

//---------------------------------------------------------------------------
//! ��ʃT�C�Y�̃X�N���[���s���Ԃ�
//---------------------------------------------------------------------------
Matrix Collision2D::conversion2D(f32 w, f32 h)
{
    const f32 halfW = w * 0.5f;
    const f32 halfH = h * 0.5f;

    Matrix matScreen;
    matScreen.m_[0][0] = halfW;
    matScreen.m_[0][1] = 0.0f;
    matScreen.m_[0][2] = 0.0f;
    matScreen.m_[0][3] = 0.0f;

    matScreen.m_[1][0] = 0.0f;
    matScreen.m_[1][1] = -halfH;
    matScreen.m_[1][2] = 0.0f;
    matScreen.m_[1][3] = 0.0f;

    matScreen.m_[2][0] = 0.0f;
    matScreen.m_[2][1] = 0.0f;
    matScreen.m_[2][2] = 1.0f;
    matScreen.m_[2][3] = 0.0f;

    matScreen.m_[3][0] = halfW;
    matScreen.m_[3][1] = halfH;
    matScreen.m_[3][2] = 0.0f;
    matScreen.m_[3][3] = 1.0f;

    return matScreen;
}
