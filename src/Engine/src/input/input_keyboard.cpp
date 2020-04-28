//===========================================================================
//!	@file	input_keyboard.cpp
//!	@brief	キー,マウス入力用
//===========================================================================

//---------------------------------------------------------------------------
//!	キーステート取得
//---------------------------------------------------------------------------
KEY_STATE KeyInput::getKeyState(KEY_KIND keyKind, u32 hexadecimal)
{
	if (GetKeyState(hexadecimal) & 0x8000) {
		if(oldKey[keyKind]==OFF){
			key[keyKind] = PUSH;
			oldKey[keyKind] = ON;
			return PUSH;
		}
		else {
			key[keyKind] = ON;
			oldKey[keyKind] = ON;
			return ON;
		}
	}
	else {
		if(oldKey[keyKind]==ON){
			key[keyKind] = RELEASE;
			oldKey[keyKind] = OFF;
			return RELEASE;
		}
		else {
			key[keyKind] = OFF;
			oldKey[keyKind] = OFF;
			return OFF;
		}
	}
}