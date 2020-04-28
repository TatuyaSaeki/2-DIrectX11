//===========================================================================
//! @file	app_main.cpp
//!	@brief	�A�v���P�[�V�������C������
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
//! ������
//---------------------------------------------------------------------------
bool AppMainSystem::initialize(u32 windowH, u32 windowW, HWND hWnd)
{
    inputKey_.reset(new KeyInput());

    //=============================================================
    //	�V�X�e���`�揉����
    //=============================================================
    if(!GmRender()->setup(windowH, windowW)) {
        return false;
    }

    //=============================================================
    // imgui�̏�����
    //=============================================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();   // �J���[�X�^�C����Dark�ɐݒ�

    // Win32/D3D�̏�����
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device::D3DDevice(), device::D3DContext());

    // �t�H���g��ύX
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:/windows/fonts/meiryob.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

    //=============================================================
    //	�V�[���̏�����
    //=============================================================
    sceneManager_.reset(new AppSceneManager());
    if(!SAFE_TASK::initialize(sceneManager_.get())) {
        return false;
    };

    //=============================================================
    // �V�[���W�����v
    //=============================================================
    if(!sceneJump(new SceneDemo())) {
        // MessageBoxB
        return false;
    }

    gpu::setTexture(TEX_TOON, Asset::getSystemTexture(SYSTEM_TEXTURE::SYSTEM_TEXTURE_TOON).get());

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void AppMainSystem::run()
{
    //=============================================================
    // imgui�̃t���[���̏�����
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
    // �V�[���X�V
    //=============================================================
    {
        // �V�[���J�n
		GmRender()->setIBLTextureType(RenderConfig().getIBLTextureType());
        GmRender()->begin();

        // �V�[���X�V
        sceneManager_->update(deleteTime);

        // �`��ݒ�
        RenderConfig().update();
    }

    //=============================================================
    // �V���h�E�`��
    //=============================================================
    {
        if(RenderConfig().isShadowmapping())
            sceneManager_->render(RenderMode::Shadow);
        else
            gpu::setConstantBuffer(8, nullptr);
    }

    //=============================================================
    //	�V�[���`��
    //=============================================================
    {
        // ���C���[�t���[���`��
        bool isWireFrame = RenderConfig().isWireFrame();
        if(isWireFrame) {
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
        }

        // �V�[���`��
        GmRender()->beginScene();
        {
            sceneManager_->render(RenderMode::Scene);
        }
        GmRender()->endScene();

        // ���C���[�t���[��
        if(isWireFrame) {
            GmRender()->settingCullingState(D3D11_FILL_MODE::D3D11_FILL_SOLID);
        }
    }

    //=============================================================
    // �t�B���^�K�p�J�n
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
            // �V�[����ImGui
            sceneManager_->showImGuiWindow();
        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    //=============================================================
    // FrameRate�v�Z
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
//! ���
//---------------------------------------------------------------------------
void AppMainSystem::finalize()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // �V�[�����
    SAFE_TASK::cleanup(sceneManager_.get());

    // �����_�����O�V�X�e�����
    GmRender()->cleanup();
}

//---------------------------------------------------------------------------
//! ���݂̃V�[�����I��������ǂ���
//---------------------------------------------------------------------------
bool AppMainSystem::isSceneEnd()
{
    return sceneManager_->activeSceneIsEnd();
}

//---------------------------------------------------------------------------
//! �w�肳�ꂽ�V�[���ɍX�V
//---------------------------------------------------------------------------
bool AppMainSystem::sceneJump(SceneBase* nextScene)
{
    if(!nextScene)
        return false;

    sceneManager_->setNextScene(nextScene);

    return true;
}