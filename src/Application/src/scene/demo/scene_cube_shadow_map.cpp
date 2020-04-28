//===========================================================================
//! @file	scene_cube_shadow_map.cpp
//!	@brief	�S���ʃV���h�E�}�b�v
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SceneCubeShadowMap::initialize()
{
	//=============================================================
    // ���C�g������( �J�����Ń��C�g������Ă�̂Ń��C�g����̏������K�{ )
	//=============================================================
    if(!initializeLight())
        return false;

	//=============================================================
    // �J����������
	//=============================================================
    if(!initializeCamera())
        return false;

	//=============================================================
    // ���f��������
	//=============================================================
    if(!initializeModel())
        return false;

	//=============================================================
    // �e�N�X�`��������
	//=============================================================
    if(!initializeTexture())
        return false;

	setFilter(FilterType::ToneMapping);

    return true;
}

//---------------------------------------------------------------------------
//! �J����������
//---------------------------------------------------------------------------
bool SceneCubeShadowMap::initializeCamera()
{
    // ����J����
    {
        auto cameraPosition = Vector3(+0.0f, +5.0f, +10.0f);
        auto cameraLookAt   = Vector3(+0.0f, +2.0f, +0.0f);
        if(!DemoSceneBase::initializeCameraFps(cameraPosition, cameraLookAt))
            return false;
    }

    // �L���[�u�}�b�v���C�g�J����*6
    {
        // ���C�g�̃��[���h�ۂ������
        const auto lightPosition = lightManager_->getLight(LightType::PointLight, 0)->getPosition();

        struct CameraInfo
        {
            Vector3 lookAt_  = Vector3::ZERO;
            Vector3 worldUp_ = Vector3::ZERO;
        };

        std::array<CameraInfo, 6> cameraInfos =
            { {
                { lightPosition + Vector3(+1.0f, +0.0f, +0.0f), Vector3(+0.0f, +1.0f, +0.0f) },   // +X
                { lightPosition + Vector3(-1.0f, +0.0f, +0.0f), Vector3(+0.0f, +1.0f, +0.0f) },   // -X
                { lightPosition + Vector3(+0.0f, +1.0f, +0.0f), Vector3(+0.0f, +0.0f, -1.0f) },   // +Y
                { lightPosition + Vector3(+0.0f, -1.0f, +0.0f), Vector3(+0.0f, +0.0f, +1.0f) },   // -Y
                { lightPosition + Vector3(+0.0f, +0.0f, +1.0f), Vector3(+0.0f, +1.0f, +0.0f) },   // +Z
                { lightPosition + Vector3(+0.0f, +0.0f, -1.0f), Vector3(+0.0f, +1.0f, +0.0f) },   // -Z
            } };

        for(auto& cameraInfo : cameraInfos) {
            cameraManager_->addCamera(lightPosition, cameraInfo.lookAt_, cameraInfo.worldUp_);
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���C�g������
//---------------------------------------------------------------------------
bool SceneCubeShadowMap::initializeLight()
{
	//=============================================================
    // �_����������
	//=============================================================
    {
        auto position = Vector3(0.0f, 3.0f, -4.0f);
        auto color    = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
        if(!lightManager_->addLight(position, color))
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���f��������
//---------------------------------------------------------------------------
bool SceneCubeShadowMap::initializeModel()
{
    // �{�b�N�X
    modelBox_.reset(new Object());
    if(!modelBox_)
        return false;
    if(!modelBox_->initialize("box.fbx"))
        return false;

    modelFloor_.reset(new Object());
    if(!modelFloor_)
        return false;
    if(!modelFloor_->initialize("plane.fbx", 50.0f))
        return false;

    // �ʒu�ݒ�
    modelBox_->setPosition(Vector3(-3.0f, +1.0f, +0.0f));
    modelFloor_->setPosition(Vector3(+0.0f, +0.0f, +0.0f));

    taskManager_->addObject(modelBox_);
    taskManager_->addObject(modelFloor_);

    return true;
}

//---------------------------------------------------------------------------
//! �e�N�X�`��������
//---------------------------------------------------------------------------
bool SceneCubeShadowMap::initializeTexture()
{
    auto    device = device::D3DDevice();
    HRESULT hr     = S_OK;

    // Texture
    {
        D3D11_TEXTURE2D_DESC desc{};

        desc.Width              = WINDOW_H;
        desc.Height             = WINDOW_H;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage              = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags     = 0;
        desc.ArraySize          = 6;
        desc.Format             = DXGI_FORMAT_R32_FLOAT;
        desc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.MiscFlags          = D3D11_RESOURCE_MISC_TEXTURECUBE;
        desc.MipLevels          = 1;

        hr = device->CreateTexture2D(&desc, nullptr, &cubeMapDepthTexture_);
        if(hr != S_OK)
            return false;
    }

    // SRV
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

        desc.Format                      = DXGI_FORMAT_R32_FLOAT;
        desc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURECUBE;
        desc.TextureCube.MipLevels       = 1;
        desc.TextureCube.MostDetailedMip = 0;

        hr = device->CreateShaderResourceView(cubeMapDepthTexture_.Get(), &desc, &cubeMapSrv_);
        if(hr != S_OK)
            return false;
    }

    // RTV
    {
        D3D11_RENDER_TARGET_VIEW_DESC desc{};

        desc.Format                         = DXGI_FORMAT_R32_FLOAT;
        desc.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        desc.Texture2DArray.FirstArraySlice = 0;
        desc.Texture2DArray.MipSlice        = 0;
        desc.Texture2DArray.ArraySize       = 6;

        hr = device->CreateRenderTargetView(cubeMapDepthTexture_.Get(), &desc, &cubeMapRtv_);
        if(hr != S_OK)
            return false;
    }

    //
    {
        D3D11_TEXTURE2D_DESC desc{};

        desc.Width              = WINDOW_H;
        desc.Height             = WINDOW_H;
        desc.MipLevels          = 1;
        desc.ArraySize          = 6;
        desc.Format             = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage              = D3D11_USAGE_DEFAULT;
        desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        desc.CPUAccessFlags     = 0;
        desc.MiscFlags          = D3D11_RESOURCE_MISC_TEXTURECUBE;

        hr = device->CreateTexture2D(&desc, nullptr, &cubeMapDepthTextureD_);
        if(hr != S_OK)
            return false;
    }

    // DSV
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC desc{};

        desc.Format                         = DXGI_FORMAT_D32_FLOAT;
        desc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
        desc.Texture2DArray.FirstArraySlice = 0;
        desc.Texture2DArray.ArraySize       = 6;
        desc.Texture2DArray.MipSlice        = 0;
        desc.Flags                          = D3D11_RESOURCE_MISC_TEXTURECUBE;

        hr = device->CreateDepthStencilView(cubeMapDepthTextureD_.Get(), nullptr, &cubeMapDsv_);
        if(hr != S_OK)
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneCubeShadowMap::update(f32 deleteTime)
{
    // �J�����X�V
    cameraManager_->update();

    // ���C�g�X�V
    lightManager_->update();

    taskManager_->update();
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SceneCubeShadowMap::render()
{
    // ���f���`��
    taskManager_->render();
}

//---------------------------------------------------------------------------
//! �`��(���[�h�w��)
//---------------------------------------------------------------------------
void SceneCubeShadowMap::render(RenderMode renderMode)
{
    DemoSceneBase::render(renderMode);

	std::string gsName = "gsCubeMapDepth";

    switch(renderMode) {
        case RenderMode::Shadow: 
		{
			
		    modelBox_->setShader("vsCubeMapDepth", "psCubeMapDepth", &gsName);
            modelFloor_->setShader("vsCubeMapDepth", "psCubeMapDepth", &gsName);

            auto context = device::D3DContext();
			
			//���̃����_�[�^�[�Q�b�g���N���A
			float clearColor[4] = { 1, 1, 1, 1 };                                            // �N���A�F�쐬�@RGBA�̏�
			context->ClearRenderTargetView(cubeMapRtv_.Get(), clearColor);                   //��ʃN���A
			context->ClearDepthStencilView(cubeMapDsv_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);   //�[�x�o�b�t�@�N���A

            //�����_�[�^�[�Q�b�g��[�x�e�N�X�`���[�ɕύX
            const auto rtv = cubeMapRtv_.Get();
            const auto dsv = cubeMapDsv_.Get();
            context->OMSetRenderTargets(1, &rtv, dsv);
			
            break;
        }
        case RenderMode::Scene:
		{
			modelBox_->setShader("vsCubeMapShadow", "psCubeMapShadow");
			modelFloor_->setShader("vsCubeMapShadow", "psCubeMapShadow");
			// �O���b�h�`��
			gpu::setShader("vsStandard", "psStandard");
			gridRender(128.0f);

			const auto srv = cubeMapSrv_.Get();
			device::D3DContext()->PSSetShaderResources(14, 1, &srv);

		}

            break;
        default:
            break;
    }
	
    render();

	gsName.clear();
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneCubeShadowMap::cleanup()
{
}
