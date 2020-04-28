//===========================================================================
//!	@file	typedef.h
//!	@brief	�^��`
//===========================================================================
#pragma once
#include "pointer.h"

using s8  = std::int8_t;     //!< �������� 8bit����
using u8  = std::uint8_t;    //!< �����Ȃ� 8bit����
using s16 = std::int16_t;    //!< ��������16bit����
using u16 = std::uint16_t;   //!< �����Ȃ�16bit����
using s32 = std::int32_t;    //!< ��������32bit����
using u32 = std::uint32_t;   //!< �����Ȃ�32bit����
using s64 = std::int64_t;    //!< ��������64bit����
using u64 = std::uint64_t;   //!< �����Ȃ�64bit����
using f32 = float;           //!< �P���x���������_��
using f64 = double;          //!< �{���x���������_��

//---------------------------------------------------------------------------
// GPU�X���b�g�ԍ�
//---------------------------------------------------------------------------
const u32 TEX_ALBEDO       = 0;    //!< �A���x�h
const u32 TEX_ROUGHNESS    = 1;    //!< ���t�l�X
const u32 TEX_METALNESS    = 2;    //!< ���^���l�X
const u32 TEX_NORMAL       = 3;    //!< �@��
const u32 TEX_AO           = 4;    //!< �A���r�G���g
const u32 TEX_SUB          = 5;    //!<
const u32 TEX_IBL_DIFFUSE  = 6;    //!< IBL�f�B�t�[�Y
const u32 TEX_IBL_SPECULAR = 7;    //!< IBL�X�y�L����
const u32 TEX_DEPTH        = 10;   //!< �[�x
const u32 TEX_TOON         = 11;   //!< �g�D�[��
const u32 TEX_THIN         = 12;   //!< ����
const u32 TEX_CUBE         = 13;   //!< �L���[�u�}�b�v
const u32 TEX_CUBE_DEPTH   = 14;   //!< �L���[�u�}�b�v�[�x
const u32 TEX_NORMAL_1     = 20;   //!< �@��1
const u32 TEX_NORMAL_2     = 21;   //!< �@��2
const u32 TEX_HEIGHT_1     = 22;   //!< ����1
const u32 TEX_HEIGHT_2     = 23;   //!< ����2

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
namespace SAFE_TASK {
template<typename T>
bool initialize(T* p)
{
    if(!p) {
        GM_ASSERT_MESSAGE(false, "�|�C���^�G���[");
        return false;
    }

    if(!p->initialize()) {
        GM_ASSERT_MESSAGE(false, "���������s");
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
template<typename T>
void cleanup(T* p)
{
    if(!p) {
        GM_ASSERT_MESSAGE(false, "�|�C���^�G���[");
		return;
	}

    p->cleanup();
}
}   // namespace SAFE_TASK
