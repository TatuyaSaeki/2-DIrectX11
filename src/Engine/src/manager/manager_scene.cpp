//===========================================================================
//!	@file	scene_manager.cpp
//!	@brief	シーン管理システム
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SceneManager::update(f32 deleteTime)
{
    // 次のシーンがあったら
    if(nextScene_) {
        // 次のシーンの初期化
        if(nextScene_->initialize()) {
            // 現在のシーンの開放
            if(activeScene_) {
                activeScene_->cleanup();
                activeScene_.reset();
                //appFilterCleanup();
            }
            // 現在のシーンを次のシーンに
            activeScene_.reset(std::move(nextScene_.release()));
        }
        else {
            // 次のシーンの初期化に失敗したら
            nextScene_->cleanup();
        }
    }

    // 現在のシーンがあったら
    if(activeScene_) {
        activeScene_->update(deleteTime);
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void SceneManager::render()
{
    if(activeScene_) {
        activeScene_->render();
    }
}

//---------------------------------------------------------------------------
//! 描画(モード指定)
//---------------------------------------------------------------------------
void SceneManager::render(RenderMode renderMode)
{
    if(activeScene_) {
        activeScene_->render(renderMode);
    }
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SceneManager::cleanup()
{
    if(activeScene_) {
        activeScene_->cleanup();
    }
}

//---------------------------------------------------------------------------
//! アクティブシーンのImGui描画
//---------------------------------------------------------------------------
void SceneManager::showImGuiWindow()
{
    if(activeScene_) {
        activeScene_->showImGuiWindow();
    }
}

//---------------------------------------------------------------------------
//! 次のシーンを設定
//---------------------------------------------------------------------------
void SceneManager::setNextScene(SceneBase* nextScene)
{
    nextScene_.reset(nextScene);
}

//---------------------------------------------------------------------------
//! 現在のシーンの終了確認
//---------------------------------------------------------------------------
bool SceneManager::activeSceneIsEnd() const
{
    return activeScene_->isSceneEnd();
}