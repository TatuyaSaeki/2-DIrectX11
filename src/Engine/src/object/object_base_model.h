//===========================================================================
//! @file	object_base_model.h
//! @brife	モデルを使用するオブジェクトの元
//===========================================================================
#pragma once

// まとめてるだけ
struct UseShader
{
    std::string  vsName_ = "vsStandard";
    std::string  psName_ = "psStandard";
    std::string* gsName_ = nullptr;
    std::string* hsName_ = nullptr;
    std::string* dsName_ = nullptr;
};

//===========================================================================
//! @class ObjectBaseModel
//===========================================================================
class ObjectBaseModel : public ObjectBase
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    ObjectBaseModel() = default;

    //! @brief デストラクタ
    virtual ~ObjectBaseModel() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //! @brief  初期化
    bool initialize() override { return true; }

    //! @brief 初期化
    virtual bool initialize(const std::string& modelPath, f32 scale = 1.0f, PRIMITIVE_TYPE primitiveType = PT_TRIANGLES);

    //! @brief 更新
    void update() override;
    void update(const Matrix& matParent) override;

    //! @brief 描画
    virtual void render() override;

    //! @brief 描画モード指定
    virtual void render(RenderMode renderMode) override;

    //! @brief 解放
    void cleanup() override;

    //! @brief ImGui描画
    void showImGuiWindow() override;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	設定
    //-----------------------------------------------------------------------
    //@{

    //! @brief 使用するテクスチャ設定
    void setTexture(u32 slot, gpu::Texture* texture);
    void setTexture(u32 slot, std::shared_ptr<gpu::Texture> texture);

    //! @brief 使用するシェーダー設定
    void setShader(const std::string& vsName, const std::string& psName, std::string* gsName = nullptr, std::string* hsName = nullptr, std::string* dsName = nullptr);

    //@}
    //-----------------------------------------------------------------------
    //!	@name	取得
    //-----------------------------------------------------------------------
    //@{

    //! @brief ImGuiで描画するか
    bool isImGui() const;

    //! @brief ImGuiのリセットボタンでりせっとされるか
    bool isButtonReset() const;

    //@}

protected:
    //! @brief モデル変更
    void chengeModel();

    //! @brief モデル作成
    ModelFBX* createModel(ModelType modelType);

protected:
    std::unique_ptr<ModelFBX> model_;
    std::unique_ptr<ModelFBX> tmpModel_;

    ModelType currentModelType_ = ModelType::None;
    ModelType nextModelType_    = ModelType::None;
    UseShader useShader_;   //!< 使用するシェーダー

    bool isShowImGui_   = true;   //!< ImGuiでオブジェクト欄に表示するかしないか
    bool isButtonReset_ = true;

    bool isAutoRotation_ = false;
};