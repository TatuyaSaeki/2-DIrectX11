//===========================================================================
//! @file	task_manager.h
//!	@brief	タスク管理(というよりただの短縮クラスみたいになってるから、変えていく)
//===========================================================================
#pragma once

//----------------------------------------
// 1, リスト構造追加
// 2, 毎フレームタスク配列リセットして、ADDしてもらうようにしたい
//----------------------------------------

//===========================================================================
//! @class TaskManager
//===========================================================================
class TaskManager : ManagerBase
{
public:
    //-----------------------------------------------------------------------
    //! 初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    TaskManager() = default;

    //! @brief デストラクタ
    ~TaskManager() override = default;

    //@}
    //-----------------------------------------------------------------------
    //! タスク
    //-----------------------------------------------------------------------
    //@{

    //! @brief 初期化
    bool initialize() override;

    //! @brief 更新
    void update() override;

    //! @brief 描画
    void render(RenderMode renderMode);

    //! @brief 解放
    void cleanup() override;

    //! @brief 解放(ImGuiでリセットボタン押されたとき)
    void resetButtonCleanup();

    //! @brief ImGui
    void showImGuiWindow();

    //@}
    //-----------------------------------------------------------------------
    //! 追加
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief オブジェクト追加
	//! @param [in] object 追加したいオブジェクトのポインタ
	//-----------------------------------------------------------------------
    void addObject(ObjectBaseModel* object);

    //@}
    //-----------------------------------------------------------------------
    //! 追加
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief  オブジェクト配列サイズ取得
	//! @return 追加されているオブジェクトの個数
	//-----------------------------------------------------------------------
    u32 getObjectCount() const;

    //@}

private:
    std::vector<std::unique_ptr<ObjectBaseModel>> objects_;
};
