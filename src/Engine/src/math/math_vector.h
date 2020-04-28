//===========================================================================
//!	@file	math_vector.h
//!	@brief	�x�N�g��
//===========================================================================
#pragma once

// �v���g�^�C�v�錾
struct Vector4;
struct Matrix;

//===========================================================================
//!	�Q�v�f�x�N�g���\����
//===========================================================================
struct Vector2
{
	static const Vector2 ZERO;
	static const Vector2 ONE;

	static const Vector2 AXISx_;
	static const Vector2 AXISy_;

	//-----------------------------------------------------------------------
	//!	@name ������
	//-----------------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Vector2(void);

	//! �R���X�g���N�^
	Vector2(f32 x, f32 y);

	//@}

	//@}
	//-----------------------------------------------------------------------
	//!	@name ���Z
	//-----------------------------------------------------------------------
	//@{

	//! ���Z
	Vector2		operator + (const Vector2& v) const;

	//! ���Z
	Vector2		operator - (const Vector2& v) const;

	//! ��Z
	Vector2		operator * (const Vector2& v) const;

	//! �X�P�[�����O
	Vector2		operator * (f32 scale) const;

	//! �X�P�[�����O
	Vector2		operator / (f32 scale) const;

	//!	����
	static f32	dot(const Vector2& a, const Vector2& b);

	//!	�O��
	static f32	cross(const Vector2& a, const Vector2& b);

	f32 lengthSq() const
	{
		//return x_ * x_ + y_ * y_;
		return x_ * x_ + y_ * y_;
	}

	//! ���K��
	Vector2		normalize() const
	{
		f32 L = lengthSq();   // ������2������߂�

		// ������0.0�Ȃ玩�����g�̃x�N�g���̒�����0.0�ɂȂ��Ă���B
		if (L == 0.0f) {
			return *this;
		}
		return *this / sqrtf(L);
	}

	f32	x_;
	f32	y_;
};


//===========================================================================
//!	�R�v�f�x�N�g���\����
//===========================================================================
struct Vector3
{
	static const Vector3 ZERO;
	static const Vector3 ONE;

	static const Vector3 AXISx_;
	static const Vector3 AXISy_;
	static const Vector3 AXISz_;

	//-----------------------------------------------------------------------
	//!	@name ������
	//-----------------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Vector3();

	//! �R���X�g���N�^ (xyz�w��)
	Vector3(f32 x, f32 y, f32 z);

	//! �R���X�g���N�^
	Vector3(const Vector3& v);

	//! �R���X�g���N�^
	Vector3(const Vector4& v);

	//! �R���X�g���N�^
	Vector3(f32 s);

	//@}
	//-----------------------------------------------------------------------
	//!	@name ���
	//-----------------------------------------------------------------------
	//@{

	//! ���
	Vector3&	operator = (const Vector3& v);

	//! ���(Vector4)
	Vector3&	operator = (const Vector4& v);

	// �L���X�g
	operator DirectX::XMVECTOR()
	{
		return DirectX::XMVectorSet(x_, y_, z_, 0.0f);
	}

	//@}
	//-----------------------------------------------------------------------
	//!	@name �P�����Z�q
	//-----------------------------------------------------------------------
	//@{

	//! ���̒l
	Vector3		operator + () const;

	//! ���̒l
	Vector3		operator - () const;

	//@}
	//-----------------------------------------------------------------------
	//!	@name ���Z
	//-----------------------------------------------------------------------
	//@{

	//! ���Z
	Vector3		operator + (const Vector3& v) const;

	//! ���Z
	Vector3		operator - (const Vector3& v) const;

	//! ��Z
	Vector3		operator * (const Vector3& v) const;

	//! �X�P�[�����O
	Vector3		operator * (f32 scale) const;

	//! �X�P�[�����O
	Vector3		operator / (f32 scale) const;

	//! ���Z
	Vector3&	operator += (const Vector3& v);

	//! ���Z
	Vector3&	operator -= (const Vector3& v);

	//! �X�P�[�����O
	Vector3&	operator *= (f32 scale);

	//! �X�P�[�����O
	Vector3&	operator /= (f32 scale);

	//! ��r
	bool	operator == (const Vector3& v);

	//! �����̂Q��
	f32			lengthSq() const;

	//! �x�N�g���̒���
	f32			length() const;

	//! ���K��
	Vector3		normalize() const;

	//@}

	//! ���W�ϊ�(���s�ړ�����)
	Vector4		transform(const Matrix& matrix);

	//! ���W�ϊ�(�x�N�g���̉�]�B���s�ړ��Ȃ�)
	Vector3		transformNormal(const Matrix& matrix);

	//!	����
	//!	@param	a	�x�N�g��a
	//!	@param	b	�x�N�g��b
	//!	@return	���ϒl
	static f32		dot(Vector3 a, Vector3 b);

	//	�O��
	//!	@param	a	�x�N�g��a
	//!	@param	b	�x�N�g��b
	//!	@return	�O�ϒl
	static Vector3	cross(Vector3 a, Vector3 b);

	f32			x_;
	f32			y_;
	f32			z_;
};


//===========================================================================
//!	4�v�f�x�N�g��
//===========================================================================
struct Vector4
{
	static const Vector4 ZERO;
	static const Vector4 ONE;

	//! �f�t�H���g�R���X�g���N�^
	Vector4() = default;

	//! �R���X�g���N�^(4�v�f)
	Vector4(f32 x, f32 y, f32 z, f32 w)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		w_ = w;
	}

	//! �R���X�g���N�^(Vector3 + W�l)
	Vector4(const Vector3& v, f32 w)
	{
		x_ = v.x_;
		y_ = v.y_;
		z_ = v.z_;
		w_ = w;
	}

	//! ���(Vector3)
	Vector4&	operator = (const Vector3& v);

	union {
		struct {
			f32		x_;
			f32		y_;
			f32		z_;
			f32		w_;
		};
		struct {
			f32		r_;
			f32		g_;
			f32		b_;
			f32		a_;
		};
	};
};