//===========================================================================
//! @file	asset_model.h
//!	@brief	�A�Z�b�g���f�����L�N���X
//===========================================================================
#pragma once

//--------------------------------------------------------------------------
//! �O�p�`�̍��W��UV���W����ڃx�N�g�������߂�
//! @return [out]	1.�ڃx�N�g��
//! @return [out]	2.�]�@���x�N�g��
//--------------------------------------------------------------------------
std::tuple<Vector3, Vector3> calcTangent(
    const Vector3& p0, const Vector3& p1, const Vector3& p2,
    const Vector2& uv0, const Vector2& uv1, const Vector2& uv2);

//! �O���錾
class Mesh;

//===========================================================================
//! @clsss AssetModel
//===========================================================================
class AssetModel : public AssetBase
{
public:
    //--------------------------------------------------------------------------
    //!	@name	������
    //--------------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    AssetModel() = default;

    //! @brief �f�X�g���N�^
    virtual ~AssetModel() override = default;

    //@}
    //--------------------------------------------------------------------------
    //!	@name	�^�X�N
    //--------------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief ������
    //!	@param	[in]	fileName	�t�@�C����
    //!	@return	true  ����I��
    //!	@return	false �G���[�I��
	//--------------------------------------------------------------------------
    virtual bool load([[maybe_unused]] const std::string& fileName) override { return true; };

	//--------------------------------------------------------------------------
    //! @brief ������(���ڎw��)
    //!	@param	[in]	fileName	�t�@�C����
    //!	@param	[in]	vertices	�o�[�e�b�N�X�z��
    //!	@param	[in]	indices		�C���f�b�N�X�z��
    //!	@param	[in]	iSize		�C���f�b�N�X�z��T�C�Y
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
	//--------------------------------------------------------------------------
    bool load(Vertex* vertices, u32* indices, u32 iSize);

    //! @brief ���
    virtual void cleanup() override{};

    //@}
    //--------------------------------------------------------------------------
    //!	@name	�擾
    //--------------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief ���b�V���擾
    //!	@return ���b�V���z��
	//--------------------------------------------------------------------------
    std::vector<Mesh>& getMeshes() { return meshes_; }

    //@}

protected:
    std::vector<Mesh> meshes_;   //!< ���b�V��
};

//===========================================================================
//! @struct Material
//===========================================================================
struct Material
{
    enum class TextureType
    {
        Albedo,
        Normal,
        Roughness,
        Metalness,
        Ao,
        //----
        Max,
    };

    std::shared_ptr<gpu::Texture> texture_[(s32)TextureType::Max] = {};
};

//===========================================================================
//! @clsss Mesh
//===========================================================================
class Mesh
{
public:
    //-----------------------------------------------------------------------
    //! @name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Mesh() = default;

    //! @brief �f�X�g���N�^
    virtual ~Mesh() = default;

    //@}
    //-----------------------------------------------------------------------
    //! @name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

	//--------------------------------------------------------------------------
    //! @brief ������
    //!	@param	[in]	fileName	�t�@�C����
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
	//--------------------------------------------------------------------------
    virtual bool load(const std::string& fileName) { return true; };

	//--------------------------------------------------------------------------
    //! @brief ������(���ڎw��)
    //!	@param	[in]	fileName	�t�@�C����
    //!	@param	[in]	vertices	�o�[�e�b�N�X�z��
    //!	@param	[in]	indices		�C���f�b�N�X�z��
    //!	@param	[in]	iSize		�C���f�b�N�X�z��T�C�Y
    //!	@return	true	����I��
    //!	@return	false	�G���[�I��
	//--------------------------------------------------------------------------
    bool load(Vertex* vertices, u32* indices, u32 iSize);

    //@}

protected:
    //-----------------------------------------------------------------------
    //! @name	�ǂݍ���
    //-----------------------------------------------------------------------
    //@{

    //! @brief ���_�ǂݍ���
    virtual void loadVertices(){};

    //! @brief ���_�C���f�b�N�X�ԍ��ǂݍ���
    virtual void loadIndices(){};

    //! @brief �@���ǂݍ���
    virtual void loadNormals(){};

    //! @brief �e�N�X�`��UV�ǂݍ���
    virtual void loadUVs(){};

    //! @brief �}�e���A���ǂݍ���
    virtual void loadMaterials(){};

    //! @brief �o�b�t�@������
    virtual bool initializeBuffer();

    //@}

public:
    //-----------------------------------------------------------------------
    //! @name	�擾
    //-----------------------------------------------------------------------
    //@{

    //! @brief ���_�����擾
    const std::vector<Vertex>& getVertices() const { return vertices_; }

    //! @brief ���_�ʒu���擾
    const std::vector<Vector3>& getVertexPoints() const { return vertexPoints_; }

    //! @brief ���_�C���f�b�N�X�ԍ����擾
    const std::vector<u32>& getIndices() const { return indices_; }

    //! @brief �@�����擾
    const std::vector<Vector3>& getNormals() const { return normals_; }

    //! @brief �ڃx�N�g�����擾
    const std::vector<Vector3>& getTangents() const { return tangents_; }

    //! @brief �]�@���x�N�g�����擾
    const std::vector<Vector3>& getBinormals() const { return binormals_; }

    //! @brief UV���W���擾
    const std::vector<Vector2>& getUVs() const { return uvs_; }

    //! @brief �}�e���A���ꗗ���擾
    const std::vector<Material>& getMaterials() const { return materials_; }

    //! @brief �}�e���A���ԍ����擾
    const std::vector<s32>& getMaterialIndices() const { return materialIndices_; }

    //! @brief ���_�o�b�t�@�擾
    const gpu::Buffer& getVertexBuffer() const { return vertexBuffer_; }

    //! @brief �C���f�b�N�X�o�b�t�@�擾
    const gpu::Buffer& getIndexBuffer() const { return indexBuffer_; }

    //@}

protected:
    std::vector<Vertex>   vertices_;          //!< ���_���
    std::vector<Vector3>  vertexPoints_;      //!< ���_�ʒu
    std::vector<u32>      indices_;           //!< ���_�C���f�b�N�X�ԍ�
    std::vector<Vector3>  normals_;           //!< �@��
    std::vector<Vector3>  tangents_;          //!< �ڐ�
    std::vector<Vector3>  binormals_;         //!< �]�@��
    std::vector<Vector2>  uvs_;               //!< �e�N�X�`��UV���W
    std::vector<Material> materials_;         //! �}�e���A��
    std::vector<s32>      materialIndices_;   //! �}�e���A���ԍ�

    gpu::Buffer vertexBuffer_;   //!< ���_�o�b�t�@
    gpu::Buffer indexBuffer_;    //!< �C���f�b�N�X�o�b�t�@
};
