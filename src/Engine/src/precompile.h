//===========================================================================
//!	@flie	precompile.h
//!	@brief	�v���R���p�C���ς݃w�b�_�[
//===========================================================================
#pragma once

#include <SDKDDKVer.h>

//===========================================================================
// Windows�w�b�_�[�t�@�C��
//===========================================================================
#define NOMINMAX

#include <windows.h>

#include <wrl/client.h>
template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;

//===========================================================================
// C�����^�C���w�b�_�[�t�@�C��
//===========================================================================
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <malloc.h>

//===========================================================================
// STL�֘A
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

//---- VisualStudio2010 ��stdint.h��intsafe.h���Փ˂��邽�߂̌x���΍�
#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning(pop)

//===========================================================================
// DirectX�֘A
//===========================================================================
#include <d3d11.h>
#include <d3dcompiler.h>

// ���C�u�����t�@�C���̃����N�w��
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


#pragma warning(disable : 4201) // warning C4201 �𖳌���

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
// �ėp
//===========================================================================
#include "common/assert.h"    // �x���p
#include "common/typedef.h"   // ��`
#include "common/pointer.h"   // ���p�|�C���^

//===========================================================================
// ����
//===========================================================================
#include "input/input_keyboard.h"   // �L�[����

//===========================================================================
// �Z�p
//===========================================================================
#include "math/math_common.h"        //
#include "math/math_color.h"         // �J���[
#include "math/math_vector.h"        // �x�N�g��
#include "math/math_matrix.h"        // �s��
#include "math/math_matrix_util.h"   // �֗��֐����낢��(�̗\��)

//===========================================================================
// �`���`
//===========================================================================
#include "collision/shape/shape_base.h"   // �`����N���X
#include "collision/shape/shape_2D.h"     // 2D�`��
#include "collision/shape/shape_3D.h"     // 3D�`��

//===========================================================================
// �R���W����
//===========================================================================
#include "collision/collision.h"      // �����蔻����N���X
#include "collision/collision_2D.h"   // 2D�����蔻��
#include "collision/collision_3D.h"   // 3D�����蔻��

//===========================================================================
// �`��ݒ�
//===========================================================================
//#include "render/renderingConfig.h"   // �`��ݒ�

//===========================================================================
// GPU
//===========================================================================
#include "gpu/gpu_constant_buffer.h"   // �萔�o�b�t�@
#include "gpu/gpu_buffer.h"            // �o�b�t�@
#include "gpu/gpu_texture.h"           // �e�N�X�`��
#include "gpu/gpu_inputLayout.h"       // ���̓��C�A�E�g
#include "gpu/gpu_shader.h"            // �V�F�[�_�[
#include "gpu/gpu_command.h"           // GPU�R�}���h
#include "gpu/gpu_vertex.h"            // ���_�\����

//===========================================================================
// �`��
//===========================================================================
#include "draw/draw_functions.h"   // �`��֐����낢��
#include "draw/primitive.h"        // �v���~�e�B�u�`��

//===========================================================================
// �t�@�C��
//===========================================================================
#include "file/handle.h"
#include "file/file_watcher.h"   // �z�b�g�����[�h�p

//===========================================================================
// ���\�[�X
//===========================================================================
#include "asset/asset_base.h"              // �A�Z�b�g���N���X
#include "asset/asset_model.h"             // ���f���ǂݍ��݊��N���X
#include "asset/asset_model_fbx.h"         // ���f���ǂݍ���FBX
#include "asset/asset_model_manager.h"     // ���f���Ǘ�
#include "asset/asset_texture_manager.h"   // �e�N�X�`���Ǘ�
#include "asset/asset_manager.h"           // �A�Z�b�g�Ǘ�

//===========================================================================
// ���f��
//===========================================================================
#include "model/model.h"       // ���f�����N���X
#include "model/model_obj.h"   // OBJ���f��
#include "model/model_mqo.h"   // MQO���f��
#include "model/model_fbx.h"   // FBX���f��

//===========================================================================
// �I�u�W�F�N�g
//===========================================================================
#include "object/object_base.h"         // ���ׂẴI�u�W�F�N�g���N���X
#include "object/object_base_model.h"   // (���̎q��)���f�����g�p����I�u�W�F�N�g���N���X

//----- light
#include "object/light/light_base.h"          // ���C�g���N���X
#include "object/light/light_directional.h"   // ���s����
#include "object/light/light_point.h"         // �_����
#include "object/light/light_spot.h"          // �X�|�b�g���C�g

#include "render/ibl.h"          // IBL
#include "render/shadow_map.h"   // �V���h�E�}�b�v
#include "render/toon.h"         // �g�D�[���}�b�s���O

//---- camera
#include "object/camera/camera.h"       // �J�������
#include "object/camera/camera_fps.h"   // 1�l�̃J����
#include "object/camera/camera_tps.h"   // 3�l�̃J����

//---- filter
#include "filter/filter.h"                // �t�B���^���N���X
#include "filter/filter_glare.h"          // �O���A�t�B���^�[
#include "filter/filter_tone_mapping.h"   // �g�[���}�b�s���O

//===========================================================================
// �}�l�[�W���[
//===========================================================================
#include "manager/manager_base.h"        // �}�l�[�W���[���N���X
#include "manager/manager_task.h"        // �^�X�N�}�l�[�W���[
#include "manager/manager_shader.h"      // �V�F�[�_�[�}�l�[�W���[
#include "manager/manager_filter.h"      // �t�B���^�}�l�[�W���[
#include "manager/manager_collision.h"   // �����蔻��}�l�[�W���[
#include "manager/manager_camera.h"      // �J�����}�l�[�W���[
#include "manager/manager_light.h"       // ���C�g�}�l�[�W���[

//===========================================================================
// �V�[��
//===========================================================================
#include "scene/scene_base.h"        // �V�[�����N���X
#include "manager/manager_scene.h"   // �V�[���}�l�[�W���[

#include "system/directX.h"         // DirectX11
#include "system/system_render.h"   // �V�X�e���`��
