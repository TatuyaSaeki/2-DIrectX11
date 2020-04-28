//===========================================================================
//!	@file	directX.h
//!	@brief	DirectX11
//===========================================================================
#pragma once

//! �u�����h���[�h���
enum BLEND_MODE
{
    BLEND_MODE_OFF,     //!< ����
    BLEND_MODE_BLEND,   //!< �ʏ�u�����h
    BLEND_MODE_ADD,     //!< ���Z
    //----
    BLEND_MODE_MAX,   //!< �u�����h���[�h��
};

//===========================================================================
//!	@class DirectX11
//===========================================================================
class DirectX11
{
public:
    //! @brief �V���O���g��
    static DirectX11& getInstance()
    {
        static DirectX11 instance;
        return instance;
    }

private:
    //-----------------------------------------------------------------------
    //!	@name	������
	//-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    DirectX11() = default;

public:
    //! @brief �f�X�g���N�^
    virtual ~DirectX11();

    //@}
	//-----------------------------------------------------------------------
    //! @name	�^�X�N
	//-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief ������
    //!	@param	[in]	width	��
    //!	@param	[in]	height	����
    //!	@param	[in]	hWnd	�`��Ώۂ̃E�B���h�E�n���h��
    //!	@return	true	����I��	�i�����j
    //!	@return	false	�G���[�I��	�i���s�j
	//-----------------------------------------------------------------------
    bool setup(u32 width, u32 height, HWND hWnd);

    //! @brief ���
    void cleanup();

    //@}

	//-----------------------------------------------------------------------
    //! @name	�擾
	//-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief D3D�f�o�C�X���擾
	//! @return d3dDevice�̃|�C���^
	//-----------------------------------------------------------------------
    ID3D11Device* getD3DDevice() const { return d3dDevice_.Get(); }

	//-----------------------------------------------------------------------
    //! @brief D3D�R���e�L�X�g���擾
	//! @return d3dImmediateContext�̃|�C���^
	//-----------------------------------------------------------------------
    ID3D11DeviceContext* getD3DContext() const { return d3dImmediateContext_.Get(); }

	//-----------------------------------------------------------------------
	//! @brief �X���b�v�`�F�C���擾
	//! @return d3dSwapChain�̃|�C���^
	//-----------------------------------------------------------------------
    IDXGISwapChain* getD3DSwapChain() const { return d3dSwapChain_.Get(); }

    ////! ���X�^���C�Y�擾
    //ID3D11RasterizerState* getD3DRastarizeState() const { return d3dRasterizeState_.Get(); }
    //
    ////! �T���v���[�擾
    //ID3D11SamplerState* getD3DSamplerLinear() const { return d3dSamplerLinear_.Get(); }
    //
    //// �u�����h�X�e�[�g���擾
    //ID3D11BlendState* getBlendState(BLEND_MODE mode) const { return d3dBlendState_[mode].Get(); }

    //@}

private:
    D3D_DRIVER_TYPE   driverType_   = D3D_DRIVER_TYPE_UNKNOWN;   //!< �쐬���ꂽ�f�o�C�X�̎��		(HW, WARP, REF)
    D3D_FEATURE_LEVEL featureLevel_ = D3D_FEATURE_LEVEL_11_0;    //!< �쐬���ꂽ�f�o�C�X�̋@�\���x��	(DX9,DX10,DX11)

    com_ptr<ID3D11Device>        d3dDevice_;             //!< D3D�f�o�C�X
    com_ptr<ID3D11DeviceContext> d3dImmediateContext_;   //!< �C�~�f�B�G�C�g�R���e�L�X�g
    com_ptr<IDXGISwapChain>      d3dSwapChain_;          //!< �X���b�v�`�F�C��
};

namespace device {

inline ID3D11Device* D3DDevice()
{
    return DirectX11::getInstance().getD3DDevice();
}

inline ID3D11DeviceContext* D3DContext()
{
    return DirectX11::getInstance().getD3DContext();
}
}   // namespace device