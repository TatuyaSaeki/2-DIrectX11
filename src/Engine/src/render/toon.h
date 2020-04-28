//===========================================================================
//! @file	toon.h
//!	@brief	アニメ調描画
//===========================================================================
#pragma once

//===========================================================================
//! @clsss Toon
//===========================================================================
class Toon
{
public:
	//-----------------------------------------------------------------------
	//!	@name	初期化
	//-----------------------------------------------------------------------
	//@{

	//! @brief コンストラクタ
	Toon() = default;

	//! @brief デストラクタ
	~Toon() = default;

	//@}
	//-----------------------------------------------------------------------
	//!	@name	タスク
	//-----------------------------------------------------------------------
	//@{

	//! @brief 初期化
	bool initialize(s32 resolution = 2048);

	//! @brief 解放
	void cleanup();

	//! @brief エッジ描画開始
	void beginEdge();

	//! @brief トゥーン描画開始
	void begin();

	//! @brief トゥーン描画終了
	void end();

	//@}

private:
	s32                           resolution_ = 1280;
	std::unique_ptr<gpu::Texture> toonTexture_;   //!< トゥーン用テクスチャ
	gpu::ConstantBuffer<cbToon> cbToon_;	//!< 定数バッファ
	u32 gpuSlotCBToon= 9;
	u32 gpuSlotToonTexture_ = 11;	//!< gpuテクスチャスロット番号
};
