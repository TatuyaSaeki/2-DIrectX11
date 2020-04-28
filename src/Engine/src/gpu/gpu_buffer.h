//===========================================================================
//!	@file	gpu_buffer.h
//!	@brief	GPU�o�b�t�@�[
//===========================================================================
#pragma once

#include "system/directX.h"         // DirectX11

//===========================================================================
//!	@namespace	gpu
//===========================================================================
namespace gpu {

//===========================================================================
//!	@class	Buffer
//===========================================================================
class Buffer
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    Buffer() = default;

    //! @brief �f�X�g���N�^
    virtual ~Buffer() = default;

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //-----------------------------------------------------------------------
    //! @brief ������
    //!	@param	[in]	size		�o�b�t�@�T�C�Y(�P��:byte)
    //!	@param	[in]	usage		�p�r
    //!	@param	[in]	bindFlags	�ݒ�摮��
    //!	@param	[in]	initialData	�����f�[�^�[(nullptr�̏ꍇ�͂Ȃ�)
    //! @return true	����I��
    //! @return false	�G���[�I��
    //-----------------------------------------------------------------------
    bool initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData = nullptr);

    //@}
    //-----------------------------------------------------------------------
    //!	@name	�擾
    //-----------------------------------------------------------------------
    //@{

	//-----------------------------------------------------------------------
    //! @brief  D3D�o�b�t�@���擾
    //! @return �o�b�t�@�̐��|�C���^
	//-----------------------------------------------------------------------
    ID3D11Buffer* getD3DBuffer() const { return d3dBuffer_.Get(); }

    //@}

private:
    com_ptr<ID3D11Buffer> d3dBuffer_;   //!< �o�b�t�@�ۑ��p
};

//===========================================================================
//!	@class	ConstantBuffer
//===========================================================================
template<typename T>
class ConstantBuffer final : public Buffer
{
public:
    //-----------------------------------------------------------------------
    //!	@name	������
    //-----------------------------------------------------------------------
    //@{

    //! @brief �R���X�g���N�^
    ConstantBuffer() = default;

    //! @brief �f�X�g���N�^
    ~ConstantBuffer() override = default;

    //-----------------------------------------------------------------------
    //!	@name	�^�X�N
    //-----------------------------------------------------------------------
    //@{

    //! @brief �X�V�J�n
    T* begin()
    {
        // ������s���̓o�b�t�@���쐬
        if(!getD3DBuffer()) {
            create();
        }

        // �o�b�t�@��map����
        auto                     d3dBuffer = getD3DBuffer();
        D3D11_MAPPED_SUBRESOURCE data;
        device::D3DContext()->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

        return reinterpret_cast<T*>(data.pData);
    }

    //! @brief �X�V�I��
    void end()
    {
        // �o�b�t�@��unmap����
        auto d3dBuffer = getD3DBuffer();
        device::D3DContext()->Unmap(d3dBuffer, 0);
    };

private:
    //! @brief �o�b�t�@�쐬
    bool create()
    {
        auto size = (sizeof(T) + 15) & 0xfffffff0UL;   // 16�̔{���ɐ؂�グ
        if(!initialize(size, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER)) {
            return false;
        }
        return true;
    }

    //@}
};

}   // namespace gpu
