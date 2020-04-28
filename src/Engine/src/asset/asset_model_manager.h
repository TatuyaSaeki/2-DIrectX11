//===========================================================================
//! @file	asset_model_manager.h
//!	@brief	�A�Z�b�g���f���Ǘ��N���X
//===========================================================================
#pragma once

//! ���f�����
enum class ModelType : u32
{
    None = 0,   //!< �܂悢
    Box,        //!< �{�b�N�X
    Cerberus,   //!< �e
    Knight,     //!< �R�m
};

//===========================================================================
//! @clsss AssetModelManager
//===========================================================================
class AssetModelManager
{
public:
    //---------------------------------------------------------------------------
    //!	@name	������
    //---------------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    AssetModelManager() = default;

    //! @brief �f�X�g���N�^
    ~AssetModelManager();

    //@}
    //---------------------------------------------------------------------------
    //!	@name	�^�X�N
    //---------------------------------------------------------------------------
    //@{

    //! @brief ������
    bool initialize();

    //! @brief ���
    void cleanup();

    //@}
    //---------------------------------------------------------------------------
    //!	@name	�擾
    //---------------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief ���f���擾
    //!	@param	[in]	fileName	�t�@�C����
    //!	@return	���f���̃|�C���^(�󂯎���weak_ptr��), ���s��nullptr
	//--------------------------------------------------------------------------
    std::shared_ptr<AssetModel> getModel(const std::string& fileName);

    //@}

private:
	//--------------------------------------------------------------------------
    //!	@brief ���f���쐬
    //!	@param	[in]	fileName	�t�@�C����
    //!	@return	���f���̃|�C���^, �쐬���s��nullptr
	//--------------------------------------------------------------------------
    AssetModel* createModel(const std::string& fileName);

	//--------------------------------------------------------------------------
    //!	@brief �z��Ƀ��f����ǉ�
    //! @param	[in]	fileName	�t�@�C����
	//--------------------------------------------------------------------------
    void addModel(const std::string& fileName, AssetModel* const assetModel);

private:
    std::unordered_map<std::string, std::shared_ptr<AssetModel>> assetModels_;   //!< ���f���z��
};