//===========================================================================
//! @file	collision_3D.cpp
//!	@brief	�Փ˔��菈��
//===========================================================================

namespace /* �O������J */
{

//---------------------------------------------------------------------------
//! @brief  �N�����v
//! @param  [in] x   �N�����v�������l
//! @param  [in] min �N�����v�������ŏ��l
//! @param  [in] max �N�����v�������ő�l
//! @return �N�����v��̒l
//---------------------------------------------------------------------------
f32 clamp(f32 x, f32 min, f32 max)
{
    return std::fmin(std::fmax(x, min), max);
}

//---------------------------------------------------------------------------
//! @brief  �_�Ɛ����̋�����2��
//! @param  [in] point	    �_�̈ʒu
//! @param  [in] segment	����
//! @return �_�Ɛ����̋�����2��
//---------------------------------------------------------------------------
f32 lineSegmentDistancePointSq(const Vector3& point, const LineSegment3D& segment)
{
    // �n�_, �I�_
    auto start = segment.getPosition(0);
    auto end   = segment.getPosition(1);

    // �����̎n�_����I�_�ւ̃x�N�g��
    Vector3 lineDir = end - start;
    // �����̎n�_����|�C���g�ւ̃x�N�g��
    Vector3 linePointDir = point - end;

    if(Vector3::dot(lineDir, linePointDir) < FLT_EPSILON) {
        // �Q�x�N�g���̓��ς����Ȃ�A�����̎n�_���ŋߖT
        return Vector3::dot(linePointDir, linePointDir);
    }

    // �_��������̏I�_�ւ̃x�N�g��
    linePointDir = end - point;
    if(Vector3::dot(lineDir, linePointDir) < FLT_EPSILON) {
        // �Q�x�N�g���̓��ς����Ȃ�A�����̏I�_���ŋߖT
        return Vector3::dot(linePointDir, linePointDir);
    }

    // ��L�̂ǂ���ɂ��Y�����Ȃ��ꍇ�A������ɗ��Ƃ����ˉe���ŋߖT
    // (�{���Ȃ�T�C���ŋ��߂邪�A�O�ς̑傫��/�����̃x�N�g���̑傫���ŋ��܂�)
    Vector3 CP = Vector3::cross(lineDir, linePointDir);

    return Vector3::dot(CP, CP) / Vector3::dot(lineDir, lineDir);
}

//---------------------------------------------------------------------------
//! @brief  �����Ɛ����̋�����2��
//! @param  [in] segment1 �����P
//! @param  [in] segment2 �����Q
//! @return �����Ɛ����̋���
//---------------------------------------------------------------------------
f32 lineSegmentDistanceSq(const LineSegment3D& segment1, const LineSegment3D& segment2)
{
    auto start1 = segment1.getPosition(0);
    auto end1   = segment1.getPosition(1);

    auto start2 = segment2.getPosition(0);
    auto end2   = segment2.getPosition(1);

    Vector3 line1[] = { start1, end1 };
    Vector3 line2[] = { start2, end2 };

    // ����1�̕����x�N�g��
    Vector3 segment1Dir = line1[1] - line1[0];
    // ����2�̕����x�N�g��
    Vector3 segment2Dir = line2[1] - line2[0];

    // ����2�̎n�_�������1�̎n�_�ւ̃x�N�g��
    Vector3 segmentDir = line1[0] - line2[0];

    f32 segment1Sq = Vector3::dot(segment1Dir, segment1Dir);
    f32 segment2Sq = Vector3::dot(segment2Dir, segment2Dir);

    // ���Âꂩ�̐����̒�����0���ǂ����`�F�b�N
    if(segment1Sq <= FLT_EPSILON && segment2Sq <= FLT_EPSILON) {
        // ��������0
        Vector3 v = line1[0] - line2[1];
        return Vector3::dot(v, v);
    }

    if(segment1Sq <= FLT_EPSILON) {   // ����0������0
        return lineSegmentDistancePointSq(line1[0], LineSegment3D(line2[0], line2[1]));
    }

    if(segment2Sq <= FLT_EPSILON) {   // ����1������0
        return lineSegmentDistancePointSq(line2[0], LineSegment3D(line1[0], line1[1]));
    }

    f32 b = Vector3::dot(segment1Dir, segment2Dir);
    f32 f = Vector3::dot(segment2Dir, segmentDir);
    f32 c = Vector3::dot(segment1Dir, segmentDir);

    f32 s = 0.0f;
    f32 t = 0.0f;

    // ���������s�łȂ��ꍇ�A����0��̒���1�ɑ΂���ŋߐړ_���v�Z�A������
    // ����0��ɃN�����v�B�����łȂ��ꍇ�͔C�ӂ�s��I��
    f32 denom = segment1Sq * segment2Sq - b * b;
    if(denom != 0.0f) {
        s = clamp((b - c) / segment1Sq, 0, 1);
    }
    else {
        s = 0.0f;
    }

    // ����1��̍Őڋߓ_���v�Z
    f32 tnom = b * s + f;

    if(tnom < 0.0f) {
        t = 0.0f;
        s = clamp((-c / segment1Sq), 0, 1);
    }
    else if(tnom > segment2Sq) {
        t = 1.0f;
        s = clamp((b - segment2Sq) / segment1Sq, 0, 1);
    }
    else {
        t = tnom / segment2Sq;
    }

    Vector3 c0 = (segment1Dir * s) + line1[0];
    Vector3 c1 = (segment2Dir * t) + line2[0];
    Vector3 v  = c0 - c1;

    return Vector3::dot(v, v);
}

//---------------------------------------------------------------------------
//! @brief  �����Ɛ����̋�����2��
//! @param  [in] segment1 ����1
//! @param  [in] segment2 ����2
//! @param  [in] matrix1  �s��1
//! @param  [in] matrix2  �s��2
//! @return �����Ɛ����̋�����2��
//---------------------------------------------------------------------------
f32 lineSegmentDistanceSq(const LineSegment3D& segment1, const LineSegment3D& segment2, const Matrix& matrix1, const Matrix& matrix2)
{
    LineSegment3D l0 = segment1;
    LineSegment3D l1 = segment2;

    auto start1 = segment1.getPosition(0);
    auto end1   = segment1.getPosition(1);

    auto start2 = segment2.getPosition(0);
    auto end2   = segment2.getPosition(1);

    start1 = start1.transform(matrix1);
    end1   = end1.transform(matrix1);

    start2 = start2.transform(matrix2);
    end2   = end2.transform(matrix2);

    return lineSegmentDistanceSq(l0, l1);
}
}   // namespace

