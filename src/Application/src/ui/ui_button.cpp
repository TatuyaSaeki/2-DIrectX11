//===========================================================================
//! @file	UI_Button.cpp
//===========================================================================
#if 0
//===========================================================================
//! コンストラクタ
//===========================================================================
UI_Button::UI_Button()
	:deviationWidth_(0.0f)
{
}

//===========================================================================
//! デストラクタ
//===========================================================================
UI_Button::~UI_Button()
{
}

//===========================================================================
//! 初期化
//===========================================================================
bool UI_Button::initialize()
{
	std::array<std::string,4> path { { 
		{"../asset/texture2D/ui/CircleButton.png"},
		{"../asset/texture2D/ui/CrossButton.png"},
		{"../asset/texture2D/ui/SquareButton.png"},
		{"../asset/texture2D/ui/TriangleButton.png"},
	} };

	deviationWidth_ = 40.0f;

	for(u32 i = 0; i < 4; i++){ 
		AssetTexture* texture = new AssetTexture();
		texture->load(path[i]);

	//---- 頂点バッファ作成
		std::array<VertexTexture2D, 4>	vertices = { {
			{ Vector2(+0.0f,  +0.0f ),	Vector2(0.0f,0.0f) },
			{ Vector2(+deviationWidth_, +0.0f ),	Vector2(1.0f,0.0f) },
			{ Vector2(+deviationWidth_, +deviationWidth_),	Vector2(1.0f,1.0f) },
			{ Vector2(+0.0f,  +deviationWidth_),	Vector2(0.0f,1.0f) },
		} };

		if( !texture->createVertexBuffer(vertices.data(), vertices.size()) ) { 
			return false;
		}

		//---- インデックスバッファ作成
		std::array<u32, 6>	indices = { {
				0,  1,  2,
				2,  3,  0,
			} };

		if( !texture->createIndexBuffer(indices.data(),indices.size() ) ) { 
			return false;
		}

		textures_[i] = texture;
	}

	return true;
}

//===========================================================================
//! 更新
//===========================================================================
void UI_Button::update()
{
}

//===========================================================================
//! 描画
//===========================================================================
void UI_Button::render()
{	// 1100,500
	
	Vector2 position = Vector2(1100.0f,600.0f);

	std::array<Vector2, 4> positions {{
		{ position.x_+deviationWidth_, position.y_       },
		{ position.x_      , position.y_+deviationWidth_ },
		{ position.x_-deviationWidth_, position.y_       },
		{ position.x_      , position.y_-deviationWidth_ },	
	}};

	for(u32 i = 0; i < 4; i++) { 
		textures_[i]->render(positions[i]);
	}
}

//===========================================================================
//! 解放
//===========================================================================
void UI_Button::cleanup()
{
	for(auto texture : textures_) { 
		GM_SAFE_CLEANUP(texture);
	}
}
#endif