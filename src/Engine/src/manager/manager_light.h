//===========================================================================
//! @file	light_manager.h
//! @brife  ���C�g�̊Ǘ�
//===========================================================================
#pragma once

//===========================================================================
//! @class LightManager
//===========================================================================
class LightManager : ManagerBase
{
public:
    //-----------------------------------------------------------------------
    //!	@name ������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    LightManager() = default;

    //! @brief �f�X�g���N�^
    ~LightManager() override = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name �^�X�N
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ������
    //! @return true  ����I��
    //! @return false �G���[�I��
    //-----------------------------------------------------------------------
    bool initialize() override;

    //! @brief �X�V
    void update() override;

	//-----------------------------------------------------------------------
    //! @brief �`��(�`�惂�[�h�w��)
	//! @param [in] renderMode �`�悵�������[�h
	//-----------------------------------------------------------------------
    void render(RenderMode renderMode) override;

    //! @brief ���
    void cleanup() override;

    //! @brief �ǉ�����Ă�������̒萔�o�b�t�@�X�V
    void cbUpdateLights();

    //! @brief ImGui�̃E�B���h�E��\��
    void showImGuiWindow();

private:
    //-----------------------------------------------------------------------v
    //! @brief ���C�g�폜
    //!	@param	[in]	lightType	�폜���������C�g�^�C�v
    //!	@param	[in]	index		�폜���������C�g�̔z��ԍ�
    //-----------------------------------------------------------------------
    void removeLight(LightType lightType, s32 index);

    //@}

private:
    //-----------------------------------------------------------------------
    //!	@name ���C�g�̒萔�o�b�t�@�X�V
    //-----------------------------------------------------------------------
    //@{

    //! @brief ���s�����萔�o�b�t�@�X�V
    void directionalLightTransferConstantBuffer();

    //! @brief �_�����萔�o�b�t�@�X�V
    void pointLightTransferConstantBuffer();

    //! @brief �X�|�b�g���C�g�萔�o�b�t�@�X�V
    void spotLightTransferConstantBuffer();

    //@}

public:
    //-----------------------------------------------------------------------
    //! �ǉ�
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ���C�g�ǉ�(���s����)
    //!	@param	[in]	position	�ʒu
    //!	@param	[in]	lookAt		�����_
    //!	@param	[in]	color		�F
    //!	@param	[in]	intensity	���x
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
    //-----------------------------------------------------------------------
    bool addLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief ���C�g�ǉ�(�_����)
    //!	@param	[in]	position	�ʒu
    //!	@param	[in]	color		�F
    //!	@param	[in]	intensity	���x
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
    //-----------------------------------------------------------------------
    bool addLight(const Vector3& position, const Vector4& color, f32 intensity = 1.0f);

    //-----------------------------------------------------------------------
    //! @brief ���C�g�ǉ�(�X�|�b�g���C�g)
    //!	@param	[in]	position	�ʒu
    //!	@param	[in]	lookAt		�����_
    //!	@param	[in]	color		�F
    //!	@param	[in]	intensity	���x
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
    //-----------------------------------------------------------------------
    bool addSLight(const Vector3& position, const Vector3& lookAt, const Vector4& color, f32 intensity = 1.0f);

    //@}
    //-----------------------------------------------------------------------
    //! �擾
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief  ���s�����擾
	//! @return ���s�����̃|�C���^
	//-----------------------------------------------------------------------
    DirectionalLight* getDirectionalLight() const;

    //template<typename T>
    Light* getLight(LightType lightType, u32 index) const;

    //@}

private:
    std::unique_ptr<KeyInput> inputKey_;   //!< �L�[���͗p

    // ���C�g�{��
    std::unique_ptr<DirectionalLight>        directionalLight_;   //!< ���s����
    std::vector<std::unique_ptr<PointLight>> pointLights_;        //!< �_����
    std::vector<std::unique_ptr<SpotLight>>  spotLights_;         //!< �X�|�b�g���C�g

    // �萔�o�b�t�@
    gpu::ConstantBuffer<cbDirectionalLight> cbDirectionalLight_;   //!< ���s����
    gpu::ConstantBuffer<cbPointLight>       cbPointLight_;         //!< �_����
    gpu::ConstantBuffer<cbSpotLight>        cbSpotLight_;          //!< �X�|�b�g���C�g

    // GPU�X���b�g�ԍ�
    s32 slotDirectional_ = 4;
    s32 slotPoint_       = 5;
    s32 slotSpot_        = 6;

    // �X�V�t���O
    bool isDirectionalLight_ = true;
    bool isPointLight_       = true;
    bool isSpotLight_        = true;

    // �ǉ��t���O
    //bool addDirectionalLight_ = false;
    bool addPointLight_ = false;
    bool addSpotLight_  = false;
};