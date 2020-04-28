//===========================================================================
//! @file	collision_2D.h
//!	@brief	2D �����蔻��
//===========================================================================
#pragma once

//===========================================================================
//!	@class Collision2D
//===========================================================================
class Collision2D : public Collision
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Collision2D() = default;

    //! @brief �f�X�g���N�^
    ~Collision2D() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	����
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief 2D �_vs�_
    //!	@param	[in]	point1	Point2D�N���X
    //!	@param	[in]	point2	Point2D�N���X
    //!	@return	[out]	true	�������Ă���
    //!	@return	[out]	false	�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Point2D& point1, const Point2D& point2);

	//-----------------------------------------------------------------------
    //! @brief 2D �_vs�~
    //!	@param	[in]	point		Point2D�N���X
    //!	@param	[in]	circle		Circle2D�N���X
    //!	@param	[out]	outPosition	�q�b�g�|�W�V����
    //!	@return	[out]	true		�������Ă���
    //!	@return	[out]	false		�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Point2D& point, const Circle2D& circle, f32* pLength = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2D �~vs�~
    //!	@param	[in]	circle1	Circle2D�N���X
    //!	@param	[in]	circle2	Circle2D�N���X
    //!	@return	[out]	true	�������Ă���
    //!	@return	[out]	false	�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle1, const Circle2D& circle2);

	//-----------------------------------------------------------------------
    //! @brief 2D ����vs����
    //!	@param	[in]	line1		    LineSegment2D�N���X
    //!	@param	[in]	line2		    LineSegment2D�N���X
    //!	@param	[out]	outHitPosition  �q�b�g�|�W�V����
    //!	@return	[out]	true	    	�������Ă���
    //!	@return	[out]	false	    	�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const LineSegment2D& line1, const LineSegment2D& line2, Vector2* outHitPosition = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2D ��`vs��`
    //!	@param	[in]	rect1	Rect2D�N���X
    //!	@param	[in]	rect2	Rect2D�N���X
    //!	@return	[out]	true	�������Ă���
    //!	@return	[out]	false	�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Rect2D& rect1, const Rect2D& rect2);

	//-----------------------------------------------------------------------
    //! @brief 2D�̉~ �� 3D���W��2D���W�ɂ������� (Matrix)
    //!	�J�����n�����ق�����������
    //!	@param	[in]	circle			    Circle2D�N���X
    //!	@param	[in]	matWorld		    3D�̕��̃��[���h�s��
    //!	@param	[in]	matView			    �r���[�s��
    //!	@param	[in]	matProj			    �v���W�F�N�V�����s��
    //!	@param	[out]	outLenght		    2D���W�ł̃q�b�g����
    //!	@param	[out]	outScreenPosition	�q�b�g�|�W�V����
    //!	@return	[out]	true		     	�������Ă���
    //!	@return	[out]	false		     	�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle, const Matrix& matWorld, const Matrix& matView, const Matrix& matProj, f32* outLenght = nullptr, Vector2* outScreenPosition = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2D�̉~ �� 3D���W��2D���W�ɂ������� (Vector3)
    //!	@param	[in]	circle0		Circle2D�N���X
    //!	@param	[in]	position0	3D�̕��̈ʒu
    //!	@param	[in]	matView		�r���[�s��
    //!	@param	[in]	matProj		�v���W�F�N�V�����s��
    //!	@return	[out]	true		�������Ă���
    //!	@return	[out]	false		�������Ă��Ȃ�
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle0, const Vector3& position0, const Matrix& matView, const Matrix& matProj);

    //@}

	//-----------------------------------------------------------------------
	//! ��������Ȃ��C������
    //! @brief  ��ʃT�C�Y�̃X�N���[���s���Ԃ�
	//! @param  [in] w �E�B���h�E��
	//! @param  [in] h �E�B���h�E����
	//! @return �X�N���[���s��
	//-----------------------------------------------------------------------
    Matrix conversion2D(f32 w, f32 h);
};