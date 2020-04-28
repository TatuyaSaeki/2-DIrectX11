//===========================================================================
//!	@file	primitive.cpp
//!	@brief	�v���~�e�B�u�`��
//===========================================================================
namespace {

std::vector<Matrix> matrixStack_;
Matrix              matWorld_ = Matrix::identity();

gpu::Buffer*                   dynamicBuffer_;
std::unique_ptr<gpu::ShaderVs> shaderVs3D_;
std::unique_ptr<gpu::ShaderPs> shaderPsFlat_;
std::unique_ptr<gpu::ShaderPs> shaderPsTexture_;
gpu::ConstantBuffer<cbModel>  cbWorld_;

const u32 MAX_VERTEX_COUNT = 65536* 1024;   // �ő�o�^���_��
const u32 BUFFER_SIZE      = u32(sizeof(Vertex)) * MAX_VERTEX_COUNT;

u32            usedCount_ = 0;      // �g�p���̒��_��
u32            drawCount_;          // ���_��
Vertex         vertex_;             // ���_�̈ꎞ�̈�
PRIMITIVE_TYPE type_ = PT_POINTS;   // ���݂̃v���~�e�B�u�̎��
Vertex*        pvertex_;            // ���݃}�b�v����Ă��钸�_�̐擪

}   // namespace

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool PRIM_Initialize()
{
    // ���I���_�o�b�t�@�̏�����
    dynamicBuffer_ = new gpu::Buffer();
    if(dynamicBuffer_->initialize(BUFFER_SIZE, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER) == false) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void PRIM_Finalize()
{
    shaderVs3D_.reset();
    shaderPsFlat_.reset();
    shaderPsTexture_.reset();

    GM_SAFE_DELETE(dynamicBuffer_);
}

//==============================================================
// OpenGL�݊��֐�
//==============================================================

//---------------------------------------------------------------------------
//! �s��̐ݒ�
//---------------------------------------------------------------------------
void dxLoadMatrixf(const Matrix& matWorld)
{
    auto p       = cbWorld_.begin();
    p->matWorld_ = matWorld;
    cbWorld_.end();

    // �����ɑޔ�
    matWorld_ = matWorld;

    gpu::setConstantBuffer(1, cbWorld_);
}

//---------------------------------------------------------------------------
//! �s��X�^�b�N�֑ޔ�
//---------------------------------------------------------------------------
void dxPushMatrix()
{
    matrixStack_.emplace_back(matWorld_);
}

//---------------------------------------------------------------------------
//! �s��X�^�b�N���畜�A
//---------------------------------------------------------------------------
void dxPopMatrix()
{
    matWorld_ = matrixStack_.back();
    matrixStack_.pop_back();
    dxLoadMatrixf(matWorld_);   // �Đݒ�
}

//---------------------------------------------------------------------------
//! �`��J�n
//!	@param	[in]	type	�v���~�e�B�u�̎��
//---------------------------------------------------------------------------
void dxBegin(PRIMITIVE_TYPE type)
{
    // �������}�b�v�J�n
    D3D11_MAP map = D3D11_MAP_WRITE_NO_OVERWRITE;

    // ���t�߂��Ȃ�Ċm��
    if(MAX_VERTEX_COUNT - 65536 * 10 < usedCount_) {
        map        = D3D11_MAP_WRITE_DISCARD;
        usedCount_ = 0;   // �ŏ����痘�p
    }

    D3D11_MAPPED_SUBRESOURCE mapped;

    device::D3DContext()->Map(dynamicBuffer_->getD3DBuffer(), 0, map, 0, &mapped);

    // ���p���̐擪�A�h���X
    pvertex_ = &static_cast<Vertex*>(mapped.pData)[usedCount_];

    // ��Ɠ���
    //pvertex_ = static_cast<Vertex*> (mapped.pData);
    //++usedCount_;

    type_ = type;
}

//---------------------------------------------------------------------------
//! ���_�L�b�N (Kick)
//---------------------------------------------------------------------------
void dxVertex3f(f32 x, f32 y, f32 z)
{
    // �l�̐ݒ�
    vertex_.position_.x_ = x;
    vertex_.position_.y_ = y;
    vertex_.position_.z_ = z;

    // ���_�L�b�N
    *pvertex_++ = vertex_;   // �ꎞ���[�N���珑���o��
    drawCount_++;
}

//---------------------------------------------------------------------------
//! ���_�L�b�N (Kick) Vector3
//---------------------------------------------------------------------------
void dxVertex3fv(const Vector3& pos)
{
    dxVertex3f(pos.x_, pos.y_, pos.z_);
}

//---------------------------------------------------------------------------
//! �J���[u8
//---------------------------------------------------------------------------
void dxColor4ub(u8 r, u8 g, u8 b, u8 a)
{
    vertex_.color_ = Color(r, g, b, a);
}

//------------------------------------------------------
//! �J���[u8
//------------------------------------------------------
void dxColor4ub(const Color& color)
{
    vertex_.color_ = Color(color.r_, color.g_, color.b_, color.a_);
}

//---------------------------------------------------------------------------
//! �J���[f32
//---------------------------------------------------------------------------
void dxColor4f(f32 fr, f32 fg, f32 fb, f32 fa)
{
    u8 r = (u8)std::max(0.0f, std::min(fr * 255.5f, 255.5f));
	u8 g = (u8)std::max(0.0f, std::min(fg * 255.5f, 255.5f));
	u8 b = (u8)std::max(0.0f, std::min(fb * 255.5f, 255.5f));
	u8 a = (u8)std::max(0.0f, std::min(fa * 255.5f, 255.5f));

    vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! �J���[f32
//---------------------------------------------------------------------------
void dxColor4f(const Vector4& color)
{
	u8 r = (u8)std::max(0.0f, std::min(color.r_ * 255.5f, 255.5f));
	u8 g = (u8)std::max(0.0f, std::min(color.g_ * 255.5f, 255.5f));
	u8 b = (u8)std::max(0.0f, std::min(color.b_ * 255.5f, 255.5f));
	u8 a = (u8)std::max(0.0f, std::min(color.a_ * 255.5f, 255.5f));

	vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! �e�N�X�`�����W
//---------------------------------------------------------------------------
void dxTexCoord2f(f32 u, f32 v)
{
    vertex_.uv_.x_ = u;
    vertex_.uv_.y_ = v;
}

//---------------------------------------------------------------------------
//! �@��
//---------------------------------------------------------------------------
void dxNormal3fv(const Vector3& v)
{
	// �l�̐ݒ�
	vertex_.normal_.x_ = v.x_;
	vertex_.normal_.y_ = v.y_;
	vertex_.normal_.z_ = v.z_;
}

//---------------------------------------------------------------------------
//! �`��I��
//---------------------------------------------------------------------------
void dxEnd()
{
    // �������}�b�s���O�I��
    device::D3DContext()->Unmap(dynamicBuffer_->getD3DBuffer(), 0);

    // �`��
    gpu::setVertexBuffer(dynamicBuffer_, sizeof(Vertex));   // ���_�o�b�t�@

    // �`��
    gpu::draw(type_, drawCount_, usedCount_);

    usedCount_ += drawCount_;
    drawCount_ = 0;
}

//---------------------------------------------------------------------------
//! �e�N�X�`���̐ݒ�
//---------------------------------------------------------------------------
void dxTexture(ptr<gpu::Texture> texture)
{
	auto shaderVs	 = ShaderManager::getShaderVs("vsStandard");
	auto shaderPs	 = ShaderManager::getShaderPs("psStandard");
	auto shaderPsTex = ShaderManager::getShaderPs("psTexture3D");

    gpu::setShader(shaderVs, (texture) ? shaderPsTex : shaderPs);
    gpu::setTexture(0, texture);
}
