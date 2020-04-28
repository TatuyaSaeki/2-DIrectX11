//===========================================================================
//! @file	object_base_model.h
//! @brife	���f�����g�p����I�u�W�F�N�g�̌�
//===========================================================================
#pragma once

// �܂Ƃ߂Ă邾��
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
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    ObjectBaseModel() = default;

    //! @brief �f�X�g���N�^
    virtual ~ObjectBaseModel() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //! @brief  ������
    bool initialize() override { return true; }

    //! @brief ������
    virtual bool initialize(const std::string& modelPath, f32 scale = 1.0f, PRIMITIVE_TYPE primitiveType = PT_TRIANGLES);

    //! @brief �X�V
    void update() override;
    void update(const Matrix& matParent) override;

    //! @brief �`��
    virtual void render() override;

    //! @brief �`�惂�[�h�w��
    virtual void render(RenderMode renderMode) override;

    //! @brief ���
    void cleanup() override;

    //! @brief ImGui�`��
    void showImGuiWindow() override;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�ݒ�
    //-----------------------------------------------------------------------
    //@{

    //! @brief �g�p����e�N�X�`���ݒ�
    void setTexture(u32 slot, gpu::Texture* texture);
    void setTexture(u32 slot, std::shared_ptr<gpu::Texture> texture);

    //! @brief �g�p����V�F�[�_�[�ݒ�
    void setShader(const std::string& vsName, const std::string& psName, std::string* gsName = nullptr, std::string* hsName = nullptr, std::string* dsName = nullptr);

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�擾
    //-----------------------------------------------------------------------
    //@{

    //! @brief ImGui�ŕ`�悷�邩
    bool isImGui() const;

    //! @brief ImGui�̃��Z�b�g�{�^���ł肹���Ƃ���邩
    bool isButtonReset() const;

    //@}

protected:
    //! @brief ���f���ύX
    void chengeModel();

    //! @brief ���f���쐬
    ModelFBX* createModel(ModelType modelType);

protected:
    std::unique_ptr<ModelFBX> model_;
    std::unique_ptr<ModelFBX> tmpModel_;

    ModelType currentModelType_ = ModelType::None;
    ModelType nextModelType_    = ModelType::None;
    UseShader useShader_;   //!< �g�p����V�F�[�_�[

    bool isShowImGui_   = true;   //!< ImGui�ŃI�u�W�F�N�g���ɕ\�����邩���Ȃ���
    bool isButtonReset_ = true;

    bool isAutoRotation_ = false;
};