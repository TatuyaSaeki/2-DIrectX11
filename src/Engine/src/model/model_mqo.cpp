//===========================================================================
//!	@file	model_mqo.cpp
//!	@brief	MQOモデル
//===========================================================================

//---------------------------------------------------------------------------
//!	MQOソート用
//---------------------------------------------------------------------------
int sortFunc(const void* a, const void* b)
{
    int m1 = ((ModelMQO::Face*)a)->material_;
    int m2 = ((ModelMQO::Face*)b)->material_;

    if(m1 < m2)
        return -1;
    if(m1 > m2)
        return +1;
    return 0;
}

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
ModelMQO::ModelMQO()
{
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
ModelMQO::~ModelMQO()
{
    for(auto& o : objects_) {
        delete o;
        o = nullptr;
    }

    for(auto& m : materials_) {
        delete m.texture_;
    }
}

//---------------------------------------------------------------------------
//! 読み込み
//---------------------------------------------------------------------------
bool ModelMQO::load(char* fileName, f32 scale)
{
    //=============================================================
    // ファイルを開く
    //=============================================================
    FILE* fp;
    if(fopen_s(&fp, fileName, "rt") != 0) {
        return false;
    }

    //=============================================================
    // 一行ずつファイル終端まで解析していく
    //=============================================================
    enum class State
    {
        Root,
        Object,
        Vertex,
        Face,
        Material,
    };

    State state = State::Root;   // 状態の初期値

    Object* object = nullptr;

    char str[1024];
    while(fgets(str, sizeof(str), fp)) {
        switch(state) {
            case State::Root:   //---- ルート解析中
                if(strncmp(str, "Object ", 7) == 0) {
                    object = new Object();

                    // オブジェクト名を取り出す Object "name"
                    char* p = strchr(str, '\"');
                    p += 1;   // 区切り記号をスキップ

                    char* name = p;   // 名前先頭

                    p  = strchr(p, '\"');
                    *p = 0;   // 文字終端

                    object->name_ = name;

                    state = State::Object;
                    break;
                }

                if(strncmp(str, "Material", 8) == 0) {
                    state = State::Material;
                    break;
                }

                break;
            case State::Object:   //---- オブジェクト解析中
                if(strncmp(str, "}", 1) == 0) {
                    // 最適化
                    object->optimize();

                    // オブジェクトリストにつなぐ
                    objects_.push_back(object);
                    object = nullptr;

                    state = State::Root;
                    break;
                }
                if(strstr(str, "color ") != nullptr) {
                    Vector4 color;
                    sscanf_s(str, " color %f %f %f", &color.r_, &color.g_, &color.b_);
                    color.a_ = 1.0f;

                    object->color_ = color;

                    break;
                }

                if(strstr(str, "vertex ") != nullptr) {
                    state = State::Vertex;
                    break;
                }

                if(strstr(str, "facet ") != nullptr) {
                    f32 facet;
                    sscanf_s(str, " facet %f", &facet);
                    object->facet_ = facet;

                    break;
                }

                if(strstr(str, "face ") != nullptr) {
                    state = State::Face;
                    break;
                }

                break;
            case State::Vertex:   //---- 頂点解析中
                if(strstr(str, "}") != nullptr) {
                    state = State::Object;
                    break;
                }

                f32 x;
                f32 y;
                f32 z;

                sscanf_s(str, " %f %f %f", &x, &y, &z);

                x *= scale;
                y *= scale;
                z *= scale;

                object->vertices_.push_back(Vector3(x, y, z));

                break;

            case State::Face:   //---- フェイス解析中
            {
                if(strstr(str, "}") != nullptr) {
                    state = State::Object;
                    break;
                }

                // 三角形か四角形かをチェック
                s32 vertexCount;   // 3 or 4
                sscanf_s(str, " %d", &vertexCount);

                if(vertexCount <= 2) {   // 不正な値の場合はスキップ
                    break;
                }

                Face f;

                // マテリアル番号を取得
                f.material_ = -1;

                {
                    char* p = strstr(str, "M(");
                    if(p != nullptr) {
                        sscanf_s(p, "M(%d)", &f.material_);
                    }
                }

                if(vertexCount == 3) {
                    // 三角形の場合
                    char* p = strstr(str, "V(");

                    s32 index[3];
                    sscanf_s(p, "V(%d %d %d)", &index[0], &index[1], &index[2]);

                    f32 u[3]{};
                    f32 v[3]{};

                    p = strstr(str, "UV(");
                    if(p) {
                        sscanf_s(p, "UV(%f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2]);
                    }

                    f.vertexIndex_[0] = index[0];
                    f.vertexIndex_[1] = index[1];
                    f.vertexIndex_[2] = index[2];

                    const auto& v1         = object->vertices_[index[0]];
                    const auto& v2         = object->vertices_[index[1]];
                    const auto& v3         = object->vertices_[index[2]];
                    Vector3     faceNormal = Vector3::cross(v3 - v1, v2 - v1).normalize();
                    f.normal_[0]           = faceNormal;
                    f.normal_[1]           = faceNormal;
                    f.normal_[2]           = faceNormal;

                    f.u_[0] = u[0];
                    f.u_[1] = u[1];
                    f.u_[2] = u[2];

                    f.v_[0] = v[0];
                    f.v_[1] = v[1];
                    f.v_[2] = v[2];

                    // 三角形の登録
                    object->faces_.push_back(f);
                }
                else {
                    // 四角形の場合
                    char* p = strstr(str, "V(");

                    s32 index[4];
                    sscanf_s(p, "V(%d %d %d %d)", &index[0], &index[1], &index[2], &index[3]);

                    f32 u[4]{};
                    f32 v[4]{};

                    p = strstr(str, "UV(");
                    if(p) {
                        sscanf_s(p, "UV(%f %f %f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2], &u[3], &v[3]);
                    }

                    f.vertexIndex_[0] = index[0];
                    f.vertexIndex_[1] = index[1];
                    f.vertexIndex_[2] = index[2];

                    const auto& v1         = object->vertices_[index[0]];
                    const auto& v2         = object->vertices_[index[1]];
                    const auto& v3         = object->vertices_[index[2]];
                    Vector3     faceNormal = Vector3::cross(v3 - v1, v2 - v1).normalize();
                    f.normal_[0]           = faceNormal;
                    f.normal_[1]           = faceNormal;
                    f.normal_[2]           = faceNormal;
                    f.normal_[3]           = faceNormal;

                    f.u_[0] = u[0];
                    f.u_[1] = u[1];
                    f.u_[2] = u[2];
                    f.v_[0] = v[0];
                    f.v_[1] = v[1];
                    f.v_[2] = v[2];

                    object->faces_.push_back(f);

                    f.vertexIndex_[0] = index[2];
                    f.vertexIndex_[1] = index[3];
                    f.vertexIndex_[2] = index[0];

                    f.u_[0] = u[2];
                    f.u_[1] = u[3];
                    f.u_[2] = u[0];
                    f.v_[0] = v[2];
                    f.v_[1] = v[3];
                    f.v_[2] = v[0];

                    object->faces_.push_back(f);
                }

                break;
            }
            case State::Material: {
                if(str[0] == '}') {
                    state = State::Root;
                    break;
                }

                // col()構文を検出
                Vector4 color(1, 1, 1, 1);
                {
                    char* p = strstr(str, "col(");
                    if(p) {
                        sscanf_s(p, "col(%f %f %f %f)", &color.r_, &color.g_, &color.b_, &color.a_);
                    }
                }
                gpu::Texture* texture = nullptr;

                // tex("")構文を検出
                char* p = strstr(str, "tex(");
                if(p != nullptr) {
                    // tex(" の5文字分進める
                    p += 5;

                    // テクスチャ名を抽出
                    char* textureName = p;   // 先頭を保存
                    while(*p != '\"') {
                        p++;
                    }

                    *p = 0;

                    // テクスチャを読み込み

                    // MQOファイルのパス指定箇所のパス名のみ抽出
                    char drive[MAX_PATH];
                    char path[MAX_PATH];
                    char file[MAX_PATH];
                    char ext[MAX_PATH];
                    _splitpath_s(fileName, drive, path, file, ext);

                    char textureFullPath[MAX_PATH];
                    strcpy_s(textureFullPath, drive);
                    strcat_s(textureFullPath, path);
                    strcat_s(textureFullPath, textureName);

                    texture = gpu::createTexture(textureFullPath);
                    if(!texture) {
                        MessageBox(nullptr, textureFullPath, "テクスチャ読み込みに失敗しました.", MB_OK);
                        delete texture;
                        texture = nullptr;
                    }
                }

                // マテリアルを登録
                Material m;
                m.texture_ = texture;
                m.color_   = color;
                materials_.push_back(m);
                break;
            }
        }

        //		MessageBox(nullptr, str, "読み込み", MB_OK);
    }

    //-------------------------------------------------------------------------------
    // ファイルを閉じる
    //-------------------------------------------------------------------------------
    fclose(fp);

    return true;
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void ModelMQO::render()
{
    //----------------------------------------------------------------------------------
    // 点群の描画
    //----------------------------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {

		// オブジェクト内のすべての頂点
		for (auto& v : o->_vertices) {
			glVertex3fv((GLfloat*)& v);
		}
	}

	glEnd();

	glPointSize(1.0f);
#endif
    //=============================================================
    // ワイヤーフレームの描画
    //=============================================================
#if 0
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

	// 全てのオブジェクト部品
	for (auto& o : _objects) {
		auto& v = o->_vertices;	// 頂点配列  

		// 全てのフェイス
		for (auto& face : o->_faces) {
			glVertex3fv((GLfloat*)& v[face._vertexIndex[0]]);
			glVertex3fv((GLfloat*)& v[face._vertexIndex[1]]);

			glVertex3fv((GLfloat*)& v[face._vertexIndex[1]]);
			glVertex3fv((GLfloat*)& v[face._vertexIndex[2]]);

			glVertex3fv((GLfloat*)& v[face._vertexIndex[2]]);
			glVertex3fv((GLfloat*)& v[face._vertexIndex[0]]);
		}
	}

	glEnd();

	glColor3ub(128, 128, 128);
#endif
    //=============================================================
    // 三角形の描画
    //=============================================================
    s32 material = -1;   // 現在のマテリアル番号
    dxTexture(Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE).get());
    gpu::setShader("vsNormal.fx", "psNormal.fx");   // シェーダー

    u32 triangleCount = 0;

    // ポリゴンオフセット機能でデプスをずらす
    //   glEnable(GL_POLYGON_OFFSET_FILL);
    // glPolygonOffset(1.0f, 1.0f);

    // アルファテスト有効化
    //  glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GREATER, 0.5f);   // 比較条件　A > 0.5のときに描画する

    dxBegin(PT_TRIANGLES);
    // 全てのオブジェクト部品
    for(auto& o : objects_) {
        auto& v = o->vertices_;   // 頂点配列

        // オブジェクトカラー
        dxColor4f(o->color_.r_, o->color_.g_, o->color_.b_, o->color_.a_);

        // 全てのフェイス
        for(auto& face : o->faces_) {
            // マテリアルの変更
            if(material != face.material_ ||
               triangleCount > 65536

            ) {
                dxEnd();   //--- いったん描画コマンドを閉じる

                // テクスチャの設定
                gpu::Texture* texture = nullptr;

                if(face.material_ != -1) {
                    // テクスチャあり
                    texture = materials_[face.material_].texture_;
                    // マテリアルカラー
                    dxColor4f(materials_[face.material_].color_.r_,
                              materials_[face.material_].color_.g_,
                              materials_[face.material_].color_.b_,
                              materials_[face.material_].color_.a_);
                }
                else {
                    // オブジェクトカラー
                    dxColor4f(o->color_.r_, o->color_.g_, o->color_.b_, o->color_.a_);
                }
                dxTexture(texture ? texture : Asset::getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE).get());

                //gpu::setShader("vsNormal.fx", "psNormal.fx");   // シェーダー   // シェーダー

                dxBegin(PT_TRIANGLES);   //--- 再度開きなおす

                // 現在の番号を更新
                material      = face.material_;
                triangleCount = 0;
            }

            const auto& v1 = v[face.vertexIndex_[0]];
            const auto& v2 = v[face.vertexIndex_[1]];
            const auto& v3 = v[face.vertexIndex_[2]];

            dxTexCoord2f(face.u_[0], face.v_[0]);
            dxNormal3fv(face.normal_[0]);
            dxVertex3fv(v1);

            dxTexCoord2f(face.u_[1], face.v_[1]);
            dxNormal3fv(face.normal_[1]);
            dxVertex3fv(v2);

            dxTexCoord2f(face.u_[2], face.v_[2]);
            dxNormal3fv(face.normal_[2]);
            dxVertex3fv(v3);
            triangleCount++;
        }
    }
    dxEnd();

    // アルファテスト無効化
    //    glDisable(GL_ALPHA_TEST);

    // ポリゴンオフセット無効化
    // glDisable(GL_POLYGON_OFFSET_FILL);

    // テクスチャをもとに戻す
    dxTexture(nullptr);
}

//---------------------------------------------------------------------------
//!	最適化
//---------------------------------------------------------------------------
void ModelMQO::Object::optimize()
{
    // マテリアル単位でソート
    std::qsort(faces_.data(), faces_.size(), sizeof(ModelMQO::Face), &sortFunc);

    //=============================================================
    // 法線平滑化
    //=============================================================
    struct Point
    {
        Vector3 vertex_ = Vector3(0, 0, 0);
        Vector3 normal_ = Vector3(0, 0, 0);
        u32     count_  = 0;
    };
    std::vector<Point> points;
    std::vector<u32>   pointIndices;

    for(auto& f : faces_) {
        for(u32 i = 0; i < 3; ++i) {
            bool found = false;
            for(u32 index = 0; index < points.size(); ++index) {
                auto& p = points[index];
                if((p.vertex_ - vertices_[f.vertexIndex_[i]]).lengthSq() > 0.01f) {
                    continue;
                }

                // 法線の一致度チェック
                if(Vector3::dot(p.normal_.normalize(), f.normal_[i]) < cosf(facet_ * (3.1415f / 180.0f))) {
                    continue;
                }

                // すでに共有頂点があったらマージ
                p.normal_ += f.normal_[i];
                p.count_++;
                pointIndices.emplace_back(index);   // インデックス番号

                found = true;
                break;
            }

            // 新規登録
            if(!found) {
                pointIndices.emplace_back(points.size());   // インデックス番号

                Point p;
                p.vertex_ = vertices_[f.vertexIndex_[i]];
                p.normal_ = f.normal_[i];
                p.count_  = 1;
                points.emplace_back(std::move(p));
            }
        }
    }
    // 平均化
    for(auto& p : points) {
        p.normal_ /= f32(p.count_);
    }

    // 共有法線を書き戻し
    {
        u32 index = 0;
        for(auto& f : faces_) {
            //bool found = false;
            f.normal_[0] = points[pointIndices[index++]].normal_;
            f.normal_[1] = points[pointIndices[index++]].normal_;
            f.normal_[2] = points[pointIndices[index++]].normal_;
        }
    }
}

//---------------------------------------------------------------------------
//! Objectの個数を取得
//---------------------------------------------------------------------------
size_t ModelMQO::getObjectCount() const
{
	return objects_.size();
}
