//===========================================================================
//! @file	asset_texture_manager.h
//!	@brief	�e�N�X�`���Ǘ��N���X
//===========================================================================
#pragma once

//! �V�X�e���e�N�X�`�����
enum SYSTEM_TEXTURE : u32
{
    SYSTEM_TEXTURE_NULL_WHITE = 0,
    SYSTEM_TEXTURE_NULL_BLACK,
    SYSTEM_TEXTURE_NULL_NORMAL,
    SYSTEM_TEXTURE_CUBEMAP_NULL_BLACK,
    //----
    // tmp
    SYSTEM_TEXTURE_TOON,

    SYSTEM_TEXTURE_MAX,
};

//===========================================================================
//! @clsss AssetTextureManager
//===========================================================================
class AssetTextureManager
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    AssetTextureManager() = default;

    //! @brief �f�X�g���N�^
    ~AssetTextureManager() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ������
    //! @return true	����I��
    //! @return false	�G���[�I��
    //-----------------------------------------------------------------------
    bool initialize();

    //! @brief ���
    void cleanup();

    //-----------------------------------------------------------------------
    //! @brief �V�X�e���e�N�X�`���擾
    //! @param	[in]	type		�擾�������V�X�e���e�N�X�`���^�C�v
    //! @return	�e�N�X�`���̃|�C���^(�󂯎�鑤�́Aweak_ptr)
    //-----------------------------------------------------------------------
    std::shared_ptr<gpu::Texture> getSystemTexture(SYSTEM_TEXTURE type);

    //-----------------------------------------------------------------------
    //! @brief �e�N�X�`���擾
    //! @param	[in]	fileName	�擾�������t�@�C����
    //! @param	[in]	isCubemap	�L���[�u�}�b�v���ǂ���(�쐬�p�ɕK�v)
    //! @return	�e�N�X�`���̃|�C���^(�󂯎�鑤�́Aweak_ptr)
    //-----------------------------------------------------------------------
    std::shared_ptr<gpu::Texture> getTexture(const std::string& fileName, bool isCubemap = false);

private:
    //-----------------------------------------------------------------------
    //! @brief �e�N�X�`���쐬
    //! @param	[in]	fileName		�擾�������t�@�C����
    //! @param	[in]	isCubemap		�L���[�u�}�b�v���ǂ���(�쐬�p�ɕK�v)
    //! @return	�e�N�X�`���̃|�C���^
    //-----------------------------------------------------------------------
    gpu::Texture* createTexture(const std::string& fileName, bool isCubemap = false);

    //-----------------------------------------------------------------------
    //!	@brief �e�N�X�`���̒ǉ�
    //! @param	[in]	fileName		�擾�������t�@�C����
    //! @param	[in]	isCubemap		�L���[�u�}�b�v���ǂ���(�쐬�p�ɕK�v)
    //! @return �e�N�X�`���̃|�C���^
    //-----------------------------------------------------------------------
    void addTexture(const std::string& fileName, gpu::Texture* assetTexture);

    //@}

private:
    std::shared_ptr<gpu::Texture>                                  systemTextures_[SYSTEM_TEXTURE_MAX];   //!< �V�X�e���e�N�X�`���z��                            //!< �V�X�e���e�N�X�`��
    std::unordered_map<std::string, std::shared_ptr<gpu::Texture>> assetTextures_;                        //!< �e�N�X�`��
};