//===========================================================================
//!	@flie	precompile.h
//!	@brief	プリコンパイル済みヘッダー
//===========================================================================
#pragma once

#include <SDKDDKVer.h>

//===========================================================================
// Windowsヘッダーファイル
//===========================================================================
#define NOMINMAX

#include <windows.h>

#include <wrl/client.h>
template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;

//===========================================================================
// Cランタイムヘッダーファイル
//===========================================================================
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <malloc.h>

//===========================================================================
// STL関連
//===========================================================================
#include <algorithm>
#include <array>
#include <exception>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <fstream>
#pragma warning(push)
#pragma warning(disable : 4267)
#include <vector>
#pragma warning(pop)
#include <unordered_map>

//---- VisualStudio2010 のstdint.hとintsafe.hが衝突するための警告対策
#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning(pop)

//===========================================================================
// DirectX関連
//===========================================================================
#include <d3d11.h>
#include <d3dcompiler.h>

// ライブラリファイルのリンク指定
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//---- DirectX Math
#include <DirectXMath.h>

//---- DirectX ToolKit
#include "DirectXTK-master/Inc/CommonStates.h"
#include "DirectXTK-master/Inc/DDSTextureLoader.h"
#include "DirectXTK-master/Inc/DirectXHelpers.h"
#include "DirectXTK-master/Inc/Effects.h"
#include "DirectXTK-master/Inc/GeometricPrimitive.h"
#include "DirectXTK-master/Inc/Model.h"
#include "DirectXTK-master/Inc/PrimitiveBatch.h"
#include "DirectXTK-master/Inc/ScreenGrab.h"
#include "DirectXTK-master/Inc/SimpleMath.h"
#include "DirectXTK-master/Inc/SpriteBatch.h"
#include "DirectXTK-master/Inc/SpriteFont.h"
#include "DirectXTK-master/Inc/VertexTypes.h"
#include "DirectXTK-master/Inc/WICTextureLoader.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK-master/Bin/Desktop_2017_Win10/x64/Debug/DirectXTK.lib")
#else
#pragma comment(lib, "DirectXTK-master/Bin/Desktop_2017_Win10/x64/Release/DirectXTK.lib")
#endif


#pragma warning(disable : 4201) // warning C4201 を無効化

//===========================================================================
//	imgui
//===========================================================================
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_dx11.h"
#include "imgui/examples/imgui_impl_win32.h"

//===========================================================================
// OpenSource OpenFBX
//===========================================================================
#include "OpenFBX/src/ofbx.h"

//===========================================================================
// 汎用
//===========================================================================
#include "common/assert.h"    // 警告用
#include "common/typedef.h"   // 定義
#include "common/pointer.h"   // 中継ポインタ

//===========================================================================
// 入力
//===========================================================================
#include "input/input_keyboard.h"   // キー入力

//===========================================================================
// 算術
//===========================================================================
#include "math/math_common.h"        //
#include "math/math_color.h"         // カラー
#include "math/math_vector.h"        // ベクトル
#include "math/math_matrix.h"        // 行列
#include "math/math_matrix_util.h"   // 便利関数いろいろ(の予定)

//===========================================================================
// 形状定義
//===========================================================================
#include "collision/shape/shape_base.h"   // 形状基底クラス
#include "collision/shape/shape_2D.h"     // 2D形状
#include "collision/shape/shape_3D.h"     // 3D形状

//===========================================================================
// コリジョン
//===========================================================================
#include "collision/collision.h"      // 当たり判定基底クラス
#include "collision/collision_2D.h"   // 2D当たり判定
#include "collision/collision_3D.h"   // 3D当たり判定

//===========================================================================
// 描画設定
//===========================================================================
//#include "render/renderingConfig.h"   // 描画設定

//===========================================================================
// GPU
//===========================================================================
#include "gpu/gpu_constant_buffer.h"   // 定数バッファ
#include "gpu/gpu_buffer.h"            // バッファ
#include "gpu/gpu_texture.h"           // テクスチャ
#include "gpu/gpu_inputLayout.h"       // 入力レイアウト
#include "gpu/gpu_shader.h"            // シェーダー
#include "gpu/gpu_command.h"           // GPUコマンド
#include "gpu/gpu_vertex.h"            // 頂点構造体

//===========================================================================
// 描画
//===========================================================================
#include "draw/draw_functions.h"   // 描画関数いろいろ
#include "draw/primitive.h"        // プリミティブ描画

//===========================================================================
// ファイル
//===========================================================================
#include "file/handle.h"
#include "file/file_watcher.h"   // ホットリロード用

//===========================================================================
// リソース
//===========================================================================
#include "asset/asset_base.h"              // アセット基底クラス
#include "asset/asset_model.h"             // モデル読み込み基底クラス
#include "asset/asset_model_fbx.h"         // モデル読み込みFBX
#include "asset/asset_model_manager.h"     // モデル管理
#include "asset/asset_texture_manager.h"   // テクスチャ管理
#include "asset/asset_manager.h"           // アセット管理

//===========================================================================
// モデル
//===========================================================================
#include "model/model.h"       // モデル基底クラス
#include "model/model_obj.h"   // OBJモデル
#include "model/model_mqo.h"   // MQOモデル
#include "model/model_fbx.h"   // FBXモデル

//===========================================================================
// オブジェクト
//===========================================================================
#include "object/object_base.h"         // すべてのオブジェクト基底クラス
#include "object/object_base_model.h"   // (↑の子供)モデルを使用するオブジェクト基底クラス

//----- light
#include "object/light/light_base.h"          // ライト基底クラス
#include "object/light/light_directional.h"   // 平行光源
#include "object/light/light_point.h"         // 点光源
#include "object/light/light_spot.h"          // スポットライト

#include "render/ibl.h"          // IBL
#include "render/shadow_map.h"   // シャドウマップ
#include "render/toon.h"         // トゥーンマッピング

//---- camera
#include "object/camera/camera.h"       // カメラ基底
#include "object/camera/camera_fps.h"   // 1人称カメラ
#include "object/camera/camera_tps.h"   // 3人称カメラ

//---- filter
#include "filter/filter.h"                // フィルタ基底クラス
#include "filter/filter_glare.h"          // グレアフィルター
#include "filter/filter_tone_mapping.h"   // トーンマッピング

//===========================================================================
// マネージャー
//===========================================================================
#include "manager/manager_base.h"        // マネージャー基底クラス
#include "manager/manager_task.h"        // タスクマネージャー
#include "manager/manager_shader.h"      // シェーダーマネージャー
#include "manager/manager_filter.h"      // フィルタマネージャー
#include "manager/manager_collision.h"   // 当たり判定マネージャー
#include "manager/manager_camera.h"      // カメラマネージャー
#include "manager/manager_light.h"       // ライトマネージャー

//===========================================================================
// シーン
//===========================================================================
#include "scene/scene_base.h"        // シーン基底クラス
#include "manager/manager_scene.h"   // シーンマネージャー

#include "system/directX.h"         // DirectX11
#include "system/system_render.h"   // システム描画
