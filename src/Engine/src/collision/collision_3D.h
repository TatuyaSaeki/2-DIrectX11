//===========================================================================
//! @file	collision_3D.h
//!	@brief	3D 当たり判定
//===========================================================================
#pragma once

//===========================================================================
//! @class Collision3D
//===========================================================================
class Collision3D : public Collision
{
public:

	//! @brief コンストラクタ
	Collision3D() = default;

	//! @brief デストラクタ
	~Collision3D() override = default;

    //-----------------------------------------------------------------------
    //! @brief  球と球の距離求める
    //! @param  [in] p1 中心座標1
    //! @param  [in] p2 中心座標2
    //! @param  [in] r1 半径1
    //! @param  [in] r2 半径2
    //! @return 距離
    //-----------------------------------------------------------------------
    static f32 distance(
        const Vector3& p1,
        const Vector3& p2,
        f32            r1,
        f32            r2);

    //-----------------------------------------------------------------------
    //! @brief  球と球の距離求める
    //! @param  [in] s0 球の形状1
    //! @param  [in] s1 球の形状2
    //! @return 距離
    //-----------------------------------------------------------------------
    static f32 distance(
        const Sphere3D& s0,
        const Sphere3D& s1);

    //---------------------------------------------------------------------------
    //! @brief  線と点の距離を求める
    //! @param  [in] line   線
    //! @param  [in] point  点の位置
    //! @return 距離
    //---------------------------------------------------------------------------
    static f32 distance(
        const Line3D&  line,
        const Vector3& point);

    //-----------------------------------------------------------------------
    //! @brief  線と点の最短距離
    //! @param  [in] line         線
    //! @param  [in] point        点の位置
    //! @return 最短距離
    //-----------------------------------------------------------------------
    static f32 nearestDistanceLine(
        const Line3D&  line,
        const Vector3& point);

    //-----------------------------------------------------------------------
    //! @brief  線分と点の最短距離
    //! @param  [in] lineSegment  線分
    //! @param  [in] point        点の位置
    //! @return 最短距離
    //-----------------------------------------------------------------------
    static Vector3 nearestDistanceLineSegment(
        const LineSegment3D& lineSegment,
        const Vector3&       point);

    //-----------------------------------------------------------------------
    //! @brief  球 vs 球
    //! @param  [in]  s1  球1
    //! @param  [in]  s2  球2
    //! @return true	  衝突あり
    //! @return false	  衝突なし
    //-----------------------------------------------------------------------
    static bool isHit(
        const Sphere3D& s1,
        const Sphere3D& s2);

    //-----------------------------------------------------------------------
    //! @brief  球 vs 球	 (関数内で押し出し)
    //! @param  [in] s1     球1
    //! @param  [in] s2     球2
    //! @param  [in] mass1  重さ1
    //! @param  [in] mass2  重さ2
    //-----------------------------------------------------------------------
    static void adjustPosition(
        const Sphere3D& s1,
        const Sphere3D& s2,
        f32             mass1 = 1.0f,
        f32             mass2 = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief  球 vs 球 (関数外で押し出し)
    //! @param  [in]  s1	     球1
    //! @param  [in]  s2	     球2
    //! @param  [out] extrusion1 押し出し値1
    //! @param  [out] extrusion2 押し出し値2
    //! @param  [in]  mass1      重さ1
    //! @param  [in]  mass2      重さ2
    //! @return true             衝突あり
    //! @return false            衝突なし
    //-----------------------------------------------------------------------
    static bool adjustPosition(
        const Sphere3D& s1,
        const Sphere3D& s2,
        Vector3&        extrusion1,
        Vector3&        extrusion2,
        f32             mass1 = 1.0f,
        f32             mass2 = 1.0f);

    //---------------------------------------------------------------------------
    //!	カプセル vs カプセル
    //! @param [in] capsule1 カプセル1
    //! @param [in] capsule2 カプセル2
    //!	@retval true	     衝突あり
    //!	@retval false	     衝突なし
    //---------------------------------------------------------------------------
    bool isHit(
        const Capsule3D& capsule1,
        const Capsule3D& capsule2);

    //-----------------------------------------------------------------------
    //! @brief  カプセル vs カプセル (関数外で押し出し)
    //! @param  [in]  c1	     カプセル1
    //! @param  [in]  c2	     カプセル2
    //! @param  [out] extrusion1 押し出し値1
    //! @param  [out] extrusion2 押し出し値2
    //! @param  [in]  mass1      重さ1
    //! @param  [in]  mass2      重さ2
    //-----------------------------------------------------------------------
    static void isHit(
        const Capsule3D& c1,
        const Capsule3D& c2,
        Vector3&         extrusion1,
        Vector3&         extrusion2,
        f32              mass1 = 1.0f,
        f32              mass2 = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief  球 vs ポリゴン
    //! @param  [in]  sphere	         球
    //! @param  [in]  triangle	         ポリゴン
    //! @param  [in]  magnifications     法線の向き (1.0 | -1.0)
    //! @param  [out] nearestHitPosition 当たった場所
    //! @param  [out] nearestHitNormal   当たった場所の法線
    //! @return true	                 衝突あり
    //! @return false	                 衝突なし
    //-----------------------------------------------------------------------
    static bool isHit(
        const Sphere3D&   sphere,
        const Triangle3D& triangle,
        const f32&        magnifications     = 1.0f,
        Vector3*          nearestHitPosition = nullptr,
        Vector3*          nearestHitNormal   = nullptr);

    //-----------------------------------------------------------------------
    //! @brief  カプセル vs ポリゴン
    //! @param  [in]  sphere	         球
    //! @param  [in]  triangle	         ポリゴン
    //! @param  [in]  magnifications     法線の向き (1.0 | -1.0)
    //! @param  [out] nearestHitPosition 当たった場所
    //! @param  [out] nearestHitNormal   当たった場所の法線
    //! @return true	                 衝突あり
    //! @return false	                 衝突なし
    //-----------------------------------------------------------------------
    static bool isHit(
        const Capsule3D&  capsule,
        const Triangle3D& triangle,
        const Vector3&    collisionPosition,
        const f32&        magnifications     = 1.0f,
        Vector3*          nearestHitPosition = nullptr,
        Vector3*          nearestHitNormal   = nullptr);

    //-----------------------------------------------------------------------
    //! @brief  カプセル vs 球
    //! @param  [in]  capsule0	カプセル
    //! @param  [in]  sphere0	球
    //! @return true	        衝突あり
    //! @return false	        衝突なし
    //-----------------------------------------------------------------------
    static bool isHit(
        const Capsule3D& capsule,
        const Sphere3D&  sphere);

    //-----------------------------------------------------------------------
    //! @brief  壁ずりベクトル
    //! @param  [in]  direction	オブジェクトの向き
    //! @param  [in]  normal	オブジェクトの法線
    //! @param  [in]  ratio	    係数 (1.0=壁ずり, 2.0=反射)
    //! @return 壁ずりベクトル
    //-----------------------------------------------------------------------
    static Vector3 wallShear(
        const Vector3& direction,
        const Vector3& normal,
        f32            ratio = 1.0f);

    //---------------------------------------------------------------------------
    //! @brief 三角形の中に含まれるかどうか
    //! param  [in] triangle  三角形
    //! param  [in] p         点の位置
    //! return true           含まれている
    //! return false　　　　　 含まれていない
    //---------------------------------------------------------------------------
    bool isInside(
        const Triangle3D& triangle,
        const Vector3&    p);
};