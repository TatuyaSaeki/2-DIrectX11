//===========================================================================
//! @file	scene_sea2.h
//!	@brief	�f���V�[�� �C2
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SceneSea2::initialize()
{
    //=============================================================
    // �J����������
	//=============================================================
    auto cameraPosition = Vector3(+0.0f, +10.0f, +50.0f);
    auto cameraLookAt   = Vector3(+0.0f, +1.0f, +0.0f);
    if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
        return false;

	//=============================================================
    // ���C�g������
	//=============================================================
    if(!initializeLight())
        return false;

	//=============================================================
    // ���f��������
	//=============================================================
    if(!initializeModel())
        return false;

    redTexture_.reset(gpu::createTexture("texture/red.png"));
    greenTexture_.reset(gpu::createTexture("texture/green.png"));
    //sea_->setTexture(3, waveTexture_.get());

    auto buffer = gpu::createRenderTarget(128, 128, DXGI_FORMAT_R32G32B32A32_FLOAT);
    textureBuffer_.reset(buffer);

    buffer = gpu::createRenderTarget(128, 128, DXGI_FORMAT_R32G32B32A32_FLOAT);
    normalBuffer_.reset(buffer);

    for(auto& buffer : buffers_) {
        buffer.reset(gpu::createRenderTarget(128, 128, DXGI_FORMAT_R32G32B32A32_FLOAT));
    }

	// �X�J�C�{�b�N�X�L����
	RenderConfig()().isSkyBox(false);

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g������
//---------------------------------------------------------------------------
bool SceneSea2::initializeLight()
{
	//=============================================================
    // ���s����������
	//=============================================================
    {
        auto position = Vector3(0.0f, 30.0f, -100.0f);
        auto lookAt   = Vector3(0.0f, 0.0f, 0.0f);
        auto color    = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        if(!lightManager_->addLight(position, lookAt, color, 1.0f))
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���f��������
//---------------------------------------------------------------------------
bool SceneSea2::initializeModel()
{
	//=============================================================
    // �C�p���f��
	//=============================================================
    sea_.reset(new Object());
    if(!sea_)
        return false;
    if(!sea_->initialize("Subdivision_Plane.fbx", 100))
        return false;

    sea_->setShader("vsSea2", "psSea2");

    // �^�X�N�ɒǉ�
    taskManager_->addObject(sea_);

    return true;
}

//---------------------------------------------------------------------------
//! �g�̐��l������
//---------------------------------------------------------------------------
bool SceneSea2::initializeWave()
{
    wave_ = Wave();

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneSea2::update(f32 deleteTime)
{
    // �J�����X�V
    cameraManager_->update();

    // ���C�g�X�V
    lightManager_->update();

    taskManager_->update();

    {
        wave_.moveDistance_ += wave_.speed_;

        auto cbWave = cbWave_.begin();
        {
            cbWave->moveDistance_ = wave_.moveDistance_;
            cbWave->fineness_     = wave_.fineness_;
            cbWave->sharp_        = wave_.sharp_;
            cbWave->height_       = wave_.height_;
        }
        cbWave_.end();
        gpu::setConstantBuffer(gpuSlot_, &cbWave_);
    }
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SceneSea2::render()
{
    // ���C�g�`��
    gpu::setShader("vsStandard", "psStandard");
    lightManager_->render();

    static bool f = false;
    if(!f) {
        f = true;

        // �ԂƗ΂̃e�N�X�`��������
        {
            // �ԃe�N�X�`�����o�b�t�@�ɃR�s�[
            for(auto& b : buffers_) {
                gpu::setTexture(0, redTexture_.get());
                gpu::setTexture(4, greenTexture_.get());
                gpu::setShader("vsPrim2D", "ps2TextureAdd");
                {
                    gpu::setRenderTarget(b.get());
                    GmRender()->put();
                }
                device::D3DContext()->OMSetRenderTargets(0, nullptr, nullptr);
                gpu::setTexture(0, nullptr);
                gpu::setTexture(4, nullptr);
            }
        }
    }

    s32 prevIndex    = (bufferIndex_ == 0) ? (2) : (bufferIndex_ - 1);
    s32 currentIndex = bufferIndex_;
    s32 nextIndex    = (bufferIndex_ + 1) % 3;

    gpu::setTexture(0, buffers_[prevIndex].get());
    gpu::setTexture(4, buffers_[currentIndex].get());
    gpu::setShader("vsPrim2D", "psSeaWave");

	f32 clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device::D3DContext()->ClearRenderTargetView(buffers_[nextIndex]->getD3DRtv(), clearColor);
    gpu::setRenderTarget(buffers_[nextIndex].get());

    // �`��
    GmRender()->put();

    device::D3DContext()->OMSetRenderTargets(0, nullptr, nullptr);
    gpu::setTexture(0, nullptr);

    bufferIndex_ = nextIndex;

    // �@���}�b�v�쐻
    {
        gpu::setTexture(3, buffers_[nextIndex].get());
        gpu::setShader("vsPrim2D", "psSeaNormal");
        gpu::setRenderTarget(normalBuffer_.get());

        GmRender()->put();

        device::D3DContext()->OMSetRenderTargets(0, nullptr, nullptr);
        gpu::setTexture(3, normalBuffer_.get());
    }

    // �g�`��
    {
        sea_->setShader("vsSea3", "psSea3");
        gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());
        taskManager_->render();
    }
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SceneSea2::render(RenderMode renderMode)
{
    if(renderMode == RenderMode::Shadow)
        return;

    DemoSceneBase::render(renderMode);

    ImGui::Begin(u8"�V���h�E�}�b�v");
    {
        auto a = 128;
        ImGui::InputInt(u8"�𑜓x", &a, 1, 50, ImGuiInputTextFlags_ReadOnly);
        ImGui::Image(normalBuffer_->getD3DSrv(), ImVec2(256, 256));
        ImGui::InputInt(u8"�𑜓x", &a, 1, 50, ImGuiInputTextFlags_ReadOnly);
        ImGui::Image(buffers_[0]->getD3DSrv(), ImVec2(256, 256));

        ImGui::InputInt(u8"�𑜓x", &a, 1, 50, ImGuiInputTextFlags_ReadOnly);
        ImGui::Image(buffers_[1]->getD3DSrv(), ImVec2(256, 256));

        ImGui::InputInt(u8"�𑜓x", &a, 1, 50, ImGuiInputTextFlags_ReadOnly);
        ImGui::Image(buffers_[2]->getD3DSrv(), ImVec2(256, 256));
    }
    ImGui::End();

    this->render();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneSea2::cleanup()
{
}