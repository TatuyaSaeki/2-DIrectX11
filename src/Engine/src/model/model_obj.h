//===========================================================================
//! @file	model_obj.h
//! @brief	Obj���f��
//===========================================================================
#pragma once

//===========================================================================
//! @class ObjModel
//===========================================================================
class ObjModel
{
public:
	void tmpInitialize(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount);

    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
	//@{

    //! @brief �R���X�g���N�^
    ObjModel() = default;

    //! @brief �f�X�g���N�^
    ~ObjModel() = default;

	//@}
	//-----------------------------------------------------------------------
    //!	@nam	�^�X�N
	//-----------------------------------------------------------------------
	//@{

	bool load(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount, const char* texturePath=nullptr);

    //! @brief ������(�ǂݍ���)
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	[out]	true		����I��
	//!	@return	[out]	false		�G���[�I��
    bool load(const std::string& filePath);

    //! @brief �`��
    void render();

    //! @brief ���
    void cleanup();

	//@}

	//! tmp pos
	void setPosition(const Vector3& pos)
	{
		position_ = pos;
	}

	Vector3 getPosition() const
	{
		return position_;
	}

private:
    //-------------------------------------------------------------------------------
    //!	@name	�ǂݍ��݊֐��܂Ƃ�
    //-------------------------------------------------------------------------------

    //! @brief ���_���W�ǂݍ���
    void loadPoint(FILE* fp);

    //! @brief ���_�ԍ��ǂݍ���
    void loadIndex(FILE* fp);

    //! @brief �@���ǂݍ���
    void loadNroaml();

private:
    std::vector<Vertex>  vertices_;
    std::vector<Vector3> points_;
    std::vector<u32>     indices_;

	u32 pointCount_ = 0;
	u32 indexCount_ = 0;

	std::unique_ptr<gpu::Texture> texture0_;

    gpu::ConstantBuffer<cbModel> cbModel_;
    gpu::Buffer                   vertexBuffer_;
    gpu::Buffer                   indexBuffer_;

	Vector3 position_;
};