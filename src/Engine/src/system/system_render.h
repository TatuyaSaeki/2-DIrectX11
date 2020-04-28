//===========================================================================
//! @file	system_render.h
//!	@brief	DirectX11�`��V�X�e���Ǘ�
//===========================================================================
#pragma once

//===========================================================================
//!	�����_�����O�V�X�e��
//===========================================================================
class SystemRender final
{
public:
    //! @brief �V���O���g�� ���Ԏ擾
    static SystemRender& getInstance()
    {
        static SystemRender instance;
        return instance;
    }

private:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    SystemRender() = default;

public:
    //! @brief �f�X�g���N�^
    ~SystemRender() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief ������
    //!	@param	[in]	width	��
    //!	@param	[in]	height	����
    //!	@return	[out]	true	����I��
    //!	@return	[out]	false	�G���[�I��
	//-----------------------------------------------------------------------
    bool setup(u32 width, u32 height);

    //! @brief ���
    void cleanup();

    //@}
    //-----------------------------------------------------------------------
    //!	@name	����
    //-----------------------------------------------------------------------
    //@{

    //! @brief �`��J�n
    void begin();

    //! @brief �`��I��
    void end();

    //! @brief �V�[���`��J�n
    void beginScene();

    //! @brief �V�[���`��I��
    void endScene();

public:
	//-----------------------------------------------------------------------
    //!	@brief ��ʍX�V
    //! @param	[in]	interval	��ʍX�V���� 1:60fps 2:30fps 3:20fps 4:15fps
	//-----------------------------------------------------------------------
    void swap(s32 interval);

    //! @brief �S��ʕ`��
    void put();

    //! @brief �o�b�t�@�؂�ւ�
    void swapRenderBuffer();

    //@}
    //-----------------------------------------------------------------------
    //! @name �ݒ�
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief �J�����O�ݒ�
    //!	@param	[in]	fileMode		�`�惂�[�h
    //!	@param	[in]	cullingState	�J�����O�ݒ�
	//-----------------------------------------------------------------------
    void settingCullingState(D3D11_FILL_MODE fileMode, D3D11_CULL_MODE cullingState = D3D11_CULL_MODE::D3D11_CULL_NONE);

    //@}
    //-----------------------------------------------------------------------
    //! @name �擾
    //-----------------------------------------------------------------------
    //@{

    //! @brief �o�b�N�o�b�t�@�擾
    gpu::Texture* getBackBuffer() const { return backBuffer_.get(); }

    //! @brief �f�v�X�X�e���V���擾
    gpu::Texture* getDepthStencil() const { return depthStencil_.get(); }

	//-----------------------------------------------------------------------
    //! @brief �`��o�b�t�@���擾
    //! index=0: ���݂̃o�b�t�@
    //! index=1: �����ЂƂ̃��[�N�o�b�t�@
	//! @return �o�b�t�@�̃|�C���^
	//-----------------------------------------------------------------------
    gpu::Texture* getRenderBuffer(u32 index = 0, bool isHDR = true) const
    {
        // HDR�`���Ԃ̈�����HDR�e�N�X�`���ASDR�`���Ԃ�backBuffer;
        u32 x = currentBuffer_ ^ index;
        return (isHDR) ? hdrBuffer_[x].get() : sdrBuffer_[x].get();
    }

	//-----------------------------------------------------------------------
    //! @brief ���[�N�pHDR�o�b�t�@���擾
	//! @param [in] inxex �z��ԍ�
	//! @param [in] inxex �~�b�v���x��(�ő�7)
	//! @return �o�b�t�@�̃|�C���^
	//-----------------------------------------------------------------------
    gpu::Texture* getHDRWorkBuffer(u32 index, u32 mip = 0) const
    {
        return hdrWorkBuffer_[index][mip].get();
    }

    //!
    FilterManager* getFilterManager() const { return filterManager_.get(); }

	//! @brief IBL�e�N�X�`���^�C�v�ݒ�
	void setIBLTextureType(IBLTextureType type) { ibl_.setTextureType(type); }

    //! @brief �u�����h�X�e�[�g���擾
    ID3D11BlendState* getBlendState(BLEND_MODE mode) const { return d3dBlendState_[mode].Get(); }

    s32 getWidth() const { return width_; }
    s32 getHeight() const { return height_; }

    //@}

private:
    s32                           width_;
    s32                           height_;
    std::unique_ptr<gpu::Texture> backBuffer_;                      //!< �o�b�N�o�b�t�@
    std::unique_ptr<gpu::Texture> depthStencil_;                    //!< �f�v�X�o�b�t�@
    std::unique_ptr<gpu::Texture> hdrBuffer_[2];                    //!< �`��pHDR�o�b�t�@
    std::unique_ptr<gpu::Texture> sdrBuffer_[2];                    //!< �`��pSDR�o�b�t�@
    std::unique_ptr<gpu::Texture> hdrWorkBuffer_[2][7];             //!< ���[�N�pHDR�o�b�t�@
    u32                           currentBuffer_ = 0;               //!< ���݂̃o�b�t�@�̃C���f�b�N�X�ԍ�
    com_ptr<ID3D11BlendState>     d3dBlendState_[BLEND_MODE_MAX];   //!< �u�����h�X�e�[�g

    com_ptr<ID3D11RasterizerState> d3dRasterizeState_;             //!< ���X�^���C�Y�ݒ�
    com_ptr<ID3D11SamplerState>    d3dSamplerState_LinearWrap_;    //!< �T���v���[
    com_ptr<ID3D11SamplerState>    d3dSamplerState_LinearClamp_;   //!< �T���v���[

    std::unique_ptr<FilterManager> filterManager_;   //!< �t�B���^�֘A�Ǘ�
    IBL                            ibl_;             //!< IBL
};

//===========================================================================
// �A�N�Z�X�C���^�[�t�F�C�X
//===========================================================================
inline SystemRender* GmRender()
{
    return &SystemRender::getInstance();
}

inline void appFilterCleanup()
{
    GmRender()->getFilterManager()->appFilterCleanup();
}

inline void setFilter(FilterType filterType)
{
    GmRender()->getFilterManager()->setFilter(filterType);
}

inline void removeFilter(FilterType filterType)
{
    GmRender()->getFilterManager()->removeFilter(filterType);
}