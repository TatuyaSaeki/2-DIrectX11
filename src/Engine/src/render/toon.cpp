//===========================================================================
//!	@file	toon.cpp
//!	@brief	シャドウマップ管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Toon::initialize(s32 resolution)
{
    resolution_ = resolution;

	//=============================================================
    // テクスチャの作成
    //=============================================================
    toonTexture_.reset(gpu::createTexture("texture/toon.png"));
    if(!toonTexture_)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void Toon::cleanup()
{
    toonTexture_.reset();
}

//---------------------------------------------------------------------------
//! エッジ描画開始
//---------------------------------------------------------------------------
void Toon::beginEdge()
{
    auto cbToon   = cbToon_.begin();
    cbToon->edge_ = true;
    cbToon_.end();

    gpu::setConstantBuffer(gpuSlotCBToon, cbToon_);

    // カリング設定
    GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);
}

//--------------------------------------------------------------------------- -
//! トゥーン描画開始
//---------------------------------------------------------------------------
void Toon::begin()
{
    auto cbToon   = cbToon_.begin();
    cbToon->edge_ = false;
    cbToon_.end();

    gpu::setConstantBuffer(gpuSlotCBToon, cbToon_);
    gpu::setTexture(gpuSlotToonTexture_, toonTexture_);

    // カリング設定戻す
    GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
}

//---------------------------------------------------------------------------
//! トゥーン描画終了
//---------------------------------------------------------------------------
void Toon::end()
{
    // tmp
    gpu::setTexture(gpuSlotToonTexture_, nullptr);
}