//---------------------------------------------------------------------------
//! ���Ƌ��̋��������߂�
//---------------------------------------------------------------------------
f32 Collision3D::distance(const Vector3& p1, const Vector3& p2, f32 r1, f32 r2)
{
    const f32 length = (p2 - p1).lengthSq();
    if(length == 0.0f)
        return 0.0f;

    const f32 L = sqrtf(length);
    const f32 R = r1 + r2;

    return L - R;
}

//---------------------------------------------------------------------------
//! ���Ƌ��̋��������߂�
//---------------------------------------------------------------------------
f32 Collision3D::distance(const Sphere3D& s1, const Sphere3D& s2)
{
    const auto position1 = s1.getPosition();
    const auto radius1   = s1.getRadius();

    const auto position2 = s2.getPosition();
    const auto radius2   = s2.getRadius();

    return distance(position1, position2, radius1, radius2);
}

//---------------------------------------------------------------------------
//! �����Ɠ_�̋��������߂�
//---------------------------------------------------------------------------
f32 Collision3D::distance(const Line3D& line, const Vector3& point)
{
    const auto start = line.getPosition(0);
    const auto end   = line.getPosition(1);

    Vector3 dir    = end - start;
    f32     lenSqV = start.lengthSq();
    f32     t      = 0.0f;

    if(lenSqV > 0.0f) {
        t = Vector3::dot(start, point - start) / lenSqV;
    }

    Vector3 h = start + (dir * t);
    return (h - point).length();
}

//---------------------------------------------------------------------------
//! �_�Ɛ��̍ŒZ����
//---------------------------------------------------------------------------
f32 Collision3D::nearestDistanceLine(const Line3D& line, const Vector3& point)
{
    const auto start = line.getPosition(0);
    const auto end   = line.getPosition(1);

    // �����̒����A�����̑��̍��W�y��t���Z�o
    f32 len = distance(line, point);

    Vector3 lineDir = end - start;
    Vector3 SP      = point - start;
    Vector3 EP      = point - len;

    if(Vector3::dot(lineDir, SP) <= 0.0f) {
        // �n�_���̊O��
        return (start - point).length();
    }

    if(Vector3::dot(lineDir, EP) >= 0.0f) {
        // �I�_���̊O��
        return (end - point).length();
    }

    return len;
}

