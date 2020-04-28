//===========================================================================
//!	@file	input_keyboard.h
//!	@brief	キー,マウス入力用
//===========================================================================
#pragma once

//! ステート種類
enum KEY_STATE : u32
{
	OFF		= 0,
	ON,
	PUSH,
	RELEASE,
};

//! キー一覧
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

	// 数字キー
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
	//! @name 初期化
	//-----------------------------------------------------------------------
	//@{

	//! @brief コンストラクタ
	KeyInput() = default;

	//! @brief デストラクタ
	~KeyInput() = default;

	//@}
	//-----------------------------------------------------------------------
	//! @name 取得
	//-----------------------------------------------------------------------
	//@{

	//-----------------------------------------------------------------------
	//!	@brief キーステート取得
	//!	@param	[in]	keyKind		KEY_KIND
	//!	@param	[in]	hexadecimal	16進
	//!	@return キーステート
	//-----------------------------------------------------------------------
	KEY_STATE getKeyState(KEY_KIND keyKind, u32 hexadecimal);

	//@}

private:
	u32	key[KEY_MAX];
	u32 oldKey[KEY_MAX];
};



