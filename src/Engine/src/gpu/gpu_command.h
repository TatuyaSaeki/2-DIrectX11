//===========================================================================
//!	@file	gpu_command.h
//!	@brief	GPU�R�}���h
//===========================================================================
#pragma once

//! �v���~�e�B�u�̎��
enum PRIMITIVE_TYPE
{
    PT_POINTS          = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    PT_LINES           = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    PT_LINE_STRIP      = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PT_TRIANGLES       = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    PT_TRIANGLE_STRIP  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    PT_3_CONTROL_POINT = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
    PT_4_CONTROL_POINT = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
};

//! �O���錾
struct UseShader;

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! @brief ���̓��C�A�E�g�̐ݒ�
//! @param [in] inputLayout ���̓��C�A�E�g����
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout);

//---------------------------------------------------------------------------
//! @brief ���̓��C�A�E�g�̐ݒ�
//! @param [in] vsFileName ���_�V�F�[�_�[�̖��O
//---------------------------------------------------------------------------
void setInputLayout(const std::string& vsFileName);

//---------------------------------------------------------------------------
//! @brief ���_�o�b�t�@�̐ݒ�
//! @param [in] buffer  ���_�o�b�t�@
//! @param [in] stride  �z��e�v�f�̃I�t�Z�b�g�l?
//! @param [in] offset  �z��I�t�Z�b�g�l
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset = 0);

//---------------------------------------------------------------------------
//! @brief �C���f�b�N�X�o�b�t�@�̐ݒ�
//! @param [in] buffer  �C���f�b�N�X�o�b�t�@
//! @param [in] offset  �z��I�t�Z�b�g�l
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset = 0);

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̐ݒ�
//! �ʐݒ�ɂ��邩��
//!	@param	[in]	vs	���_�V�F�[�_�[
//!	@param	[in]	ps	�s�N�Z���V�F�[�_�[
//!	@param	[in]	gs	�W�I���g���V�F�[�_�[
//!	@param	[in]	hs	�n���V�F�[�_�[
//!	@param	[in]	ds	�h���C���V�F�[�_�[
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps, ptr<const gpu::ShaderGs> gs = nullptr, ptr<const gpu::ShaderHs> hs = nullptr, ptr<const gpu::ShaderDs> ds = nullptr);

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̐ݒ� (�t�@�C�����w��)
//! �ʐݒ�ɂ��邩��
//!	@param	[in]	vs	���_�V�F�[�_�[
//!	@param	[in]	ps	�s�N�Z���V�F�[�_�[
//!	@param	[in]	gs	�W�I���g���V�F�[�_�[
//!	@param	[in]	hs	�n���V�F�[�_�[
//!	@param	[in]	ds	�h���C���V�F�[�_�[
//---------------------------------------------------------------------------
void setShader(const std::string& fileNameVs, const std::string& fileNamePs, const std::string* fileNameGs = nullptr, const std::string* fileNameHs = nullptr, const std::string* fileNameDs = nullptr);
void setShader(const std::string& fileNameVs, const std::string* fileNamePs);
void setShader(const UseShader& shaderName);

//---------------------------------------------------------------------------
//! @brief �萔�o�b�t�@�̐ݒ�
//! @param [in] slot   GPU�X���b�g�ԍ�
//! @param [in] buffer �萔�o�b�t�@
//---------------------------------------------------------------------------
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer);

//---------------------------------------------------------------------------
//! @brief �e�N�X�`���̐ݒ�
//! @param [in] slot    GPU�X���b�g�ԍ�
//! @param [in] texture �e�N�X�`��
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture);

//---------------------------------------------------------------------------
//! @brief �`��
//!	@param [in]	type	�v���~�e�B�u�̎��
//!	@param [in]	vertexCount	���_��
//!	@param [in]	vertexOffet	���_�ԍ��̃I�t�Z�b�g
//---------------------------------------------------------------------------
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset = 0);

//---------------------------------------------------------------------------
//! @brief �C���f�b�N�X�`��
//!	@param [in]	type	�v���~�e�B�u�̎��
//!	@param [in]	indexCount	�C���f�b�N�X��
//!	@param [in]	indexOffset �C���f�b�N�X�ԍ��̃I�t�Z�b�g
//!	@param [in]	vertexOffet	���_�ԍ��̃I�t�Z�b�g
//---------------------------------------------------------------------------
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset = 0, u32 vertexOffset = 0);

//---------------------------------------------------------------------------
//! @brief �`��^�[�Q�b�g�̐ݒ�
//! @param [in] renderTarget �����_�[�^�[�Q�b�g
//! @param [in] depthStencil �[�x�X�e���V���o�b�t�@
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil = nullptr);

//---------------------------------------------------------------------------
//! @brief �`��^�[�Q�b�g�̐ݒ�(MRT�p)
//! @param [in] renderTargetCount  �}���`�^�[�Q�b�g�̐�
//! @param [in] renderTargets      �����_�[�^�[�Q�b�g�z��
//! @param [in] depthStencil       �[�x�X�e���V���o�b�t�@
//---------------------------------------------------------------------------
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil);

//---------------------------------------------------------------------------
//! @brief �`��^�[�Q�b�g�̃N���A
//! @param [in] target  �N���A�������o�b�t�@
//! @param [in] color   �N���A�������F
//! @param [in] depth �@�f�v�X     (�悭�킩���ĂȂ��A�܂������܂�)
//! @param [in] stencil �X�e���V�� (�悭�킩���ĂȂ��A�܂������܂�)
//---------------------------------------------------------------------------
void clearTarget(ptr<gpu::Texture> target, const Vector4& color = Vector4(0.0f, 0.0f, 0.0f, 0.0f), f32 depth = 1.0f, u8 stencil = 0);
;

}   // namespace gpu