//---------------------------------------------------------------------------
//! �_�Ɛ����̍ŒZ����
//---------------------------------------------------------------------------
Vector3 Collision3D::nearestDistanceLineSegment(const LineSegment3D& lineSegment, const Vector3& point)
{
    const auto start = lineSegment.getPosition(0.0f);
    const auto end   = lineSegment.getPosition(1.0f);

    Vector3 lineDir = end - start;
    Vector3 dir     = point - start;

    f32 dot = Vector3::dot(lineDir, dir);
    f32 t   = dot / Vector3::dot(lineDir, lineDir);

    if(t <= 0)
        return start;
    if(t >= 1)
        return end;

    return start + lineDir * t;
}

//---------------------------------------------------------------------------
//	�� vs ��
//---------------------------------------------------------------------------
bool Collision3D::isHit(const Sphere3D& s1, const Sphere3D& s2)
{
    return distance(s1, s2) < 0.0f;
}

//---------------------------------------------------------------------------
//! �� vs �� (�֐����ŉ����o��)
//---------------------------------------------------------------------------
void Collision3D::adjustPosition(const Sphere3D& s1, const Sphere3D& s2, f32 mass1, f32 mass2)
{
    auto& position1 = s1.getPosition();
    auto& position2 = s2.getPosition();

    f32 d = distance(s1, s2);
    if(d > 0.0f)
        return;

    Vector3 hitNormal = (position2 - position1).normalize();
    d *= -1.0f;

    position2 += hitNormal * (d * mass2);
    position1 -= hitNormal * (d * mass1);
}

//---------------------------------------------------------------------------
//! �� vs �� (�֐��O�ŉ����o��)
//---------------------------------------------------------------------------
bool Collision3D::adjustPosition(
    const Sphere3D& s1,
    const Sphere3D& s2,
    Vector3&        extrusion1,
    Vector3&        extrusion2,
    f32             mass1,
    f32             mass2)
{
    const auto position1 = s1.getPosition();
    const auto position2 = s2.getPosition();

    f32 d = distance(s1, s2);
    if(d > 0.0f)
        return false;   // �Փ˂Ȃ�

    Vector3 hitNormal = (position2 - position1).normalize();
    d *= -1.0f;

    f32 s1factor = mass1 / (mass1 + mass2);
    f32 s2factor = mass2 / (mass1 + mass2);

    extrusion2 += hitNormal * d * 0.5f * s1factor;
    extrusion1 -= hitNormal * d * 0.5f * s2factor;

    return true;
}

