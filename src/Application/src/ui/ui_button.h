//===========================================================================
//! @file	UI_Button.h
//===========================================================================
#pragma once
#if 0
//===========================================================================
//! class UI_Button
//===========================================================================
class UI_Button : public UserInterface
{
public:

	//! �R���X�g���N�^
	UI_Button();

	//! �f�X�g���N�^
	~UI_Button();

	//! ������
	bool initialize();

	//! �X�V
	void update();

	//! �`��
	void render();

	//! ���
	void cleanup();

private:
	std::array<AssetTexture*,4> textures_;
	f32 deviationWidth_;
};
#endif
