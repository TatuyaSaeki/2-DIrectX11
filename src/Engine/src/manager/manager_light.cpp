//===========================================================================
//! @file	light_manager.cpp
//! @brife  ライトの管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool LightManager::initialize()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void LightManager::update()
{
	//=============================================================
    // 平行光源更新
	//=============================================================
    if(directionalLight_) {
        if(directionalLight_->isActive()) {
            // ライト更新
            directionalLight_->update();
        }
    }

	//=============================================================
    // 点光源更新
	//=============================================================
    {
        if(addPointLight_) {
            this->addLight({ 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 8.0f);
            addPointLight_ = false;
        }

        auto itr = pointLights_.begin();
        while(itr != pointLights_.end()) {
            if((*itr)->isRemove()) {
                removeLight((*itr)->getLightType(), (*itr)->getIndex());
                itr = pointLights_.erase(itr);
            }
            else {
                (*itr)->update();
                ++itr;
            }
        }
    }

	//=============================================================
    // スポットライト更新
	//=============================================================
    {
        if(addSpotLight_) {
            this->addSLight({ 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 8.0f);
            addSpotLight_ = false;
        }

        auto itr = spotLights_.begin();
        while(itr != spotLights_.end()) {
            if((*itr)->isRemove()) {
                removeLight((*itr)->getLightType(), (*itr)->getIndex());
                itr = spotLights_.erase(itr);
            }
            else {
                (*itr)->update();
                ++itr;
            }
        }
    }

    //================================================================
    // 定数バッファ転送
    //================================================================

    // 定数バッファ更新
    directionalLightTransferConstantBuffer();

    // 点光源
    pointLightTransferConstantBuffer();

    // スポットライト
    spotLightTransferConstantBuffer();
}

//---------------------------------------------------------------------------
//! 描画()
//---------------------------------------------------------------------------
void LightManager::render(RenderMode renderMode)
{
	if (renderMode != RenderMode::Scene && renderMode != RenderMode::Water2)
		return;

	gpu::setShader("vsStandard", "psStandard");

    //================================================================
    // ライト描画
    //================================================================

    // 平行光源
    if(isDirectionalLight_) {
        if(directionalLight_) {
            if(directionalLight_->isActive()) {
                directionalLight_->render();
            }
        }
    }

    // 点光源
    if(isPointLight_) {
        for(auto& pointLight : pointLights_) {
            pointLight->render();
        }
    }

    // スポットライト描画
    if(isSpotLight_) {
        for(auto& spotLight : spotLights_) {
            spotLight->render();
        }
    }

    //this->showImGuiWindow();
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void LightManager::cleanup()
{
    // スポットライト解放
    for(auto& sl : spotLights_) {
        sl->cleanup();
        sl.reset();
    }
    spotLights_.clear();

    // ポイントライト解放
    for(auto& pl : pointLights_) {
        pl->cleanup();
        pl.reset();
    }
    pointLights_.clear();

    // 平行光源開放
    if(directionalLight_) {
        directionalLight_->cleanup();
        directionalLight_.reset();
    }
}

//---------------------------------------------------------------------------
//! 追加されている光源の定数バッファ更新
//---------------------------------------------------------------------------
void LightManager::cbUpdateLights()
{
}

//---------------------------------------------------------------------------
//! 平行光源定数バッファ更新
//---------------------------------------------------------------------------
void LightManager::directionalLightTransferConstantBuffer()
{
    if(!directionalLight_)
        return;

	auto intensity = directionalLight_->getIntensity();
		
    if(!isDirectionalLight_) {
        //gpu::setConstantBuffer(slotDirectional_, nullptr);
        //return;
		intensity = 0;
    }

    auto buffer = cbDirectionalLight_.begin();

    // 定数バッファ更新
    buffer->position_  = directionalLight_->getPosition();
    buffer->lookAt_    = directionalLight_->getLookAt();
    buffer->color_     = directionalLight_->getColor();
	buffer->intensity_ = intensity;
    buffer->isActive_  = directionalLight_->isActive();

    cbDirectionalLight_.end();
    gpu::setConstantBuffer(slotDirectional_, cbDirectionalLight_);
}

//---------------------------------------------------------------------------
//! 点光源定数バッファ更新
//---------------------------------------------------------------------------
void LightManager::pointLightTransferConstantBuffer()
{
    if(!isPointLight_) {
        gpu::setConstantBuffer(slotPoint_, nullptr);
        return;
    }

    auto buffer = cbPointLight_.begin();

    for(u32 i = 0; i < pointLights_.size(); ++i) {
        // 短くしてるだけ
        auto pointLight = pointLights_[i].get();

        // 定数バッファ更新
        buffer->position_[i]    = pointLight->getPosition();
        buffer->color_[i]       = pointLight->getColor();
        buffer->attenuation_[i] = pointLight->getAttenuation();
        buffer->intensity_[i]   = pointLight->getIntensity();
    }

    // 現在の最大数代入
    buffer->lightCount_ = u32(pointLights_.size());
    cbPointLight_.end();
    gpu::setConstantBuffer(slotPoint_, cbPointLight_);
}

//---------------------------------------------------------------------------
//! スポットライト定数バッファ更新
//---------------------------------------------------------------------------
void LightManager::spotLightTransferConstantBuffer()
{
    if(!isSpotLight_) {
        gpu::setConstantBuffer(slotSpot_, nullptr);
        return;
    }

    auto buffer = cbSpotLight_.begin();

    for(u32 i = 0; i < spotLights_.size(); ++i) {
        // 短くしてるだけ
        auto spotLight = spotLights_[i].get();

        // 定数バッファ更新
        buffer->position_[i]    = spotLight->getPosition();
        buffer->color_[i]       = spotLight->getColor();
        buffer->lookAt_[i]      = spotLight->getLookAt();
        buffer->attenuation_[i] = spotLight->getAttenuation();
        buffer->intensity_[i]   = spotLight->getIntensity();
    }
    // 現在の最大数代入
    buffer->lightCount_ = u32(spotLights_.size());
    cbSpotLight_.end();
    gpu::setConstantBuffer(slotSpot_, cbSpotLight_);
}

//---------------------------------------------------------------------------
//! ライト追加(平行光源)
//---------------------------------------------------------------------------
bool LightManager::addLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity)
{
    //-------------------------------------------------------------
    // ライト作成 & 初期化
    //-------------------------------------------------------------
    std::unique_ptr<DirectionalLight> dl;
    dl.reset(new DirectionalLight());

    if(!dl)
        return false;
    if(!dl->initialize())
        return false;

    // 位置設定
    dl->setPosition(position);

    // 向き設定
    dl->setLookAt(lookAt);

    // カラー設定
    dl->setColor(color);

    // 強度(明るさ)設定
    dl->setIntensity(intensity);

    dl->isActive(true);

    // 追加
    directionalLight_.reset(std::move(dl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! ライト追加(点光源)
//---------------------------------------------------------------------------
bool LightManager::addLight(const Vector3& position, const Vector4& color, f32 intensity)
{
	if (LIGHT_MAX_COUNT <= pointLights_.size()) {
		return false;
	}

    //------------------------------
    // ライト作成 & 初期化
    //------------------------------
    std::unique_ptr<PointLight> pl;
    pl.reset(new PointLight());

    if(!pl)
        return false;
    if(!pl->initialize(s32(pointLights_.size())))
        return false;

    // 位置設定
    pl->setPosition(position);

    // カラー設定
    pl->setColor(color);

    // 強度(明るさ)設定
    pl->setIntensity(intensity);

    // 追加
    pointLights_.emplace_back(std::move(pl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! ライト追加(スポットライト)
//---------------------------------------------------------------------------
bool LightManager::addSLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity)
{
	if (LIGHT_MAX_COUNT <= spotLights_.size()) {
		return false;
	}

    //------------------------------
    // ライト作成 & 初期化
    //------------------------------
    std::unique_ptr<SpotLight> sl;
    sl.reset(new SpotLight());

    if(!sl)
        return false;
    if(!sl->initialize(s32(spotLights_.size())))
        return false;

    // 位置設定
    sl->setPosition(position);

    // 向き設定
    sl->setLookAt(lookAt);

    // カラー設定
    sl->setColor(color);

    // 強度(明るさ)設定
    sl->setIntensity(intensity);

    // 追加
    spotLights_.emplace_back(std::move(sl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! 平行光源取得 // tmp
//---------------------------------------------------------------------------
DirectionalLight* LightManager::getDirectionalLight() const
{
    return directionalLight_ ? directionalLight_.get() : nullptr;
}

//---------------------------------------------------------------------------
//! ライト取得
//---------------------------------------------------------------------------
//template<typename T>
Light* LightManager::getLight(LightType lightType, u32 index) const
{
    switch(lightType) {
        case LightType::DirectionalLight:
            return directionalLight_.get();

        case LightType::PointLight:
            if(index < pointLights_.size())
                return pointLights_[index].get();
            break;   // サイズオーバー

        case LightType::SpotLight:
            if(index < spotLights_.size())
                return spotLights_[index].get();
            break;   // サイズオーバー

        default:
            break;
    }

    return nullptr;
}

//---------------------------------------------------------------------------
//! ライト削除
//---------------------------------------------------------------------------
void LightManager::removeLight(LightType lightType, s32 index)
{
    switch(lightType) {
        case LightType::PointLight: {
            pointLights_[index]->cleanup();
            for(s32 i = index; i < static_cast<s32>(pointLights_.size()) - 1; ++i) {
                pointLights_[i + 1]->setIndex(i);
            }
        } break;

        case LightType::SpotLight: {
            spotLights_[index]->cleanup();
            for(s32 i = index; i < static_cast<s32>(spotLights_.size()) - 1; ++i) {
                spotLights_[i + 1]->setIndex(i);
            }
        }

        default:
            break;
    }
}

//---------------------------------------------------------------------------
//! ImGuiのウィンドウを表示
//---------------------------------------------------------------------------
void LightManager::showImGuiWindow()
{
	ImGui::Begin("Light");

    if(ImGui::TreeNode("Lights")) {
        // 平行光源
        if(ImGui::TreeNode("DirectionalLight")) {
            ImGui::Checkbox("Active", &isDirectionalLight_);

            directionalLight_->showImGuiWindow();

            ImGui::TreePop();
        }

        // 点光源
        if(ImGui::TreeNode("PointLight")) {
            ImGui::Checkbox("Active", &isPointLight_);

            if(ImGui::Button("Add"))
                addPointLight_ = true;

            if(ImGui::Button("AllRemove")) {
                // ポイントライト解放
                for(auto& pl : pointLights_) {
                    pl->cleanup();
                    pl.reset();
                }
                pointLights_.clear();
            }

            for(auto& pointLight : pointLights_)
                pointLight->showImGuiWindow();

            ImGui::TreePop();
        }

        // スポットライト
        if(ImGui::TreeNode("SpotLight")) {
            ImGui::Checkbox("Active", &isSpotLight_);

            if(ImGui::Button("Add"))
                addSpotLight_ = true;
			if (ImGui::Button("AllRemove")) {
				// ポイントライト解放
				for (auto& sl : spotLights_) {
					sl->cleanup();
					sl.reset();
				}
				spotLights_.clear();
			}
            for(auto& spotLight : spotLights_)
                spotLight->showImGuiWindow();

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
	ImGui::End();
}