//---------------------------------------------------------------------------
//!	�J�v�Z�� vs �J�v�Z��
//---------------------------------------------------------------------------
bool Collision3D::isHit(const Capsule3D& capsule1, const Capsule3D& capsule2)
{
    LineSegment3D lineSegment1 = LineSegment3D(capsule1.getPosition(0.0f), capsule1.getPosition(1.0f));
    LineSegment3D lineSegment2 = LineSegment3D(capsule2.getPosition(0.0f), capsule1.getPosition(1.0f));

    float distanceSq = lineSegmentDistanceSq(lineSegment1, lineSegment2);

    float radiusSum = capsule1.getRadius() + capsule2.getRadius();
    float radiusSq  = radiusSum * radiusSum;

    if(distanceSq > radiusSq)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! �J�v�Z�� vs �J�v�Z�� (�֐��O�ŉ����o��)
//---------------------------------------------------------------------------
void Collision3D::isHit(
    const Capsule3D& c1,
    const Capsule3D& c2,
    Vector3&         extrusion1,
    Vector3&         extrusion2,
    f32              mass1,
    f32              mass2)
{
    // �J�v�Z���̒��S�ʒu
    const f32 CENTER_T = 0.5f;
    Vector3   p1       = c1.getPosition(CENTER_T);
    Vector3   p2       = c2.getPosition(CENTER_T);

    // ���̌`��ɂ���
    Sphere3D s0(p1, c1.getRadius());
    Sphere3D s1(p2, c2.getRadius());

    //�@�����𖳎�����
    p1.y_ = 0.0f;
    p2.y_ = 0.0f;
    s0.setPosition(p1);
    s1.setPosition(p2);

    // �Փ˂��Ă��Ȃ��ꍇ�͉������Ȃ�
    f32 d = distance(s0, s1);
    if(d > 0.0f)
        return;

    //	�Փ˂̉\�������邩
    f32 y  = abs(p1.y_ - p2.y_);
    f32 h0 = c1.getPosition(CENTER_T).y_ - p1.y_;
    f32 h1 = c2.getPosition(CENTER_T).y_ - p2.y_;

    if(y < (h0 + h1)) {
        // �Փ˂��Ă���̂ŁA�����o���v�Z���s��
        Vector3 hitNormal = (s1.getPosition() - s1.getPosition()).normalize();
        //d *= -1.0f;
        //d *= 0.5f;

        extrusion2 += hitNormal * (d * mass2);
        extrusion1 -= hitNormal * (d * mass1);
    }
}

//---------------------------------------------------------------------------
//	�� vs �|���S��
//	@retval true	�Փ˂���
//	@retval false	�Փ˂Ȃ�
//---------------------------------------------------------------------------
bool Collision3D::isHit(
    const Sphere3D&             sphere,
    const Triangle3D&           triangle,
    [[maybe_unused]] const f32& magnifications,
    Vector3*                    nearestHitPosition,
    Vector3*                    nearestHitNormal)
{
    const Vector3 sphereCenter = sphere.getPosition();

    const auto trianglePos0 = triangle.getPosition(0);
    const auto trianglePos1 = triangle.getPosition(1);
    const auto trianglePos2 = triangle.getPosition(2);

    // �ǂ̖@���x�N�g��
    Vector3 normal = Vector3::cross(
        trianglePos1 - trianglePos0,
        trianglePos2 - trianglePos0);

    Vector3 ray = normal * sphere.getRadius();

    // �|���S������n�_�A�I�_�̃x�N�g��
    Vector3 PA = sphereCenter - trianglePos0;
    Vector3 PB = sphereCenter + ray - trianglePos0;

    f32 dotPA = Vector3::dot(PA, normal);
    f32 dotPB = Vector3::dot(PB, normal);

    if(dotPA == 0.0 && dotPB == 0.0) {
        //���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
        return false;
    }

    if((dotPA >= 0.0 && dotPB <= 0.0) ||
       (dotPA <= 0.0 && dotPB >= 0.0)) {
        //��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB

        // ��_ ���O�K��
        f32     t = abs(dotPA) / (abs(dotPA) + abs(dotPB));
        Vector3 intersection;
        intersection = sphereCenter + (ray * t);

        // �|���S���ɓ_�����邩
        Vector3 polygonVec[3];
        polygonVec[0] = trianglePos1 - trianglePos0;
        polygonVec[1] = trianglePos2 - trianglePos1;
        polygonVec[2] = trianglePos0 - trianglePos2;

        // ��_�ւ̃x�N�g��
        Vector3 intVec[3];
        intVec[0] = intersection - trianglePos0;
        intVec[1] = intersection - trianglePos1;
        intVec[2] = intersection - trianglePos2;

        Vector3 cross[3];
        cross[0] = Vector3::cross(polygonVec[0], intVec[0]);
        cross[1] = Vector3::cross(polygonVec[1], intVec[1]);
        cross[2] = Vector3::cross(polygonVec[2], intVec[2]);

        for(int i = 0; i < 3; i++) {
            cross[i] = cross[i].normalize();
        }

        f32 dot1 = Vector3::dot(cross[0], cross[1]);
        f32 dot2 = Vector3::dot(cross[0], cross[2]);

        if(dot1 > 0 && dot2 > 0) {
            if(nearestHitPosition != nullptr) {
                *nearestHitPosition = intersection;
            }

            if(nearestHitNormal != nullptr) {
                *nearestHitNormal = normal;
            }
            return true;
        }
    }

    //�������Ă��Ȃ�
    return false;
}

//---------------------------------------------------------------------------
//	�J�v�Z�� vs �|���S��
//---------------------------------------------------------------------------
bool Collision3D::isHit(
    const Capsule3D&                capsule,
    const Triangle3D&               triangle,
    [[maybe_unused]] const Vector3& collisionPosition,
    const f32&                      magnifications,
    Vector3*                        nearestHitPosition,
    Vector3*                        nearestHitNormal)
{
    const Vector3 lineCenter = capsule.getPosition(0.5f);

    auto trianglePos0 = triangle.getPosition(0);
    auto trianglePos1 = triangle.getPosition(1);
    auto trianglePos2 = triangle.getPosition(2);

    // �|���S���@���x�N�g��
    Vector3 polygonNormal = Vector3::cross(trianglePos1 - trianglePos0, trianglePos2 - trianglePos0);
    polygonNormal         = polygonNormal.normalize() * magnifications;

    // �J�v�Z���̏c��������
    LineSegment3D lineSegment = LineSegment3D(capsule.getPosition(0.0f), capsule.getPosition(1.0f));

    // �J�v�Z������
    Vector3 capsuleRay = lineSegment.getPosition(1.0f) - lineSegment.getPosition(0.0f);

    // �|���S������n�_�A�I�_�̃x�N�g��
    Vector3 PA = lineSegment.getPosition(0.0f) - trianglePos0;
    Vector3 PB = lineSegment.getPosition(1.0f) - trianglePos0;

    f32 dotPA = Vector3::dot(PA, polygonNormal);
    f32 dotPB = Vector3::dot(PB, polygonNormal);

    if(dotPA == 0.0 && dotPB == 0.0) {
        //���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
        return false;
    }

    if((dotPA >= 0.0 && dotPB <= 0.0) ||
       (dotPA <= 0.0 && dotPB >= 0.0)) {
        //��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB

        // ��_
        f32     t      = abs(dotPA) / (abs(dotPA) + abs(dotPB));
        Vector3 intsec = lineSegment.getPosition(0.0f) + (capsuleRay * t);

        // �|���S���ɓ_�����邩
        Vector3 polygonVec[3];
        polygonVec[0] = trianglePos1 - trianglePos0;
        polygonVec[1] = trianglePos2 - trianglePos1;
        polygonVec[2] = trianglePos0 - trianglePos2;

        // ��_�ւ̃x�N�g��
        Vector3 intVec[3];
        intVec[0] = intsec - trianglePos0;
        intVec[1] = intsec - trianglePos1;
        intVec[2] = intsec - trianglePos2;

        Vector3 normal[3];
        normal[0] = Vector3::cross(polygonVec[0], intVec[0]).normalize();
        normal[1] = Vector3::cross(polygonVec[1], intVec[1]).normalize();
        normal[2] = Vector3::cross(polygonVec[2], intVec[2]).normalize();

        f32 dot1 = Vector3::dot(normal[0], normal[1]);
        f32 dot2 = Vector3::dot(normal[0], normal[2]);

        if(dot1 > 0 && dot2 > 0) {
            if(nearestHitPosition != nullptr) {
                *nearestHitPosition = intsec;
            }

            if(nearestHitNormal != nullptr) {
                *nearestHitNormal = *normal;
            }
            return true;
        }
    }

    //�������Ă��Ȃ�
    return false;
}

//---------------------------------------------------------------------------
//! �J�v�Z�� vs ��
//---------------------------------------------------------------------------
bool Collision3D::isHit(const Capsule3D& capsule, const Sphere3D& sphere)
{
    LineSegment3D lineSegment(capsule.getPosition(0.0f), capsule.getPosition(1.0f));

    f32 d = nearestDistanceLine(lineSegment, sphere.getPosition());

    f32 r = capsule.getRadius() + sphere.getRadius();

    if(d <= r)
        return true;

    return false;
}

//---------------------------------------------------------------------------
//!	�ǂ���x�N�g��
//---------------------------------------------------------------------------
Vector3 Collision3D::wallShear(const Vector3& direction, const Vector3& normal, f32 ratio)
{
    f32 t = Vector3::dot(direction, normal);

    // ratio = 1.0f // �ǂ��蓙
    // ratio = 2.0f // ����
    t *= ratio;

    //Vector3 w;
    //w._x = (normal._x * t) - direction._x;
    //w.y_ = (normal.y_ * t) - direction.y_;
    //w._z = (normal._z * t) - direction._z;

    Vector3 w;
    w.x_ = direction.x_ - t * normal.x_;
    w.y_ = direction.y_ - t * normal.y_;
    w.z_ = direction.z_ - t * normal.z_;

    return w;
}

//---------------------------------------------------------------------------
//! �O�p�`�̒��Ɋ܂܂�邩�ǂ���
//---------------------------------------------------------------------------
bool Collision3D::isInside(const Triangle3D& triangle, const Vector3& p)
{
    const u32 nextIndex[3] = { 1, 2, 0 };

    for(u32 i = 0; i < 3; ++i) {
        Vector3 edge = triangle.getPosition(nextIndex[i]) - triangle.getPosition(i);
        Vector3 to   = p - triangle.getPosition(nextIndex[i]);

        Vector3 vcross = Vector3::cross(edge, to);

        if(Vector3::dot(vcross, triangle.getNormal()) < -0.00001f)
            return false;
    }
    return true;
}
