//===========================================================================
//! @file	task_manager.cpp
//!	@brief	タスク管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool TaskManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void TaskManager::update()
{
    // ob更新
    for(auto& object : objects_) {
        if(object) {
            object->update();
        }
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void TaskManager::render(RenderMode renderMode)
{
    for(auto& object : objects_) {
        if(object) {
            object->render(renderMode);
        }
    }
}

//---------------------------------------------------------------------------
//! 解放 ( !!シーンの開放より先に呼ぶこと！! )
//---------------------------------------------------------------------------
void TaskManager::cleanup()
{
    for(auto& object : objects_) {
        if(object) {
            object->cleanup();
        }
    }
    objects_.clear();
}

//---------------------------------------------------------------------------
//! 解放(ImGuiでリセットボタン押されたとき)
//---------------------------------------------------------------------------
void TaskManager::resetButtonCleanup()
{
    std::vector<ObjectBaseModel*> tmp;

    for(auto& object : objects_) {
        if(object) {
            if(!object->isButtonReset()) {
                tmp.emplace_back(object.release());
                continue;
            }
            object->cleanup();
        }
    }
    objects_.clear();

    for(auto& object : tmp) {
        object->initialize();
        objects_.emplace_back(object);
    }
}

//---------------------------------------------------------------------------
//! ImGui
//---------------------------------------------------------------------------
void TaskManager::showImGuiWindow()
{
    //
    ImGui::Begin("Object");

    if(ImGui::TreeNode("Models")) {
        s32 i = 0;
        for(auto& object : objects_) {
            if(!object->isImGui())
                continue;
            std::string s = "Model [" + std::to_string(i) + "]";
            if(ImGui::TreeNode(s.c_str())) {
                object->showImGuiWindow();
                ImGui::TreePop();
            }
            ++i;
        }

        // 追加
        if(ImGui::Button("Add")) {
            if(objects_.size() < MODEL_MAX_COUNT) {
                std::unique_ptr<ObjectBaseModel> object(new ObjectBaseModel());
                object->initialize("shape/box.fbx", +1.0f);

                object->setPosition({ +0.0f, +3.0f, +0.0f });
                this->addObject(object.release());
            }
        }

        if(ImGui::Button("AllRemove")) {
            this->resetButtonCleanup();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! オブジェクト追加
//---------------------------------------------------------------------------
void TaskManager::addObject(ObjectBaseModel* object)
{
    objects_.emplace_back(object);
}

//---------------------------------------------------------------------------
//! オブジェクト配列サイズ取得
//---------------------------------------------------------------------------
u32 TaskManager::getObjectCount() const
{
    return u32(objects_.size());
}