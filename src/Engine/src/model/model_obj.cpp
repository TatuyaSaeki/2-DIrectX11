//===========================================================================
//! @file	model_obj.cpp
//! @brief	Objモデル
//===========================================================================

// 解決シダいけす
void ObjModel::tmpInitialize(Vertex* vertices, u32* indices, u32 verticesCount, u32 indicesCount)
{
    //----------------------------------------------------------------------------------
    // 頂点データ(入力レイアウトと同じメンバ変数レイアウトの構造体の配列)
    //----------------------------------------------------------------------------------
    vertexBuffer_.initialize(sizeof(Vertex) * verticesCount, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, vertices);
    pointCount_ = verticesCount;

    //---- インデックスバッファの作成
    indexBuffer_.initialize(sizeof(u32) * indicesCount, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, indices);
    indexCount_ = indicesCount;
}

//-------------------------------------------------------------------------------
//! 初期化()
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
	// ファイルを開く
	FILE * fp;
	if (fopen_s(&fp, filePath.c_str(), "rt") != 0) {
		GM_ASSERT_MESSAGE(false, "model_obj.cpp 83");
		return false;
	}

	// 頂点座標読み込み
	loadPoint(fp);

	// 頂点番号読み込み
	//loadIndex(fp);

	// 法線読み込み

	fclose(fp);
#endif

    return true;
}

//-------------------------------------------------------------------------------
//! 初期化(読み込み)
//-------------------------------------------------------------------------------
bool ObjModel::load([[maybe_unused]] const std::string& filePath)
{
    //tmpInitialize();

#if 0
	// ファイルを開く
	FILE* fp;
	if (fopen_s(&fp, filePath.c_str(), "rt") != 0) {
		GM_ASSERT_MESSAGE(false, "model_obj.cpp 83");
		return false;
	}

	// 頂点座標読み込み
	loadPoint(fp);

	// 頂点番号読み込み
	//loadIndex(fp);

	// 法線読み込み

	fclose(fp);
#endif

    return true;
}

//-------------------------------------------------------------------------------
//! 描画
//-------------------------------------------------------------------------------
void ObjModel::render()
{
    // 定数バッファ更新
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
//! 解放
//-------------------------------------------------------------------------------
void ObjModel::cleanup()
{
}

//-------------------------------------------------------------------------------
//! 頂点座標読み込み
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
            //indices_[loadCount * 3]     = v1 - 1;   //!< OBJファイルはインデックスが1から始まるからずらす
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
//! 頂点番号読み込み
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
//! 法線読み込み
//-------------------------------------------------------------------------------
void ObjModel::loadNroaml()
{
}