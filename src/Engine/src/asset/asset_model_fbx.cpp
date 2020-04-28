//===========================================================================
//! @file	asset_model_fbx.cpp
//!	@brief	FBXアセットモデルクラス
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool AssetModelFBX::load(const std::string& fileName)
{
    const std::string& addFilePath = "../../Application/asset/model/fbx/";
    filePath_                      = addFilePath + fileName;

    //================================================================
    // ファイル読み込み
    //================================================================
    FILE* fp;
    if(fopen_s(&fp, filePath_.c_str(), "rb")) {
        MessageBox(NULL, (filePath_ + "\nこのファイル名で実行が出来ない。").c_str(), "エラー", MB_OK);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    auto fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::vector<u8> fileImage;
    fileImage.resize(fileSize);
    fread(fileImage.data(), fileSize, 1, fp);
    fclose(fp);

    //================================================================
    // FBX読み込み
    //================================================================
    const ofbx::IScene* scene = nullptr;   // FBXシーンデータ

    u64 loadFlags = (u64)ofbx::LoadFlags::TRIANGULATE;
    scene         = ofbx::load(fileImage.data(), fileSize, loadFlags);
    if(!scene) {
        OutputDebugString(ofbx::getError());
        return false;
    }

    //================================================================
    // メッシュの読み込み
    //================================================================
    auto meshCount = scene->getMeshCount();   // メッシュ数
    for(int i = 0; i < meshCount; ++i) {
        const ofbx::Mesh& mesh = *scene->getMesh(i);

        MeshFBX meshFbx;
        if(!meshFbx.load(mesh, filePath_)) {
            continue;
        }
        meshes_.emplace_back(std::move(meshFbx));
    }

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void AssetModelFBX::cleanup()
{
}

//===========================================================================
//! MeshFBX
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool MeshFBX::load(const ofbx::Mesh& mesh, const std::string& filePath)
{
    // ジオメトリ取得
    const ofbx::Geometry& geometry = *mesh.getGeometry();

    //! 頂点読み込み
    loadVertices(geometry);

    //! 頂点インデックス番号読み込み
    loadIndices(geometry);

    //! 法線読み込み
    //loadNormals(geometry);

    //! テクスチャUV読み込み
    //loadUVs(geometry);

    //! マテリアル読み込み
    loadMaterials(mesh, filePath);

    Vertex v{};
    for(size_t i = 0; i < indices_.size(); ++i) {
        v.position_ = vertexPoints_[i];
        v.normal_   = normals_[i];
        v.tangent_  = tangents_[i];
        v.binormal_ = binormals_[i];
        v.color_    = Color(255, 255, 255, 255);
        v.uv_       = uvs_[i];
        vertices_.emplace_back(std::move(v));
    }

    if(!this->Mesh::initializeBuffer())
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! 頂点読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadVertices(const ofbx::Geometry& geometry)
{
    auto              vertexCount    = geometry.getVertexCount();   // 頂点数
    const ofbx::Vec3* verticesPoints = geometry.getVertices();      // 頂点配列の先頭

    ofbx::Vec3 vertex{};
    Vector3    v{};
    for(size_t i = 0; i < vertexCount; ++i) {
        vertex = verticesPoints[i];
        v.x_   = f32(vertex.x);
        v.y_   = f32(vertex.y);
        v.z_   = f32(vertex.z);
        vertexPoints_.emplace_back(std::move(v));
    }
}

//---------------------------------------------------------------------------
//! 頂点インデックス番号読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadIndices(const ofbx::Geometry& geometry)
{
    //================================================================
    // 頂点インデクッスの読み込み
    //================================================================
    for(u32 i = 0; i < (u32)geometry.getIndexCount(); ++i) {
        u32  index  = (geometry.getFaceIndices()[i] < 0) ? -geometry.getFaceIndices()[i] - 1 : geometry.getFaceIndices()[i];
        auto uv     = geometry.getUVs()[i];
        auto normal = geometry.getNormals()[i];

        // 頂点インデックス
        indices_.emplace_back(index);
        // 法線
        normals_.emplace_back(Vector3((f32)normal.x, (f32)normal.y, (f32)normal.z));

        // UV座標
        uvs_.emplace_back(Vector2{ f32(uv.x), f32(1.0f - uv.y) });

        Vector3 tangent{};
        Vector3 binormal{};

        // 接線、従法線
        if(i % 3 == 2) {
            auto i1 = indices_[i - 2];
            auto i2 = indices_[i - 1];
            auto i3 = indices_[i - 0];

            i3 = i3;

            auto p1 = vertexPoints_[i1];
            auto p2 = vertexPoints_[i2];
            auto p3 = vertexPoints_[i3];

            auto uv1 = uvs_[i - 2];
            auto uv2 = uvs_[i - 1];
            auto uv3 = uvs_[i - 0];

            auto value = calcTangent(p1, p2, p3, uv1, uv2, uv3);
            tangent    = std::get<0>(value);
            binormal   = std::get<1>(value);

            //// 接ベクトル(tangent)
            tangents_.emplace_back(tangent);
            tangents_.emplace_back(tangent);
            tangents_.emplace_back(tangent);

            // 従法線ベクトル(binormal)
            binormals_.emplace_back(binormal);
            binormals_.emplace_back(binormal);
            binormals_.emplace_back(binormal);
        }

        Vertex vertex;

        vertex.position_ = vertexPoints_[index];
        vertex.normal_   = { (f32)normal.x, (f32)normal.y, (f32)normal.z };
        vertex.tangent_  = tangent;
        vertex.binormal_ = binormal;
        vertex.color_    = { 255, 255, 255, 255 };
        vertex.uv_       = { (f32)uv.x, 1.0f - (f32)uv.y };
        vertices_.emplace_back(vertex);
    }
}

//---------------------------------------------------------------------------
//! 法線読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadNormals(const ofbx::Geometry& geometry)
{
    const auto normals    = geometry.getNormals();
    const auto indexCount = geometry.getIndexCount();

    for(size_t i = 0; i < indexCount; ++i) {
        const auto normal = normals[i];
        normals_.emplace_back(Vector3(f32(normal.x), f32(normal.y), f32(normal.z)));
    }
}

//---------------------------------------------------------------------------
//! テクスチャUV読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadUVs(const ofbx::Geometry& geometry)
{
    const auto UVs        = geometry.getUVs();
    const auto indexCount = geometry.getIndexCount();

    for(size_t i = 0; i < indexCount; ++i) {
        const auto uv = UVs[i];
        uvs_.emplace_back(Vector2(f32(uv.x), f32(1.0f - uv.y)));   // 座標系違うらしい
    }
}

//---------------------------------------------------------------------------
//! 接線,従法線読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadTangentBinormals()
{
}

//---------------------------------------------------------------------------
//! マテリアル読み込み
//---------------------------------------------------------------------------
void MeshFBX::loadMaterials(const ofbx::Mesh& mesh, const std::string& filePath)
{
    const auto& geometry = *mesh.getGeometry();

    //================================================================
    // マテリアル番号読み込み
    //================================================================
    for(s32 i = 0; i < geometry.getIndexCount() / 3; ++i) {
        s32 materialIndex = 0;

        if(geometry.getMaterials()) {
            materialIndex = geometry.getMaterials()[i];
        }
        //! マテリアル番号
        materialIndices_.emplace_back(materialIndex);
    }

    //================================================================
    // マテリアルの読み込み
    //================================================================
    for(s32 i = 0; i < mesh.getMaterialCount(); ++i) {
        auto m = mesh.getMaterial(i);
        auto t = m->getTexture(ofbx::Texture::TextureType::DIFFUSE);

		if (!t) 
			continue;
        
        auto f  = t->getFileName();
        auto f2 = t->getRelativeFileName();

        // テクスチャファイル名の取得
        char fileName[MAX_PATH];
        f.toString(fileName);

        char drive[MAX_PATH];
        char dir[MAX_PATH];
        char name[MAX_PATH];
        char ext[MAX_PATH];

        // テクスチャファイル名の部分だけ抽出
        _splitpath_s(fileName, drive, dir, name, ext);

        // 現在のファイルパスを分解してパス部分を抽出
        _splitpath_s(filePath.c_str(),
                     drive, sizeof(drive),
                     dir, sizeof(dir),
                     nullptr, 0,
                     nullptr, 0);

        // FBXファイルのパスとテクスチャファイル名を結合
        _makepath_s(fileName, drive, dir, name, ext);

#if 1
        Material material{};
        material.texture_[(s32)Material::TextureType::Albedo].reset(gpu::createTexture(fileName));
        //material->texture_[(s32)Material::TextureType::Albedo].reset(gpu::createTexture(fileName));

        //// 末尾に"_A"が付いていた場合外す
        if(strlen(name) > 2) {
            auto tail = name + strlen(name);
            if(strcmp(tail - 2, "_A") == 0) {
                *(tail - 2) = 0;
            }
        }

        // Roughness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   // ベースの名前を追加
            strcat_s(file, "_R");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            auto texture = gpu::createTexture(fileName);
            if(texture) {
                material.texture_[(s32)Material::TextureType::Roughness] = std::shared_ptr<gpu::Texture>(texture);
            }
            else {
                material.texture_[(s32)Material::TextureType::Roughness] = Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE);
            }
        }

        // Metalness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   // ベースの名前を追加
            strcat_s(file, "_M");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            auto texture = gpu::createTexture(fileName);
            if(texture) {
                material.texture_[(s32)Material::TextureType::Metalness] = std::shared_ptr<gpu::Texture>(texture);
            }
            else {
                material.texture_[(s32)Material::TextureType::Metalness] = Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE);
            }
        }

        // Normalmap
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   // ベースの名前を追加
            strcat_s(file, "_N");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            auto texture = gpu::createTexture(fileName);
            if(texture) {
                material.texture_[(s32)Material::TextureType::Normal] = std::shared_ptr<gpu::Texture>(texture);
            }
            else {
                material.texture_[(s32)Material::TextureType::Normal] = Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_NORMAL);
            }
        }

        // Ao
        {
            char file[MAX_PATH];
            strcpy_s(file, name);    // ベースの名前を追加
            strcat_s(file, "_AO");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            auto texture = gpu::createTexture(fileName);

            if(texture) {
                material.texture_[(s32)Material::TextureType::Ao] = std::shared_ptr<gpu::Texture>(texture);
            }
            else {
                material.texture_[(s32)Material::TextureType::Ao] = Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE);
            }
        }

        materials_.emplace_back(std::move(material));
#endif
    }
}