//===========================================================================
//!	@file	scene_base.h
//!	@brief	�V�[�����N���X
//===========================================================================
#pragma once

//! �`�惂�[�h
enum class RenderMode : u32
{
    Shadow = 0,
    Scene,
    Edge,
    Toon,
    Reflection,
    Fog,
	Water1,
	Water2,

	ModeMax
};

//! ���g�����V�[���^�C�v
enum class SceneType : u32
{
    None = 0,
    Title,    // �s�g�p
    Game,     // �s�g�p
    Demo,     // �g�p
    Result,   // �s�g�p
};

//===========================================================================
//!	@class	SceneBase
//===========================================================================
class SceneBase
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    SceneBase() = default;

    //! @brief �f�X�g���N�^
    virtual ~SceneBase() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //! @brief ������
    virtual bool initialize() = 0;

    //! @brief �X�V
    virtual void update(f32 deleteTime) = 0;

    //! @brief �`��
    virtual void render() = 0;

    //! @brief �`��(���[�h�w��)
    virtual void render([[maybe_unused]] RenderMode renderMode){};

    //! @brief ���
    virtual void cleanup() = 0;

    //! @brief ImGui�`��
    virtual void showImGuiWindow(){};

    //-----------------------------------------------------------------------
    //!	@name	�V�[���Ǘ�
    //-----------------------------------------------------------------------
    //@{

    //! @brief �V�[�����I��������ǂ���
    bool isSceneEnd() const { return sceneEnd_; }

    //! @brief �V�[���^�C�v�擾
    SceneType getSceneType() { return sceneType_; }

    //@}

protected:
    SceneType sceneType_ = SceneType::None;
    bool      sceneEnd_  = false;   //!< �V�[���I���t���O
};

//===========================================================================
//! TMP
//===========================================================================
class SceneTitle : public SceneBase
{
public:
    SceneTitle() { sceneType_ = SceneType::Title; }
    ~SceneTitle() = default;

    bool initialize() override { return true; }

    void update([[maybe_unused]] f32 deleteTime) override {}

    void render() override {}

    void cleanup() override {}
};

//===========================================================================
//! TMP
//===========================================================================
class SceneResult : public SceneBase
{
public:
    SceneResult() { sceneType_ = SceneType::Result; }
    ~SceneResult() = default;

    bool initialize() override { return true; }

    void update([[maybe_unused]] f32 deleteTime) override {}

    void render() override {}

    void cleanup() override {}
};