//===========================================================================
//! @file	object_base_model.h
//! @brife	モデルを使用するオブジェクトの元
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool ObjectBaseModel::initialize(const std::string& modelPath, f32 scale, PRIMITIVE_TYPE primitiveType)
{
    // モデル取得
    model_.reset(new ModelFBX());
    if(!model_)
        return false;
    if(!model_->load(modelPath, scale, primitiveType))
        return false;

    this->ObjectBase::setScale(scale);

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void ObjectBaseModel::update()
{
    if(currentModelType_ != nextModelType_) {
        this->chengeModel();
    }

    if(isAutoRotation_) {
        auto tmp = this->ObjectBase::getRotation();
        tmp.y_ += 0.5f;
        this->ObjectBase::setRotation(tmp);
    }

    matWorld_ =
        Matrix::scale(scale_) *
        Matrix::rotateX(DEG_TO_RAD(rotation_.x_)) *
        Matrix::rotateY(DEG_TO_RAD(rotation_.y_)) *
        Matrix::rotateZ(DEG_TO_RAD(rotation_.z_)) *
        Matrix::translate(position_);
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void ObjectBaseModel::update(const Matrix& matParent)
{
    if(currentModelType_ != nextModelType_) {
        this->chengeModel();
    }

    if(isAutoRotation_) {
        auto tmp = this->ObjectBase::getRotation();
        tmp.y_ += 0.5f;
        this->ObjectBase::setRotation(tmp);
    }

    matWorld_ =
        Matrix::scale(scale_) *
        Matrix::rotateX(DEG_TO_RAD(rotation_.x_)) *
        Matrix::rotateY(DEG_TO_RAD(rotation_.y_)) *
        Matrix::rotateZ(DEG_TO_RAD(rotation_.z_)) *
        Matrix::translate(position_) *
        matParent;
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void ObjectBaseModel::render()
{
    // シェーダー設定
    gpu::setShader(useShader_);

    // モデル描画
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
//! 描画(モード指定)
//---------------------------------------------------------------------------
void ObjectBaseModel::render(RenderMode renderMode)
{
    switch(renderMode) {
        case RenderMode::Shadow:   // 影
            useShader_.vsName_ = "vsShadowDepth";
            useShader_.psName_ = "nullptr";
            break;

        case RenderMode::Scene:   // シーン描画
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psLightPBR";
            break;

        case RenderMode::Edge:   // エッジ描画
            useShader_.vsName_ = "vsEdge";
            useShader_.psName_ = "psEdge";
            break;

        case RenderMode::Toon:   // トゥーン描画
            useShader_.vsName_ = "vsShadowMap";
            useShader_.psName_ = "psToon";
            break;

        case RenderMode::Reflection:   // 環境反射
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psReflectionMap";
            break;

        case RenderMode::Fog:   // 霧
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psFog";
            break;

        case RenderMode::Water1:   // 水
        case RenderMode::Water2:   // 水
            useShader_.vsName_ = "vsStandard";
            useShader_.psName_ = "psTexture3D";
            break;

        default:
            return;
    }

    this->render();
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void ObjectBaseModel::cleanup()
{
    if(model_) {
        model_->cleanup();
    }
}

//---------------------------------------------------------------------------
//! 使用するテクスチャ設定(外部からの指定用)
//---------------------------------------------------------------------------
void ObjectBaseModel::setTexture(u32 slot, gpu::Texture* texture)
{
    if(texture) {
        model_->setTexture(slot, std::make_shared<gpu::Texture>(*texture));
    }
}

//---------------------------------------------------------------------------
//! 使用するテクスチャ設定(外部からの指定用)
//---------------------------------------------------------------------------
void ObjectBaseModel::setTexture(u32 slot, std::shared_ptr<gpu::Texture> texture)
{
    if(texture) {
        model_->setTexture(slot, texture);
    }
}

//---------------------------------------------------------------------------
//! 使用するシェーダー設定
//---------------------------------------------------------------------------
void ObjectBaseModel::setShader(const std::string& vsName, const std::string& psName, std::string* gsName, std::string* hsName, std::string* dsName)
{
    useShader_.vsName_ = vsName;
    useShader_.psName_ = psName;

    useShader_.gsName_ = gsName;
    useShader_.hsName_ = hsName;
    useShader_.dsName_ = dsName;
}

//---------------------------------------------------------------------------
//! モデル変更
//---------------------------------------------------------------------------
void ObjectBaseModel::chengeModel()
{
    auto m = this->createModel(nextModelType_);
    if(m) {
        model_->cleanup();
        model_.reset(std::move(m));
        currentModelType_ = nextModelType_;
    }
    else {
        m->cleanup();
        delete m;
        nextModelType_ = currentModelType_;
    }
}

//---------------------------------------------------------------------------
//! モデル作成
//---------------------------------------------------------------------------
ModelFBX* ObjectBaseModel::createModel(ModelType modelType)
{
    tmpModel_.reset(new ModelFBX());

    switch(modelType) {
        case ModelType::Box:
            scale_ = 1.0f;
            tmpModel_->load("shape/box.fbx", scale_);
            break;

        case ModelType::Cerberus:
            scale_ = 0.03f;
            tmpModel_->load("Cerberus/Cerberus_LP.fbx", scale_);
            break;

        case ModelType::Knight:
            scale_ = 4.0f;
            tmpModel_->load("Knight/Knight.fbx", scale_);
            break;
    }

    return tmpModel_ ? tmpModel_.release() : nullptr;
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void ObjectBaseModel::showImGuiWindow()
{
    auto p = this->ObjectBase::getPosition();
    auto r = this->ObjectBase::getRotation();
    auto s = this->ObjectBase::getScale();

    f32 fp[3] = { p.x_, p.y_, p.z_ };
    f32 fr[3] = { r.x_, r.y_, r.z_ };
    f32 fs    = s.x_;   // tmp

    ImGui::DragFloat3("Position", fp, 0.05f);
    ImGui::DragFloat3("Rotation", fr, 0.3f);
    ImGui::Checkbox("AutoRoation", &isAutoRotation_);
    ImGui::DragFloat("Scale", &fs, 0.01f, 0.01f, 10.0f);

    this->ObjectBase::setPosition({ fp[0], fp[1], fp[2] });
    this->ObjectBase::setRotation({ fr[0], fr[1], fr[2] });
    this->ObjectBase::setScale({ fs, fs, fs });

    if(ImGui::Button("Box"))
        nextModelType_ = ModelType::Box;
    if(ImGui::Button("Cerberus"))
        nextModelType_ = ModelType::Cerberus;
    if(ImGui::Button("Knight"))
        nextModelType_ = ModelType::Knight;
}

//---------------------------------------------------------------------------
//! ImGuiで描画するか
//---------------------------------------------------------------------------
bool ObjectBaseModel::isImGui() const
{
    return isShowImGui_;
}

//---------------------------------------------------------------------------
//! ImGuiのリセットボタンでりせっとされるか
//---------------------------------------------------------------------------
bool ObjectBaseModel::isButtonReset() const
{
    return isButtonReset_;
}