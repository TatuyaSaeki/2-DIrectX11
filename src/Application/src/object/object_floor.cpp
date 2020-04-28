//===========================================================================
//! @file	object_floor.cpp
//! @brife	���I�u�W�F�N�g
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Floor::Floor()
{
    isShowImGui_   = false;
    isButtonReset_ = false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void Floor::update()
{
    ObjectBaseModel::update();
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void Floor::render(RenderMode renderMode)
{
    // �G�b�W, ����, ��, �Ȃ�`�悵�Ȃ�
    if(renderMode == RenderMode::Edge ||
       renderMode == RenderMode::Reflection ||
       renderMode == RenderMode::Fog) {
        return;
    }

    switch(renderMode) {
        case RenderMode::Shadow:   // �e
            useShader_.vsName_ = "vsShadowDepth";
            useShader_.psName_ = "nullptr";
            break;

        case RenderMode::Scene:   // �V�[���`��
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psLight";
            break;

        case RenderMode::Toon:   // �g�D�[���`��
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psLight";
            break;

        case RenderMode::Fog:   // ��
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psFog";
            break;

        case RenderMode::Water1:   // ��
        case RenderMode::Water2:   // ��
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psTexture3D";
            break;

        default:
            break;
    }

    // �V�F�[�_�[�ݒ�
    gpu::setShader(useShader_);

    // ���f���`��
    model_.get()->render(matWorld_);

    //
    auto d3dContext = device::D3DContext();
    d3dContext->VSSetShader(nullptr, nullptr, 0);
    d3dContext->PSSetShader(nullptr, nullptr, 0);
    d3dContext->GSSetShader(nullptr, nullptr, 0);
    d3dContext->HSSetShader(nullptr, nullptr, 0);
    d3dContext->DSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------------------------
//! ImGui�`��
//---------------------------------------------------------------------------
void Floor::showImGuiWindow()
{
}