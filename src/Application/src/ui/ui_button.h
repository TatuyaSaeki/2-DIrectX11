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

	//! コンストラクタ
	UI_Button();

	//! デストラクタ
	~UI_Button();

	//! 初期化
	bool initialize();

	//! 更新
	void update();

	//! 描画
	void render();

	//! 解放
	void cleanup();

private:
	std::array<AssetTexture*,4> textures_;
	f32 deviationWidth_;
};
#endif
