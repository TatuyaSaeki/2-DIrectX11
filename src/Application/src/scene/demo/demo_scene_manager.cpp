#include "demo_scene_manager.h"
////===========================================================================
//! @file	demo_scene_manager.cpp
//!	@brief	デモシーン管理用クラス
//===========================================================================

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
DemoSceneManager::~DemoSceneManager()
{
    //    cleanup();
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool DemoSceneManager::initialize()
{
    // 最初に実行するシーンを代入
    nextSceneType_ = DemoSceneType::Light;
    //nextSceneType_ = DemoSceneType::Water;
    //nextSceneType_ = DemoSceneType::Sea;

    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    if(!shadowMap_.initialize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void DemoSceneManager::update(f32 deleteTime)
{
    if(sceneReset_) {
        activeSceneType_ = DemoSceneType::None;
        RenderConfig().initialize();
        sceneReset_ = false;
    }

    if(activeSceneType_ != nextSceneType_) {
        DemoSceneBase* nextScene = createScene();

        if(nextScene) {
            if(nextScene->initialize()) {
                if(activeScene_) {
                    activeScene_.get()->cleanup();
                    activeScene_.reset();
                }
                activeScene_.reset(nextScene);
                activeSceneType_ = nextSceneType_;
            }
            else {
                GM_SAFE_CLEANUP(nextScene);
                MessageBox(NULL, "次のシーンの初期化に失敗", "エラー", MB_OK);
            }
        }
    }

    if(activeScene_) {
        activeScene_->update(deleteTime);
    }

    // 0が押されたら、次のシーン
    if(inputKey_->getKeyState(KEY_0, 0x30) == KEY_STATE::PUSH) {
        sceneNext_ = true;
    }

    // 次のシーンへの処理
    if(sceneNext_) {
        auto a         = u32(activeSceneType_);
        auto b         = u32(DemoSceneType::SceneTypeMax) - 1;
        nextSceneType_ = DemoSceneType(a % b + 1);
        sceneNext_     = false;
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void DemoSceneManager::render()
{
    if(activeScene_) {
        activeScene_->render();
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void DemoSceneManager::render(RenderMode renderMode)
{
    if(renderMode == RenderMode::Shadow) {
        if(activeScene_) {
            const auto light = activeScene_->getDirectionalLight();
            if(light)
                shadowMap_.begin(*light);
        }
    }

    if(activeScene_) {
        activeScene_->render(renderMode);
    }

    if(renderMode == RenderMode::Shadow) {
        shadowMap_.end();
    }
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void DemoSceneManager::cleanup()
{
    shadowMap_.finalize();

    if(activeScene_) {
        SAFE_TASK::cleanup(activeScene_.get());
    }

    activeSceneType_ = DemoSceneType::None;
    nextSceneType_   = activeSceneType_;
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void DemoSceneManager::showImGuiWindow()
{
    if(!activeScene_) {
        return;
    }

    // シーンのImGui
    activeScene_->showImGuiWindow();

    // リセット
    if(ImGui::Button("SceneReset")) {
        sceneReset_ = true;
    }
    // 次のシーン
    if(ImGui::Button("SceneNext")) {
        sceneNext_ = true;
    }
}

//---------------------------------------------------------------------------
//! 現在のシーンタイプ取得
//---------------------------------------------------------------------------
DemoSceneType DemoSceneManager::getActiveSceneType() const
{
    return activeSceneType_;
}

//---------------------------------------------------------------------------
//! シーン作成
//---------------------------------------------------------------------------
DemoSceneBase* DemoSceneManager::createScene()
{
    switch(nextSceneType_) {
        // ライト
        case DemoSceneType::Light:
            return new SceneLight();
        // 海
        case DemoSceneType::Sea:
            return new SceneSea();
        // 水屈折
        case DemoSceneType::Water:
            return new SceneWater();

        default:
            break;
    }

    return nullptr;
}