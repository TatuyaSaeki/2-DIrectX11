//===========================================================================
//!	@file	scene_base.h
//!	@brief	シーン基底クラス
//===========================================================================
#pragma once

//! 描画モード
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

//! 自身が持つシーンタイプ
enum class SceneType : u32
{
    None = 0,
    Title,    // 不使用
    Game,     // 不使用
    Demo,     // 使用
    Result,   // 不使用
};

//===========================================================================
//!	@class	SceneBase
//===========================================================================
class SceneBase
{
public:
    //-----------------------------------------------------------------------
    //!	@name	初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    SceneBase() = default;

    //! @brief デストラクタ
    virtual ~SceneBase() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	タスク
    //-----------------------------------------------------------------------
    //@{

    //! @brief 初期化
    virtual bool initialize() = 0;

    //! @brief 更新
    virtual void update(f32 deleteTime) = 0;

    //! @brief 描画
    virtual void render() = 0;

    //! @brief 描画(モード指定)
    virtual void render([[maybe_unused]] RenderMode renderMode){};

    //! @brief 解放
    virtual void cleanup() = 0;

    //! @brief ImGui描画
    virtual void showImGuiWindow(){};

    //-----------------------------------------------------------------------
    //!	@name	シーン管理
    //-----------------------------------------------------------------------
    //@{

    //! @brief シーンが終わったかどうか
    bool isSceneEnd() const { return sceneEnd_; }

    //! @brief シーンタイプ取得
    SceneType getSceneType() { return sceneType_; }

    //@}

protected:
    SceneType sceneType_ = SceneType::None;
    bool      sceneEnd_  = false;   //!< シーン終了フラグ
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