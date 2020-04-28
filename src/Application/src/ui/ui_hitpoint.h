//===========================================================================
//! @file	HitPoint.h
//===========================================================================
#pragma once

#if 0
//===========================================================================
//! HitPoint
//===========================================================================
class HitPoint : public UserInterface
{
public:
    //---------------------------------------------------------------------------------
    //!	@name 初期化
    //---------------------------------------------------------------------------------

	//! コンストラクタ
    HitPoint();

	//! デストラクタ
    ~HitPoint() = default;

	//---------------------------------------------------------------------------------
    //!	@name タスク
    //---------------------------------------------------------------------------------
	
	//! 初期化
    bool initialize(const std::string& filePath);

	//! 更新
	void update(const u32 hitPoint);

	//! 描画
    void render(const Vector2& position);

	//! 開放
    void cleanup();


private:
	AssetTexture* hitPointTexture_;
	AssetTexture* hitPointFrameTexture_;

	u32 hitPoint_;
};
#endif