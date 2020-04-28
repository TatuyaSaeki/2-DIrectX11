//===========================================================================
//! @file	model_obj.cpp
//! @brief	Obj���f��
//===========================================================================

// �����V�_������
void ObjModel::tmpInitialize(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount)
{
    //----------------------------------------------------------------------------------
    // ���_�f�[�^(���̓��C�A�E�g�Ɠ��������o�ϐ����C�A�E�g�̍\���̂̔z��)
    //----------------------------------------------------------------------------------
    vertexBuffer_.initialize(sizeof(Vertex) * verticesCount, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, vertices);
    pointCount_ = verticesCount;

    //---- �C���f�b�N�X�o�b�t�@�̍쐬
    indexBuffer_.initialize(sizeof(u32) * indicesCount, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, indices);
    indexCount_ = indicesCount;
}

//-------------------------------------------------------------------------------
//! ������()
//-------------------------------------------------------------------------------
bool ObjModel::load(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount, const char* texturePath)
{
    tmpInitialize(vertices, indices, verticesCount, indicesCount);

    if(texturePath) {
        texture0_.reset(gpu::createTexture(texturePath));
        if(!texture0_)
            return false;
    }

#if 0
	// �t�@�C�����J��
	FILE * fp;
	if (fopen_s(&fp, filePath.c_str(), "rt") != 0) {
		GM_ASSERT_MESSAGE(false, "model_obj.cpp 83");
		return false;
	}

	// ���_���W�ǂݍ���
	loadPoint(fp);

	// ���_�ԍ��ǂݍ���
	//loadIndex(fp);

	// �@���ǂݍ���

	fclose(fp);
#endif

    return true;
}

//-------------------------------------------------------------------------------
//! ������(�ǂݍ���)
//-------------------------------------------------------------------------------
bool ObjModel::load([[maybe_unused]] const std::string& filePath)
{
    //tmpInitialize();

#if 0
	// �t�@�C�����J��
	FILE* fp;
	if (fopen_s(&fp, filePath.c_str(), "rt") != 0) {
		GM_ASSERT_MESSAGE(false, "model_obj.cpp 83");
		return false;
	}

	// ���_���W�ǂݍ���
	loadPoint(fp);

	// ���_�ԍ��ǂݍ���
	//loadIndex(fp);

	// �@���ǂݍ���

	fclose(fp);
#endif

    return true;
}

//-------------------------------------------------------------------------------
//! �`��
//-------------------------------------------------------------------------------
void ObjModel::render()
{
    // �萔�o�b�t�@�X�V
    auto cbModel       = cbModel_.begin();
    cbModel->matWorld_ = Matrix::translate(position_);
    //cbModel->position_ = position_;
    cbModel_.end();

    gpu::setConstantBuffer(1, cbModel_);
    gpu::setVertexBuffer(&vertexBuffer_, sizeof(Vertex));
    gpu::setIndexBuffer(&indexBuffer_);

    if(texture0_)
        gpu::setTexture(0, texture0_);

    gpu::drawIndexed(PT_TRIANGLES, indexCount_ /*static_cast<u32>(indices_.size())*/);

    gpu::setTexture(0, nullptr);
}

//-------------------------------------------------------------------------------
//! ���
//-------------------------------------------------------------------------------
void ObjModel::cleanup()
{
}

//-------------------------------------------------------------------------------
//! ���_���W�ǂݍ���
//-------------------------------------------------------------------------------
void ObjModel::loadPoint(FILE* fp)
{
    Vector3 point(Vector3::ZERO);
    //u32     loadCount = 0;
    u32 v1 = 0, v2 = 0, v3 = 0;
    u32 vn1 = 0, vn2 = 0, vn3 = 0;

    char str[128];
    while(fgets(str, sizeof(str), fp)) {
        if(strstr(str, "v") != nullptr) {
            sscanf_s(str, "%f %f %f", &point.x_, &point.y_, &point.z_);
            //points_[loadCount++] = point;
            point.x_ = -point.x_;
            points_.emplace_back(point);
        }
        else if(strstr(str, "f") != nullptr) {
            sscanf_s(str, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
            //indices_[loadCount * 3]     = v1 - 1;   //!< OBJ�t�@�C���̓C���f�b�N�X��1����n�܂邩�炸�炷
            //indices_[loadCount * 3 + 1] = v2 - 1;
            //indices_[loadCount * 3 + 2] = v3 - 1;
            indices_.emplace_back(v1 - 1);
            indices_.emplace_back(v2 - 1);
            indices_.emplace_back(v3 - 1);
            //++loadCount;
        }
    }
}

//-------------------------------------------------------------------------------
//! ���_�ԍ��ǂݍ���
//-------------------------------------------------------------------------------
void ObjModel::loadIndex([[maybe_unused]] FILE* fp)
{
    //u32  loadCount = 0;
    //char str[128];
    //
    //while(fgets(str, sizeof(str), fp)) {
    //}
}

//-------------------------------------------------------------------------------
//! �@���ǂݍ���
//-------------------------------------------------------------------------------
void ObjModel::loadNroaml()
{
}