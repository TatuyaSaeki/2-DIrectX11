//===========================================================================
//!	@file	typedef.h
//!	@brief	型定義
//===========================================================================
#pragma once
#include "pointer.h"

using s8  = std::int8_t;     //!< 符号あり 8bit整数
using u8  = std::uint8_t;    //!< 符号なし 8bit整数
using s16 = std::int16_t;    //!< 符号あり16bit整数
using u16 = std::uint16_t;   //!< 符号なし16bit整数
using s32 = std::int32_t;    //!< 符号あり32bit整数
using u32 = std::uint32_t;   //!< 符号なし32bit整数
using s64 = std::int64_t;    //!< 符号あり64bit整数
using u64 = std::uint64_t;   //!< 符号なし64bit整数
using f32 = float;           //!< 単精度浮動小数点数
using f64 = double;          //!< 倍精度浮動小数点数

//---------------------------------------------------------------------------
// GPUスロット番号
//---------------------------------------------------------------------------
const u32 TEX_ALBEDO       = 0;    //!< アルベド
const u32 TEX_ROUGHNESS    = 1;    //!< ラフネス
const u32 TEX_METALNESS    = 2;    //!< メタルネス
const u32 TEX_NORMAL       = 3;    //!< 法線
const u32 TEX_AO           = 4;    //!< アンビエント
const u32 TEX_SUB          = 5;    //!<
const u32 TEX_IBL_DIFFUSE  = 6;    //!< IBLディフーズ
const u32 TEX_IBL_SPECULAR = 7;    //!< IBLスペキュラ
const u32 TEX_DEPTH        = 10;   //!< 深度
const u32 TEX_TOON         = 11;   //!< トゥーン
const u32 TEX_THIN         = 12;   //!< 薄膜
const u32 TEX_CUBE         = 13;   //!< キューブマップ
const u32 TEX_CUBE_DEPTH   = 14;   //!< キューブマップ深度
const u32 TEX_NORMAL_1     = 20;   //!< 法線1
const u32 TEX_NORMAL_2     = 21;   //!< 法線2
const u32 TEX_HEIGHT_1     = 22;   //!< 高さ1
const u32 TEX_HEIGHT_2     = 23;   //!< 高さ2

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 初期化
//---------------------------------------------------------------------------
namespace SAFE_TASK {
template<typename T>
bool initialize(T* p)
{
    if(!p) {
        GM_ASSERT_MESSAGE(false, "ポインタエラー");
        return false;
    }

    if(!p->initialize()) {
        GM_ASSERT_MESSAGE(false, "初期化失敗");
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
template<typename T>
void cleanup(T* p)
{
    if(!p) {
        GM_ASSERT_MESSAGE(false, "ポインタエラー");
		return;
	}

    p->cleanup();
}
}   // namespace SAFE_TASK
