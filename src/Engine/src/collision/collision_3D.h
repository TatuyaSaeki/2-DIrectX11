//===========================================================================
//! @file	collision_3D.h
//!	@brief	3D �����蔻��
//===========================================================================
#pragma once

//===========================================================================
//! @class Collision3D
//===========================================================================
class Collision3D : public Collision
{
public:

	//! @brief �R���X�g���N�^
	Collision3D() = default;

	//! @brief �f�X�g���N�^
	~Collision3D() override = default;

    //-----------------------------------------------------------------------
    //! @brief  ���Ƌ��̋������߂�
    //! @param  [in] p1 ���S���W1
    //! @param  [in] p2 ���S���W2
    //! @param  [in] r1 ���a1
    //! @param  [in] r2 ���a2
    //! @return ����
    //-----------------------------------------------------------------------
    static f32 distance(
        const Vector3& p1,
        const Vector3& p2,
        f32            r1,
        f32            r2);

    //-----------------------------------------------------------------------
    //! @brief  ���Ƌ��̋������߂�
    //! @param  [in] s0 ���̌`��1
    //! @param  [in] s1 ���̌`��2
    //! @return ����
    //-----------------------------------------------------------------------
    static f32 distance(
        const Sphere3D& s0,
        const Sphere3D& s1);

    //---------------------------------------------------------------------------
    //! @brief  ���Ɠ_�̋��������߂�
    //! @param  [in] line   ��
    //! @param  [in] point  �_�̈ʒu
    //! @return ����
    //---------------------------------------------------------------------------
    static f32 distance(
        const Line3D&  line,
        const Vector3& point);

    //-----------------------------------------------------------------------
    //! @brief  ���Ɠ_�̍ŒZ����
    //! @param  [in] line         ��
    //! @param  [in] point        �_�̈ʒu
    //! @return �ŒZ����
    //-----------------------------------------------------------------------
    static f32 nearestDistanceLine(
        const Line3D&  line,
        const Vector3& point);

    //-----------------------------------------------------------------------
    //! @brief  �����Ɠ_�̍ŒZ����
    //! @param  [in] lineSegment  ����
    //! @param  [in] point        �_�̈ʒu
    //! @return �ŒZ����
    //-----------------------------------------------------------------------
    static Vector3 nearestDistanceLineSegment(
        const LineSegment3D& lineSegment,
        const Vector3&       point);

    //-----------------------------------------------------------------------
    //! @brief  �� vs ��
    //! @param  [in]  s1  ��1
    //! @param  [in]  s2  ��2
    //! @return true	  �Փ˂���
    //! @return false	  �Փ˂Ȃ�
    //-----------------------------------------------------------------------
    static bool isHit(
        const Sphere3D& s1,
        const Sphere3D& s2);

