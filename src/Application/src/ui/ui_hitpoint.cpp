//================================================================
//! @file HitPoint
//================================================================
#if 0
//===========================================================================
//! 初期化
//===========================================================================
HitPoint::HitPoint()
: hitPointTexture_(nullptr)
, hitPointFrameTexture_(nullptr)
, hitPoint_(0)
{
}

//===========================================================================
//! 初期化
//===========================================================================
bool HitPoint::initialize(const std::string & filePath)
{
	hitPointTexture_ = new AssetTexture();
	hitPointFrameTexture_ = new AssetTexture();

	hitPointTexture_->load(filePath);
	hitPointFrameTexture_->load("../asset/texture2D/ui/hitPointFrame.png");

		//---- 頂点バッファ作成
	std::array<VertexTexture2D, 4>	vertices = { {
		{ Vector2(+0.0f,   +0.0f ),	Vector2(0.0f,0.0f) },
		{ Vector2(+350.0f, +0.0f ),	Vector2(1.0f,0.0f) },
		{ Vector2(+350.0f, +50.0f),	Vector2(1.0f,1.0f) },
		{ Vector2(+0.0f,   +50.0f),	Vector2(0.0f,1.0f) },
	} };

	if( !hitPointTexture_->createVertexBuffer(vertices.data(), vertices.size()) ) { 
		return false;
	}
	if( !hitPointFrameTexture_->createVertexBuffer(vertices.data(), vertices.size()) ) { 
		return false;
	}

	//---- インデックスバッファ作成
	std::array<u32, 6>	indices = { {
			0,  1,  2,
			2,  3,  0,
	} };

	if( !hitPointTexture_->createIndexBuffer(indices.data(),indices.size() ) ) { 
		return false;
	}
	if( !hitPointFrameTexture_->createIndexBuffer(indices.data(),indices.size() ) ) { 
		return false;
	}

	return true;
}

//===========================================================================
//! 更新
//===========================================================================
void HitPoint::update(const u32 hitPoint)
{
	if(hitPoint != hitPoint_) { 
		//---- 頂点バッファ作成
		std::array<VertexTexture2D, 4>	vertices = { {
		{ Vector2(+0.0f,   +0.0f ),	Vector2(0.0f,0.0f) },
		{ Vector2(+350.0f, +0.0f ),	Vector2(1.0f,0.0f) },
		{ Vector2(+350.0f, +50.0f),	Vector2(1.0f,1.0f) },
		{ Vector2(+0.0f,   +50.0f),	Vector2(0.0f,1.0f) },
	} };

		if( !hitPointTexture_->createVertexBuffer(vertices.data(), vertices.size()) ) { 
			assert(false);
		}

		hitPoint_ = hitPoint;
	}
}

//===========================================================================
//! 初期化
//===========================================================================
void HitPoint::render(const Vector2& position)
{
	hitPointTexture_->render(position);
	hitPointFrameTexture_->render(position);
}

//===========================================================================
//! 初期化
//===========================================================================
void HitPoint::cleanup()
{
	GM_SAFE_CLEANUP(hitPointTexture_);
	GM_SAFE_CLEANUP(hitPointFrameTexture_);
}
#endif