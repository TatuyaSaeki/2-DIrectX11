//================================================================
//! @file HitPoint
//================================================================
#if 0
//===========================================================================
//! ������
//===========================================================================
HitPoint::HitPoint()
: hitPointTexture_(nullptr)
, hitPointFrameTexture_(nullptr)
, hitPoint_(0)
{
}

//===========================================================================
//! ������
//===========================================================================
bool HitPoint::initialize(const std::string & filePath)
{
	hitPointTexture_ = new AssetTexture();
	hitPointFrameTexture_ = new AssetTexture();

	hitPointTexture_->load(filePath);
	hitPointFrameTexture_->load("../asset/texture2D/ui/hitPointFrame.png");

		//---- ���_�o�b�t�@�쐬
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

	//---- �C���f�b�N�X�o�b�t�@�쐬
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
//! �X�V
//===========================================================================
void HitPoint::update(const u32 hitPoint)
{
	if(hitPoint != hitPoint_) { 
		//---- ���_�o�b�t�@�쐬
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
//! ������
//===========================================================================
void HitPoint::render(const Vector2& position)
{
	hitPointTexture_->render(position);
	hitPointFrameTexture_->render(position);
}

//===========================================================================
//! ������
//===========================================================================
void HitPoint::cleanup()
{
	GM_SAFE_CLEANUP(hitPointTexture_);
	GM_SAFE_CLEANUP(hitPointFrameTexture_);
}
#endif