    //-----------------------------------------------------------------------
    //! @brief  �� vs ��	 (�֐����ŉ����o��)
    //! @param  [in] s1     ��1
    //! @param  [in] s2     ��2
    //! @param  [in] mass1  �d��1
    //! @param  [in] mass2  �d��2
    //-----------------------------------------------------------------------
    static void adjustPosition(
        const Sphere3D& s1,
        const Sphere3D& s2,
        f32             mass1 = 1.0f,
        f32             mass2 = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief  �� vs �� (�֐��O�ŉ����o��)
    //! @param  [in]  s1	     ��1
    //! @param  [in]  s2	     ��2
    //! @param  [out] extrusion1 �����o���l1
    //! @param  [out] extrusion2 �����o���l2
    //! @param  [in]  mass1      �d��1
    //! @param  [in]  mass2      �d��2
    //! @return true             �Փ˂���
    //! @return false            �Փ˂Ȃ�
    //-----------------------------------------------------------------------
    static bool adjustPosition(
        const Sphere3D& s1,
        const Sphere3D& s2,
        Vector3&        extrusion1,
        Vector3&        extrusion2,
        f32             mass1 = 1.0f,
        f32             mass2 = 1.0f);

    //---------------------------------------------------------------------------
    //!	�J�v�Z�� vs �J�v�Z��
    //! @param [in] capsule1 �J�v�Z��1
    //! @param [in] capsule2 �J�v�Z��2
    //!	@retval true	     �Փ˂���
    //!	@retval false	     �Փ˂Ȃ�
    //---------------------------------------------------------------------------
    bool isHit(
        const Capsule3D& capsule1,
        const Capsule3D& capsule2);

    //-----------------------------------------------------------------------
    //! @brief  �J�v�Z�� vs �J�v�Z�� (�֐��O�ŉ����o��)
    //! @param  [in]  c1	     �J�v�Z��1
    //! @param  [in]  c2	     �J�v�Z��2
    //! @param  [out] extrusion1 �����o���l1
    //! @param  [out] extrusion2 �����o���l2
    //! @param  [in]  mass1      �d��1
    //! @param  [in]  mass2      �d��2
    //-----------------------------------------------------------------------
    static void isHit(
        const Capsule3D& c1,
        const Capsule3D& c2,
        Vector3&         extrusion1,
        Vector3&         extrusion2,
        f32              mass1 = 1.0f,
        f32              mass2 = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief  �� vs �|���S��
    //! @param  [in]  sphere	         ��
    //! @param  [in]  triangle	         �|���S��
    //! @param  [in]  magnifications     �@���̌��� (1.0 | -1.0)
    //! @param  [out] nearestHitPosition ���������ꏊ
    //! @param  [out] nearestHitNormal   ���������ꏊ�̖@��
    //! @return true	                 �Փ˂���
    //! @return false	                 �Փ˂Ȃ�
    //-----------------------------------------------------------------------
    static bool isHit(
        const Sphere3D&   sphere,
        const Triangle3D& triangle,
        const f32&        magnifications     = 1.0f,
        Vector3*          nearestHitPosition = nullptr,
        Vector3*          nearestHitNormal   = nullptr);

    //-----------------------------------------------------------------------
    //! @brief  �J�v�Z�� vs �|���S��
    //! @param  [in]  sphere	         ��
    //! @param  [in]  triangle	         �|���S��
    //! @param  [in]  magnifications     �@���̌��� (1.0 | -1.0)
    //! @param  [out] nearestHitPosition ���������ꏊ
    //! @param  [out] nearestHitNormal   ���������ꏊ�̖@��
    //! @return true	                 �Փ˂���
    //! @return false	                 �Փ˂Ȃ�
    //-----------------------------------------------------------------------
    static bool isHit(
        const Capsule3D&  capsule,
        const Triangle3D& triangle,
        const Vector3&    collisionPosition,
        const f32&        magnifications     = 1.0f,
        Vector3*          nearestHitPosition = nullptr,
        Vector3*          nearestHitNormal   = nullptr);

    //-----------------------------------------------------------------------
    //! @brief  �J�v�Z�� vs ��
    //! @param  [in]  capsule0	�J�v�Z��
    //! @param  [in]  sphere0	��
    //! @return true	        �Փ˂���
    //! @return false	        �Փ˂Ȃ�
    //-----------------------------------------------------------------------
    static bool isHit(
        const Capsule3D& capsule,
        const Sphere3D&  sphere);

    //-----------------------------------------------------------------------
    //! @brief  �ǂ���x�N�g��
    //! @param  [in]  direction	�I�u�W�F�N�g�̌���
    //! @param  [in]  normal	�I�u�W�F�N�g�̖@��
    //! @param  [in]  ratio	    �W�� (1.0=�ǂ���, 2.0=����)
    //! @return �ǂ���x�N�g��
    //-----------------------------------------------------------------------
    static Vector3 wallShear(
        const Vector3& direction,
        const Vector3& normal,
        f32            ratio = 1.0f);

    //---------------------------------------------------------------------------
    //! @brief �O�p�`�̒��Ɋ܂܂�邩�ǂ���
    //! param  [in] triangle  �O�p�`
    //! param  [in] p         �_�̈ʒu
    //! return true           �܂܂�Ă���
    //! return false�@�@�@�@�@ �܂܂�Ă��Ȃ�
    //---------------------------------------------------------------------------
    bool isInside(
        const Triangle3D& triangle,
        const Vector3&    p);
};