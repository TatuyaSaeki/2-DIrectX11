//===========================================================================
//!	@file	input_keyboard.h
//!	@brief	�L�[,�}�E�X���͗p
//===========================================================================
#pragma once

//! �X�e�[�g���
enum KEY_STATE : u32
{
	OFF		= 0,
	ON,
	PUSH,
	RELEASE,
};

//! �L�[�ꗗ
enum KEY_KIND : u32
{
	KEY_LEFT	= 0,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,

	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_SPACE,
	KEY_CTRL,
	KEY_ESC,
	KEY_LSHIFT,
	KEY_RSHIFT,

	// �����L�[
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_F1,

	MOUSE_RIGHT,
	MOUSE_LEFT,
	MOUSE_CENTER,

	KEY_TAB,

	KEY_MAX
};

//===========================================================================
//! @class KeyInput
//===========================================================================
class KeyInput
{
public:
	//-----------------------------------------------------------------------
	//! @name ������
	//-----------------------------------------------------------------------
	//@{

	//! @brief �R���X�g���N�^
	KeyInput() = default;

	//! @brief �f�X�g���N�^
	~KeyInput() = default;

	//@}
	//-----------------------------------------------------------------------
	//! @name �擾
	//-----------------------------------------------------------------------
	//@{

	//-----------------------------------------------------------------------
	//!	@brief �L�[�X�e�[�g�擾
	//!	@param	[in]	keyKind		KEY_KIND
	//!	@param	[in]	hexadecimal	16�i
	//!	@return �L�[�X�e�[�g
	//-----------------------------------------------------------------------
	KEY_STATE getKeyState(KEY_KIND keyKind, u32 hexadecimal);

	//@}

private:
	u32	key[KEY_MAX];
	u32 oldKey[KEY_MAX];
};



