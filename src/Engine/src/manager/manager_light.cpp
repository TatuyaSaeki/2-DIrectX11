//===========================================================================
//! @file	light_manager.cpp
//! @brife  ���C�g�̊Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool LightManager::initialize()
{
    inputKey_.reset(new KeyInput());
    if(!inputKey_)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void LightManager::update()
{
	//=============================================================
    // ���s�����X�V
	//=============================================================
    if(directionalLight_) {
        if(directionalLight_->isActive()) {
            // ���C�g�X�V
            directionalLight_->update();
        }
    }

	//=============================================================
    // �_�����X�V
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
    // �X�|�b�g���C�g�X�V
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
    // �萔�o�b�t�@�]��
    //================================================================

    // �萔�o�b�t�@�X�V
    directionalLightTransferConstantBuffer();

    // �_����
    pointLightTransferConstantBuffer();

    // �X�|�b�g���C�g
    spotLightTransferConstantBuffer();
}

//---------------------------------------------------------------------------
//! �`��()
//---------------------------------------------------------------------------
void LightManager::render(RenderMode renderMode)
{
	if (renderMode != RenderMode::Scene && renderMode != RenderMode::Water2)
		return;

	gpu::setShader("vsStandard", "psStandard");

    //================================================================
    // ���C�g�`��
    //================================================================

    // ���s����
    if(isDirectionalLight_) {
        if(directionalLight_) {
            if(directionalLight_->isActive()) {
                directionalLight_->render();
            }
        }
    }

    // �_����
    if(isPointLight_) {
        for(auto& pointLight : pointLights_) {
            pointLight->render();
        }
    }

    // �X�|�b�g���C�g�`��
    if(isSpotLight_) {
        for(auto& spotLight : spotLights_) {
            spotLight->render();
        }
    }

    //this->showImGuiWindow();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void LightManager::cleanup()
{
    // �X�|�b�g���C�g���
    for(auto& sl : spotLights_) {
        sl->cleanup();
        sl.reset();
    }
    spotLights_.clear();

    // �|�C���g���C�g���
    for(auto& pl : pointLights_) {
        pl->cleanup();
        pl.reset();
    }
    pointLights_.clear();

    // ���s�����J��
    if(directionalLight_) {
        directionalLight_->cleanup();
        directionalLight_.reset();
    }
}

//---------------------------------------------------------------------------
//! �ǉ�����Ă�������̒萔�o�b�t�@�X�V
//---------------------------------------------------------------------------
void LightManager::cbUpdateLights()
{
}

//---------------------------------------------------------------------------
//! ���s�����萔�o�b�t�@�X�V
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

    // �萔�o�b�t�@�X�V
    buffer->position_  = directionalLight_->getPosition();
    buffer->lookAt_    = directionalLight_->getLookAt();
    buffer->color_     = directionalLight_->getColor();
	buffer->intensity_ = intensity;
    buffer->isActive_  = directionalLight_->isActive();

    cbDirectionalLight_.end();
    gpu::setConstantBuffer(slotDirectional_, cbDirectionalLight_);
}

//---------------------------------------------------------------------------
//! �_�����萔�o�b�t�@�X�V
//---------------------------------------------------------------------------
void LightManager::pointLightTransferConstantBuffer()
{
    if(!isPointLight_) {
        gpu::setConstantBuffer(slotPoint_, nullptr);
        return;
    }

    auto buffer = cbPointLight_.begin();

    for(u32 i = 0; i < pointLights_.size(); ++i) {
        // �Z�����Ă邾��
        auto pointLight = pointLights_[i].get();

        // �萔�o�b�t�@�X�V
        buffer->position_[i]    = pointLight->getPosition();
        buffer->color_[i]       = pointLight->getColor();
        buffer->attenuation_[i] = pointLight->getAttenuation();
        buffer->intensity_[i]   = pointLight->getIntensity();
    }

    // ���݂̍ő吔���
    buffer->lightCount_ = u32(pointLights_.size());
    cbPointLight_.end();
    gpu::setConstantBuffer(slotPoint_, cbPointLight_);
}

//---------------------------------------------------------------------------
//! �X�|�b�g���C�g�萔�o�b�t�@�X�V
//---------------------------------------------------------------------------
void LightManager::spotLightTransferConstantBuffer()
{
    if(!isSpotLight_) {
        gpu::setConstantBuffer(slotSpot_, nullptr);
        return;
    }

    auto buffer = cbSpotLight_.begin();

    for(u32 i = 0; i < spotLights_.size(); ++i) {
        // �Z�����Ă邾��
        auto spotLight = spotLights_[i].get();

        // �萔�o�b�t�@�X�V
        buffer->position_[i]    = spotLight->getPosition();
        buffer->color_[i]       = spotLight->getColor();
        buffer->lookAt_[i]      = spotLight->getLookAt();
        buffer->attenuation_[i] = spotLight->getAttenuation();
        buffer->intensity_[i]   = spotLight->getIntensity();
    }
    // ���݂̍ő吔���
    buffer->lightCount_ = u32(spotLights_.size());
    cbSpotLight_.end();
    gpu::setConstantBuffer(slotSpot_, cbSpotLight_);
}

//---------------------------------------------------------------------------
//! ���C�g�ǉ�(���s����)
//---------------------------------------------------------------------------
bool LightManager::addLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity)
{
    //-------------------------------------------------------------
    // ���C�g�쐬 & ������
    //-------------------------------------------------------------
    std::unique_ptr<DirectionalLight> dl;
    dl.reset(new DirectionalLight());

    if(!dl)
        return false;
    if(!dl->initialize())
        return false;

    // �ʒu�ݒ�
    dl->setPosition(position);

    // �����ݒ�
    dl->setLookAt(lookAt);

    // �J���[�ݒ�
    dl->setColor(color);

    // ���x(���邳)�ݒ�
    dl->setIntensity(intensity);

    dl->isActive(true);

    // �ǉ�
    directionalLight_.reset(std::move(dl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g�ǉ�(�_����)
//---------------------------------------------------------------------------
bool LightManager::addLight(const Vector3& position, const Vector4& color, f32 intensity)
{
	if (LIGHT_MAX_COUNT <= pointLights_.size()) {
		return false;
	}

    //------------------------------
    // ���C�g�쐬 & ������
    //------------------------------
    std::unique_ptr<PointLight> pl;
    pl.reset(new PointLight());

    if(!pl)
        return false;
    if(!pl->initialize(s32(pointLights_.size())))
        return false;

    // �ʒu�ݒ�
    pl->setPosition(position);

    // �J���[�ݒ�
    pl->setColor(color);

    // ���x(���邳)�ݒ�
    pl->setIntensity(intensity);

    // �ǉ�
    pointLights_.emplace_back(std::move(pl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g�ǉ�(�X�|�b�g���C�g)
//---------------------------------------------------------------------------
bool LightManager::addSLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity)
{
	if (LIGHT_MAX_COUNT <= spotLights_.size()) {
		return false;
	}

    //------------------------------
    // ���C�g�쐬 & ������
    //------------------------------
    std::unique_ptr<SpotLight> sl;
    sl.reset(new SpotLight());

    if(!sl)
        return false;
    if(!sl->initialize(s32(spotLights_.size())))
        return false;

    // �ʒu�ݒ�
    sl->setPosition(position);

    // �����ݒ�
    sl->setLookAt(lookAt);

    // �J���[�ݒ�
    sl->setColor(color);

    // ���x(���邳)�ݒ�
    sl->setIntensity(intensity);

    // �ǉ�
    spotLights_.emplace_back(std::move(sl.release()));

    return true;
}

//---------------------------------------------------------------------------
//! ���s�����擾 // tmp
//---------------------------------------------------------------------------
DirectionalLight* LightManager::getDirectionalLight() const
{
    return directionalLight_ ? directionalLight_.get() : nullptr;
}

//---------------------------------------------------------------------------
//! ���C�g�擾
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
            break;   // �T�C�Y�I�[�o�[

        case LightType::SpotLight:
            if(index < spotLights_.size())
                return spotLights_[index].get();
            break;   // �T�C�Y�I�[�o�[

        default:
            break;
    }

    return nullptr;
}

//---------------------------------------------------------------------------
//! ���C�g�폜
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
//! ImGui�̃E�B���h�E��\��
//---------------------------------------------------------------------------
void LightManager::showImGuiWindow()
{
	ImGui::Begin("Light");

    if(ImGui::TreeNode("Lights")) {
        // ���s����
        if(ImGui::TreeNode("DirectionalLight")) {
            ImGui::Checkbox("Active", &isDirectionalLight_);

            directionalLight_->showImGuiWindow();

            ImGui::TreePop();
        }

        // �_����
        if(ImGui::TreeNode("PointLight")) {
            ImGui::Checkbox("Active", &isPointLight_);

            if(ImGui::Button("Add"))
                addPointLight_ = true;

            if(ImGui::Button("AllRemove")) {
                // �|�C���g���C�g���
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

        // �X�|�b�g���C�g
        if(ImGui::TreeNode("SpotLight")) {
            ImGui::Checkbox("Active", &isSpotLight_);

            if(ImGui::Button("Add"))
                addSpotLight_ = true;
			if (ImGui::Button("AllRemove")) {
				// �|�C���g���C�g���
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