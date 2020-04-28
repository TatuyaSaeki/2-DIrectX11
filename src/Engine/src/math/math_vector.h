//===========================================================================
//!	@file	math_vector.h
//!	@brief	ベクトル
//===========================================================================
#pragma once

// プロトタイプ宣言
struct Vector4;
struct Matrix;

//===========================================================================
//!	２要素ベクトル構造体
//===========================================================================
struct Vector2
{
	static const Vector2 ZERO;
	static const Vector2 ONE;

	static const Vector2 AXISx_;
	static const Vector2 AXISy_;

	//-----------------------------------------------------------------------
	//!	@name 初期化
	//-----------------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Vector2(void);

	//! コンストラクタ
	Vector2(f32 x, f32 y);

	//@}

	//@}
	//-----------------------------------------------------------------------
	//!	@name 演算
	//-----------------------------------------------------------------------
	//@{

	//! 加算
	Vector2		operator + (const Vector2& v) const;

	//! 減算
	Vector2		operator - (const Vector2& v) const;

	//! 乗算
	Vector2		operator * (const Vector2& v) const;

	//! スケーリング
	Vector2		operator * (f32 scale) const;

	//! スケーリング
	Vector2		operator / (f32 scale) const;

	//!	内積
	static f32	dot(const Vector2& a, const Vector2& b);

	//!	外積
	static f32	cross(const Vector2& a, const Vector2& b);

	f32 lengthSq() const
	{
		//return x_ * x_ + y_ * y_;
		return x_ * x_ + y_ * y_;
	}

	//! 正規化
	Vector2		normalize() const
	{
		f32 L = lengthSq();   // 長さの2乗を求める

		// 長さが0.0なら自分自身のベクトルの長さは0.0になっている。
		if (L == 0.0f) {
			return *this;
		}
		return *this / sqrtf(L);
	}

	f32	x_;
	f32	y_;
};


//===========================================================================
//!	３要素ベクトル構造体
//===========================================================================
struct Vector3
{
	static const Vector3 ZERO;
	static const Vector3 ONE;

	static const Vector3 AXISx_;
	static const Vector3 AXISy_;
	static const Vector3 AXISz_;

	//-----------------------------------------------------------------------
	//!	@name 初期化
	//-----------------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Vector3();

	//! コンストラクタ (xyz指定)
	Vector3(f32 x, f32 y, f32 z);

	//! コンストラクタ
	Vector3(const Vector3& v);

	//! コンストラクタ
	Vector3(const Vector4& v);

	//! コンストラクタ
	Vector3(f32 s);

	//@}
	//-----------------------------------------------------------------------
	//!	@name 代入
	//-----------------------------------------------------------------------
	//@{

	//! 代入
	Vector3&	operator = (const Vector3& v);

	//! 代入(Vector4)
	Vector3&	operator = (const Vector4& v);

	// キャスト
	operator DirectX::XMVECTOR()
	{
		return DirectX::XMVectorSet(x_, y_, z_, 0.0f);
	}

	//@}
	//-----------------------------------------------------------------------
	//!	@name 単項演算子
	//-----------------------------------------------------------------------
	//@{

	//! 正の値
	Vector3		operator + () const;

	//! 負の値
	Vector3		operator - () const;

	//@}
	//-----------------------------------------------------------------------
	//!	@name 演算
	//-----------------------------------------------------------------------
	//@{

	//! 加算
	Vector3		operator + (const Vector3& v) const;

	//! 減算
	Vector3		operator - (const Vector3& v) const;

	//! 乗算
	Vector3		operator * (const Vector3& v) const;

	//! スケーリング
	Vector3		operator * (f32 scale) const;

	//! スケーリング
	Vector3		operator / (f32 scale) const;

	//! 加算
	Vector3&	operator += (const Vector3& v);

	//! 減算
	Vector3&	operator -= (const Vector3& v);

	//! スケーリング
	Vector3&	operator *= (f32 scale);

	//! スケーリング
	Vector3&	operator /= (f32 scale);

	//! 比較
	bool	operator == (const Vector3& v);

	//! 長さの２乗
	f32			lengthSq() const;

	//! ベクトルの長さ
	f32			length() const;

	//! 正規化
	Vector3		normalize() const;

	//@}

	//! 座標変換(平行移動あり)
	Vector4		transform(const Matrix& matrix);

	//! 座標変換(ベクトルの回転。平行移動なし)
	Vector3		transformNormal(const Matrix& matrix);

	//!	内積
	//!	@param	a	ベクトルa
	//!	@param	b	ベクトルb
	//!	@return	内積値
	static f32		dot(Vector3 a, Vector3 b);

	//	外積
	//!	@param	a	ベクトルa
	//!	@param	b	ベクトルb
	//!	@return	外積値
	static Vector3	cross(Vector3 a, Vector3 b);

	f32			x_;
	f32			y_;
	f32			z_;
};


//===========================================================================
//!	4要素ベクトル
//===========================================================================
struct Vector4
{
	static const Vector4 ZERO;
	static const Vector4 ONE;

	//! デフォルトコンストラクタ
	Vector4() = default;

	//! コンストラクタ(4要素)
	Vector4(f32 x, f32 y, f32 z, f32 w)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		w_ = w;
	}

	//! コンストラクタ(Vector3 + W値)
	Vector4(const Vector3& v, f32 w)
	{
		x_ = v.x_;
		y_ = v.y_;
		z_ = v.z_;
		w_ = w;
	}

	//! 代入(Vector3)
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