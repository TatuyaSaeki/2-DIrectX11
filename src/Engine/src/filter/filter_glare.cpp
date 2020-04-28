//===========================================================================
//! @file	filter_glare.cpp
//!	@brief	グレアフィルター
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool FilterGlare::initialize()
{
    filterType_ = FilterType::GaussianBlur;

    return true;
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void FilterGlare::begin()
{
    gpu::Texture* work[2];
    work[0] = GmRender()->getHDRWorkBuffer(0);
    work[1] = GmRender()->getHDRWorkBuffer(1);

    gpu::Texture* backBuffer = GmRender()->getRenderBuffer();

	//=============================================================
    // 高輝度成分を抽出
	//=============================================================
    gpu::setRenderTarget(work[0]);
    gpu::setTexture(0, backBuffer);
    gpu::setShader("vsPrim2D", "psFilterGlare_Luminance");

    GmRender()->put();

	//=============================================================
    // ガウシアンぼかし
	//=============================================================

    for(u32 mip = 1; mip < 7; ++mip) {
        // ミップ一段縮小コピーする
        // 1pass目 (水平)
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, mip - 1));
        gpu::setShader("vsPrim2D", "psTexture2D");

        GmRender()->put();

        // 1pass目 (水平)
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(1, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setShader("vsPrim2D", "psGaussianBlur_H");

        GmRender()->put();

        // 2pass目 (垂直)
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(1, mip));
        gpu::setShader("vsPrim2D", "psGaussianBlur_V");

        GmRender()->put();
    }
#if 1
	//=============================================================
    // 結果を加算合成
	//=============================================================
    {
        f32 blendFactor[4]{};
        device::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);

        gpu::setRenderTarget(backBuffer);

        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 1));
        gpu::setTexture(1, GmRender()->getHDRWorkBuffer(0, 2));
        gpu::setTexture(2, GmRender()->getHDRWorkBuffer(0, 3));
        gpu::setTexture(3, GmRender()->getHDRWorkBuffer(0, 4));
        gpu::setTexture(4, GmRender()->getHDRWorkBuffer(0, 5));
        gpu::setTexture(5, GmRender()->getHDRWorkBuffer(0, 6));

        gpu::setShader("vsPrim2D", "psFilterGlare");

        GmRender()->put();

        // 半透明合成をOFF
        device::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);
    }
	//=============================================================
    // グレア光芒を計算
	//=============================================================

    u32 lineNum = 6;

    for(u32 n = 0; n < lineNum; ++n) {
		//=============================================================
        // 方向
		//=============================================================
        Vector4 dir{};

        f32 angle       = f32(n) / f32(lineNum) * (2.0f * PI);
        f32 angleOffset = 0.25f / f32(lineNum) * (2.0f * PI);
        dir.x_          = sinf(angle + angleOffset);
        dir.y_          = cosf(angle + angleOffset);

        // ミップ一段縮小コピーする
        // 1280x720 → 640x360
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, 1));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 0));
        gpu::setShader("vsPrim2D", "psFilterGlare");
        GmRender()->put();

        // 640x360 → 360x180
        //gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, 2));
        //gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 1));
        //gpu::setShader("vsPrim2D", "psFilterGlare");
        //GmRender()->put();

		//=============================================================
        // ブラー
		//=============================================================
        u32 srcIndex = 0;   // 転送元インデックス番号

        const u32 PASS_COUNT = 3;

        for(s32 i = 0; i < PASS_COUNT; ++i) {
            {
                // 定数バッファを転送
                auto p          = cbFilter_.begin();
                p->stride_      = powf(8.0f, f32(i));
                p->attenuation_ = (i == (PASS_COUNT - 1)) ? 0.3f : 0.9f;
                p->dir_         = dir;

                cbFilter_.end();
            }
            gpu::setConstantBuffer(7, cbFilter_);   // 定数バッファ

            gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(srcIndex ^ 1, 1));
            gpu::setTexture(0, GmRender()->getHDRWorkBuffer(srcIndex, 1));
            gpu::setShader("vsPrim2D", "psFilterGlare_Glare");
            GmRender()->put();

            srcIndex ^= 1;
        }

		//=============================================================
        // 画面に適用(加算合成)
		//=============================================================
        {
            f32 blendFactor[4]{};
            device::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);

            gpu::setRenderTarget(backBuffer);
            gpu::setTexture(0, GmRender()->getHDRWorkBuffer(srcIndex, 1));

            gpu::setShader("vsPrim2D", "psTexture_Clamp");

            GmRender()->put();

            // 半透明合成をOFF
            device::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);
        }
    }
#endif

    // 元に戻す
    gpu::setTexture(0, nullptr);
    gpu::setTexture(1, nullptr);
    gpu::setTexture(2, nullptr);
    gpu::setTexture(3, nullptr);
    gpu::setTexture(4, nullptr);
    gpu::setTexture(5, nullptr);
    gpu::setRenderTarget(backBuffer, GmRender()->getDepthStencil());

    //ImGui::Begin(u8"HDRワークバッファ");
    //{
    //	ImGui::Text("work[0]");
    //	ImGui::Image(GmRender()->getHDRWorkBuffer(0, 1)->getD3DSrv(), ImVec2(1280 / 2, 720 / 2));
    //
    //	ImGui::Text("work[1]");
    //	ImGui::Image(GmRender()->getHDRWorkBuffer(1, 1)->getD3DSrv(), ImVec2(1280 / 2, 720 / 2));
    //}
    //ImGui::End();
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void FilterGlare::finalize()
{
}