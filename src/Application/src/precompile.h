//===========================================================================
//!	@file	precompile.h
//!	@brief	プリコンパイルヘッダー済みヘッダー
//===========================================================================
#pragma once

//===========================================================================
// エンジンのプリコンパイルヘッダーヘッダー
//===========================================================================
#include "Engine/src/precompile.h"

#include "render/renderingConfig.h"   // 描画設定

//===========================================================================
// オブジェクト
//===========================================================================
#include "object/object_floor.h"
#include "object/object_skybox.h"
#include "object/object_sea.h"
#include "object/object_water.h"

//===========================================================================
// シーン
//===========================================================================
#include "scene/demo/demo_scene_base.h"         // 基底
#include "scene/demo/scene_light.h"             // 平行光源+点光源+スポットライト
#include "scene/demo/scene_cube_shadow_map.h"   // 全方位シャドウマップ(使ってない)
#include "scene/demo/scene_sea.h"               // 海
#include "scene/demo/scene_sea2.h"              // 海(使ってない)
#include "scene/demo/scene_water.h"             // 水
#include "scene/demo/demo_scene_manager.h"      // デモシーン管理クラス

#include "scene/scene_demo.h"   // デモシーン

#include "scene/app_manager_scene.h"   // アプリ側シーン管理クラス

//===========================================================================
// メイン
//===========================================================================
#include "app_main.h"
#include "main.h"
