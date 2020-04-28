//===========================================================================
//! @file	light_manager.h
//! @brife  ライトの管理
//===========================================================================
#pragma once

//===========================================================================
//! @class LightManager
//===========================================================================
class LightManager : ManagerBase
{
public:
    //-----------------------------------------------------------------------
    //!	@name 初期化
    //-----------------------------------------------------------------------
    //@{

    //! @brief コンストラクタ
    LightManager() = default;

    //! @brief デストラクタ
    ~LightManager() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name タスク
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief 初期化
    //! @return true  正常終了
    //! @return false エラー終了
    //-----------------------------------------------------------------------
    bool initialize() override;

    //! @brief 更新
    void update() override;

	//-----------------------------------------------------------------------
    //! @brief 描画(描画モード指定)
	//! @param [in] renderMode 描画したいモード
	//-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief 解放
    void cleanup() override;

    //! @brief 追加されている光源の定数バッファ更新
    void cbUpdateLights();

    //! @brief ImGuiのウィンドウを表示
    void showImGuiWindow();

private:
    //-----------------------------------------------------------------------v
    //! @brief ライト削除
    //!	@param	[in]	lightType	削除したいライトタイプ
    //!	@param	[in]	index		削除したいライトの配列番号
    //-----------------------------------------------------------------------
    void removeLight(LightType lightType, s32 index);

    //@}

private:
    //-----------------------------------------------------------------------
    //!	@name ライトの定数バッファ更新
    //-----------------------------------------------------------------------
    //@{

    //! @brief 平行光源定数バッファ更新
    void directionalLightTransferConstantBuffer();

    //! @brief 点光源定数バッファ更新
    void pointLightTransferConstantBuffer();

    //! @brief スポットライト定数バッファ更新
    void spotLightTransferConstantBuffer();

    //@}

public:
    //-----------------------------------------------------------------------
    //! 追加
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ライト追加(平行光源)
    //!	@param	[in]	position	位置
    //!	@param	[in]	lookAt		注視点
    //!	@param	[in]	color		色
    //!	@param	[in]	intensity	強度
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
    //-----------------------------------------------------------------------
    bool addLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief ライト追加(点光源)
    //!	@param	[in]	position	位置
    //!	@param	[in]	color		色
    //!	@param	[in]	intensity	強度
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
    //-----------------------------------------------------------------------
    bool addLight(const Vector3& position, const Vector4& color, f32 intensity = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief ライト追加(スポットライト)
    //!	@param	[in]	position	位置
    //!	@param	[in]	lookAt		注視点
    //!	@param	[in]	color		色
    //!	@param	[in]	intensity	強度
    //!	@return	true	正常終了
    //!	@return	false	エラー終了
    //-----------------------------------------------------------------------
    bool addSLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity = 1.0f);

    //@}
    //-----------------------------------------------------------------------
    //! 取得
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief  平行光源取得
	//! @return 平行光源のポインタ
	//-----------------------------------------------------------------------
    DirectionalLight* getDirectionalLight() const;

    //template<typename T>
    Light* getLight(LightType lightType, u32 index) const;

    //@}

private:
    std::unique_ptr<KeyInput> inputKey_;   //!< キー入力用

    // ライト本体
    std::unique_ptr<DirectionalLight>        directionalLight_;   //!< 平行光源
    std::vector<std::unique_ptr<PointLight>> pointLights_;        //!< 点光源
    std::vector<std::unique_ptr<SpotLight>>  spotLights_;         //!< スポットライト

    // 定数バッファ
    gpu::ConstantBuffer<cbDirectionalLight> cbDirectionalLight_;   //!< 平行光源
    gpu::ConstantBuffer<cbPointLight>       cbPointLight_;         //!< 点光源
    gpu::ConstantBuffer<cbSpotLight>        cbSpotLight_;          //!< スポットライト

    // GPUスロット番号
    s32 slotDirectional_ = 4;
    s32 slotPoint_       = 5;
    s32 slotSpot_        = 6;

    // 更新フラグ
    bool isDirectionalLight_ = true;
    bool isPointLight_       = true;
    bool isSpotLight_        = true;

    // 追加フラグ
    //bool addDirectionalLight_ = false;
    bool addPointLight_ = false;
    bool addSpotLight_  = false;
};