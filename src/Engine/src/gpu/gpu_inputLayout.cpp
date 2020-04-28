//===========================================================================
//!	@file	gpu_inputLayout.cpp
//!	@brief	���̓��C�A�E�g
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! �쐬
//---------------------------------------------------------------------------
bool InputLayout::create(const gpu::ShaderVs& shaderVs)
{
    //=============================================================
    // ���̓��C�A�E�g�̍쐬
	//=============================================================
    device::D3DDevice()->CreateInputLayout(
        desc_.data(),                     // D3D11_INPUT_ELEMENT_DESC�̐擪
        UINT(desc_.size()),               // �v�f��
        shaderVs.getShaderBinary(),       // ���_�V�F�[�_�[�̃V�F�[�_�[�o�C�i���[
        shaderVs.getShaderBinarySize(),   // ���_�V�F�[�_�[�̃o�C�i���[�T�C�Y
        &inputLayout_);

    if(!inputLayout_)
        return false;

    return true;
}

//---------------------------------------------------------------------------
//! D3D���̓��C�A�E�g���擾
//---------------------------------------------------------------------------
ID3D11InputLayout* InputLayout::getInputLayout()
{
    return inputLayout_.Get();
}

}   // namespace gpu
