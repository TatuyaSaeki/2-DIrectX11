//===========================================================================
//! @file	collision_2D.h
//!	@brief	2D 当たり判定
//===========================================================================
#pragma once

//===========================================================================
//!	@class Collision2D
//===========================================================================
class Collision2D : public Collision
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    Collision2D() = default;

    //! @brief デストラクタ
    ~Collision2D() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	判定
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief 2D 点vs点
    //!	@param	[in]	point1	Point2Dクラス
    //!	@param	[in]	point2	Point2Dクラス
    //!	@return	[out]	true	当たっている
    //!	@return	[out]	false	当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Point2D& point1, const Point2D& point2);

	//-----------------------------------------------------------------------
    //! @brief 2D 点vs円
    //!	@param	[in]	point		Point2Dクラス
    //!	@param	[in]	circle		Circle2Dクラス
    //!	@param	[out]	outPosition	ヒットポジション
    //!	@return	[out]	true		当たっている
    //!	@return	[out]	false		当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Point2D& point, const Circle2D& circle, f32* pLength = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2D 円vs円
    //!	@param	[in]	circle1	Circle2Dクラス
    //!	@param	[in]	circle2	Circle2Dクラス
    //!	@return	[out]	true	当たっている
    //!	@return	[out]	false	当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle1, const Circle2D& circle2);

	//-----------------------------------------------------------------------
    //! @brief 2D 線分vs線分
    //!	@param	[in]	line1		    LineSegment2Dクラス
    //!	@param	[in]	line2		    LineSegment2Dクラス
    //!	@param	[out]	outHitPosition  ヒットポジション
    //!	@return	[out]	true	    	当たっている
    //!	@return	[out]	false	    	当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const LineSegment2D& line1, const LineSegment2D& line2, Vector2* outHitPosition = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2D 矩形vs矩形
    //!	@param	[in]	rect1	Rect2Dクラス
    //!	@param	[in]	rect2	Rect2Dクラス
    //!	@return	[out]	true	当たっている
    //!	@return	[out]	false	当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Rect2D& rect1, const Rect2D& rect2);

	//-----------------------------------------------------------------------
    //! @brief 2Dの円 と 3D座標を2D座標にしたもの (Matrix)
    //!	カメラ渡したほうがいいかも
    //!	@param	[in]	circle			    Circle2Dクラス
    //!	@param	[in]	matWorld		    3Dの方のワールド行列
    //!	@param	[in]	matView			    ビュー行列
    //!	@param	[in]	matProj			    プロジェクション行列
    //!	@param	[out]	outLenght		    2D座標でのヒット距離
    //!	@param	[out]	outScreenPosition	ヒットポジション
    //!	@return	[out]	true		     	当たっている
    //!	@return	[out]	false		     	当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle, const Matrix& matWorld, const Matrix& matView, const Matrix& matProj, f32* outLenght = nullptr, Vector2* outScreenPosition = nullptr);

	//-----------------------------------------------------------------------
    //! @brief 2Dの円 と 3D座標を2D座標にしたもの (Vector3)
    //!	@param	[in]	circle0		Circle2Dクラス
    //!	@param	[in]	position0	3Dの方の位置
    //!	@param	[in]	matView		ビュー行列
    //!	@param	[in]	matProj		プロジェクション行列
    //!	@return	[out]	true		当たっている
    //!	@return	[out]	false		当たっていない
	//-----------------------------------------------------------------------
    bool isHit(const Circle2D& circle0, const Vector3& position0, const Matrix& matView, const Matrix& matProj);

    //@}

	//-----------------------------------------------------------------------
	//! ここじゃない気がする
    //! @brief  画面サイズのスクリーン行列を返す
	//! @param  [in] w ウィンドウ幅
	//! @param  [in] h ウィンドウ高さ
	//! @return スクリーン行列
	//-----------------------------------------------------------------------
    Matrix conversion2D(f32 w, f32 h);
};