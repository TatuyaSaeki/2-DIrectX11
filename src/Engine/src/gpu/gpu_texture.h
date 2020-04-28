//===========================================================================
//!	@file	gpu_texture.h
//!	@brief	�e�N�X�`��
//===========================================================================
#pragma once

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//===========================================================================
//! @class	Texture
//===========================================================================
class Texture
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Texture() = default;

    //! @brief �f�X�g���N�^
    ~Texture() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ������(D3DResource�w��)
    //!	@param	[in]	d3dResource	ID3D11Resource�̃|�C���^
    //!	@param	[in]	cubemap		�L���[�u�}�b�v���ǂ���
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
    //-----------------------------------------------------------------------
    bool initialize(ID3D11Resource* d3dResource, bool cubemap = false);

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�擾
    //-----------------------------------------------------------------------
    //@{

    u32         getWidth() const { return width_; }           // ���擾
    u32         getHeight() const { return height_; }         // �����擾
    u32         getMipLevels() const { return mipLevels_; }   // �~�b�v���x���擾
    DXGI_FORMAT getFormat() const { return format_; }         // �`���擾

    ID3D11Resource*            getD3DResource() const { return d3dResource_.Get(); }   // �e�N�X�`���������[
    ID3D11ShaderResourceView*  getD3DSrv() const { return d3dSrv_.Get(); }             // SRV
    ID3D11RenderTargetView*    getD3DRtv() const { return d3dRtv_.Get(); }             // RTV
    ID3D11DepthStencilView*    getD3DDsv() const { return d3dDsv_.Get(); }             // DSV
    ID3D11UnorderedAccessView* getD3DUav() const { return d3dUav_.Get(); }             // UAV

    //@}

private:
    u32         width_     = 0;                     //!< ��
    u32         height_    = 0;                     //!< ����
    u32         depth_     = 0;                     //!< ���s��
    u32         mipLevels_ = 0;                     //!< �~�b�v���x����
    DXGI_FORMAT format_    = DXGI_FORMAT_UNKNOWN;   //!< �s�N�Z���t�H�[�}�b�g
    u32         bindFlags_ = 0;                     //!< �֘A�t�����(D3D11_BIND_FLAG�̑g�ݍ��킹)

    com_ptr<ID3D11Resource>            d3dResource_;   //!< �e�N�X�`���������[
    com_ptr<ID3D11ShaderResourceView>  d3dSrv_;        //!< SRV
    com_ptr<ID3D11RenderTargetView>    d3dRtv_;        //!< RTV
    com_ptr<ID3D11DepthStencilView>    d3dDsv_;        //!< DSV
    com_ptr<ID3D11UnorderedAccessView> d3dUav_;        //!< UAV
};

//-----------------------------------------------------------------------
//! @brief  �e�N�X�`���쐬�i�t�@�C�����w��j
//!	@param	[in]	filePath	�t�@�C����
//!	@param	[in]	cubemap		�L���[�u�}�b�v���ǂ���
//!	@return	�e�N�X�`���̃|�C���^, ���snullptr
//-----------------------------------------------------------------------
gpu::Texture* createTexture(const std::string& filePath, bool cubemap = false);

//-----------------------------------------------------------------------
//! @brief  �e�N�X�`�����쐬(ID3D11Resource�w��)
//!	@param	[in]	filePath	�t�@�C����
//!	@param	[in]	cubemap		�L���[�u�}�b�v���ǂ���
//!	@return	�e�N�X�`���̃|�C���^, ���snullptr
//-----------------------------------------------------------------------
gpu::Texture* createTexture(ID3D11Resource* d3dResouce, bool cubemap = false);

//-----------------------------------------------------------------------
//! @brief  �`��^�[�Q�b�g�쐬
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	format	�s�N�Z���`��
//!	@param	[in]	cubemap	�L���[�u�}�b�v���ǂ���
//!	@return	�e�N�X�`���̃|�C���^
//-----------------------------------------------------------------------
gpu::Texture* createRenderTarget(u32 width, u32 height, DXGI_FORMAT format, bool cubemap = false);

}   // namespace gpu
