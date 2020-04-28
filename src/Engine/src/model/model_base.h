//===========================================================================
//! @file	model_base.h
//===========================================================================
#pragma once

//===========================================================================
//! @class ModelBase
//===========================================================================
class ModelBase
{
public:
	//----------------------------------------------------------
    //!	@name	������
    //----------------------------------------------------------

    //! �R���X�g���N�^
	ModelBase() = default;

    //! �f�X�g���N�^
    ~ModelBase() = default;

    //----------------------------------------------------------
    //!	@nam	�^�X�N
    //----------------------------------------------------------

    //! �`��
    void render();

    //! ���
    void cleanup();

	//! tmp pos
	void setPosition(const Vector3& pos)
	{
		position_ = pos;
	}

private:
    //----------------------------------------------------------
    //!	@name	�ǂݍ��݊֐��܂Ƃ�
    //----------------------------------------------------------

    //! ���_���W�ǂݍ���
    void loadPoint(FILE* fp);

    //! ���_�ԍ��ǂݍ���
    void loadIndex(FILE* fp);

    //! �@���ǂݍ���
    void loadNroaml();

private:
    std::vector<Vertex>  vertices_;
    std::vector<Vector3> points_;
    std::vector<u32>     indices_;

	u32 pointCount_ = 0;
	u32 indexCount_ = 0;

	std::unique_ptr<gpu::Texture> texture0_;

    gpu::ConstantBuffer<CB_World> cbModel_;
    gpu::Buffer                   vertexBuffer_;
    gpu::Buffer                   indexBuffer_;

	Vector3 position_;
};