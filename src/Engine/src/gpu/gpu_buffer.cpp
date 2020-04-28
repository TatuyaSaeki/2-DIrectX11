//===========================================================================
//!	@file	gpu_buffer.cpp
//!	@brief	GPU�o�b�t�@�[
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool Buffer::initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData)
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth      = UINT((size + 15) & ~(16ul - 1));   // �o�b�t�@�̃T�C�Y(16�̔{��)
    desc.Usage          = usage;                             // �o�b�t�@�̔z�u�ꏊ(VRAM��)
    desc.BindFlags      = bindFlags;                         // ���_�o�b�t�@�Ƃ��Đݒ�
    desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

    // �o�b�t�@�̏����f�[�^��n���Ē��_�o�b�t�@���쐬
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = initialData;   // �����f�[�^�̐擪�A�h���X

    if(FAILED(device::D3DDevice()->CreateBuffer(&desc, initialData ? &initData : nullptr, d3dBuffer_.GetAddressOf()))) {
        return false;
    }
    return true;
}

}   // namespace gpu
