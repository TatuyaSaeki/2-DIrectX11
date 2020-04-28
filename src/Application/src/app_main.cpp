//===========================================================================
//! @file	app_main.cpp
//!	@brief	アプリケーションメイン処理
//===========================================================================

// tmp
namespace {
f32 oldTime;
u32 count;
f32 deleteTime;
f32 frameRate;
}   // namespace

#include <mmsystem.h>	// timeGetTime()
#pragma comment( lib, "winmm.lib" )

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool AppMainSystem::initialize(u32 windowH, u32 windowW, HWND hWnd)
{
    inputKey_.reset(new KeyInput());

    //=============================================================
    //	システム描画初期化
    //=============================================================
    if(!GmRender()->setup(windowH, windowW)) {
        return false;
    }

    //=============================================================
    // imguiの初期化
    //=============================================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();   // カラースタイルをDarkに設定

    // Win32/D3Dの初期化
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device::D3DDevice(), device::D3DContext());

    // フォントを変更
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:/windows/fonts/meiryob.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

    //=============================================================
    //	シーンの初期化
    //=============================================================
    sceneManager_.reset(new AppSceneManager());
    if(!SAFE_TASK::initialize(sceneManager_.get())) {
        return false;
    };

    //=============================================================
    // シーンジャンプ
    //=============================================================
    if(!sceneJump(new SceneDemo())) {
        // MessageBoxB
        return false;
    }

    gpu::setTexture(TEX_TOON, Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_TOON).get());

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void AppMainSystem::run()
{
    //=============================================================
    // imguiのフレームの初期化
    //=============================================================
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool show_demo_window = false;
        if(show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
    }

    //=============================================================
    // シーン更新
    //=============================================================
    {
        // シーン開始
		GmRender()->setIBLTextureType(RenderConfig().getIBLTextureType());
        GmRender()->begin();

        // シーン更新
        sceneManager_->update(deleteTime);

        // 描画設定
        RenderConfig().update();
    }

    //=============================================================
    // シャドウ描画
    //=============================================================
    {
        if(RenderConfig().isShadowmapping())
            sceneManager_->render(RenderMode::Shadow);
        else
            gpu::setConstantBuffer(8, nullptr);
    }

    //=============================================================
    //	シーン描画
    //=============================================================
    {
        // ワイヤーフレーム描画
        bool isWireFrame = RenderConfig().isWireFrame();
        if(isWireFrame) {
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
        }

        // シーン描画
        GmRender()->beginScene();
        {
            sceneManager_->render(RenderMode::Scene);
        }
        GmRender()->endScene();

        // ワイヤーフレーム
        if(isWireFrame) {
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID);
        }
    }

    //=============================================================
    // フィルタ適用開始
    //=============================================================
    GmRender()->end();

    //=============================================================
    // ImGui
    //=============================================================
    {
        if(inputKey_->getKeyState(KEY_F1, 112) == PUSH)
            isImGui_ = isImGui_ ? false : true;
        if(isImGui_) {
            ImGui::Text("fps: %.1f", frameRate);

            RenderConfig().showImGuiWindow(A_DemoSceneType::getDemoSceneType());
            // シーンのImGui
            sceneManager_->showImGuiWindow();
        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    //=============================================================
    // FrameRate計算
    //=============================================================
    {
        ++count;
        f32 now    = f32(timeGetTime());
        deleteTime = (now - oldTime) * 0.001f;
        if(now - oldTime >= 1000) {
            frameRate = (f32)(count * 1000) / (f32)(now - oldTime);
            oldTime   = now;
            count     = 0;
        }
    }

    GmRender()->swap(1);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void AppMainSystem::finalize()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // シーン解放
    SAFE_TASK::cleanup(sceneManager_.get());

    // レンダリングシステム解放
    GmRender()->cleanup();
}

//---------------------------------------------------------------------------
//! 現在のシーンが終わったかどうか
//---------------------------------------------------------------------------
bool AppMainSystem::isSceneEnd()
{
    return sceneManager_->activeSceneIsEnd();
}

//---------------------------------------------------------------------------
//! 指定されたシーンに更新
//---------------------------------------------------------------------------
bool AppMainSystem::sceneJump(SceneBase* nextScene)
{
    if(!nextScene)
        return false;

    sceneManager_->setNextScene(nextScene);

    return true;
}