//===========================================================================
//!	@file	pointer.h
//!	@brief	�|�C���^
//===========================================================================
#pragma once

//===========================================================================
// ���p�|�C���^
//===========================================================================
template<typename T>
class ptr
{
public:
    ptr() = default;

    //! @brief �R���X�g���N�^(�|�C���^)
    ptr(T* p)
    : ptr_(p)
    {
    }

    // @brief �R���X�g���N�^(����)
    ptr(T& p)
    : ptr_(&p)
    {
    }

    // @brief �R���X�g���N�^ unique_ptr<T>
    template<typename U>
    ptr(std::unique_ptr<U>& p)
    : ptr_(p.get())
    {
    }

	// @brief �R���X�g���N�^ shared_ptr<T>
	template<typename S>
	ptr(std::shared_ptr<S>& p)
		: ptr_(p.get())
	{
	}

    //! @brief �|�C���^���Z
    T* operator->() const { return ptr_; }

    //! @brief �|�C���^���Z
    T& operator*() const { return ptr_; }

    //! @brief �L���X�g
    operator T*() const { return ptr_; }

private:
    T* ptr_;
};

//! ���S�ȉ���i��d����E�����|�C���^�h�~�j
template<class T>
inline void GM_SAFE_RELEASE(T*& p)
{
    if(p) {
        p->Release();
        p = nullptr;
    }
}

//! ���S�ȍ폜
template<class T>
inline void GM_SAFE_DELETE(T*& p)
{
    if(p) {
        delete p;
        p = nullptr;
    }
}

template<class T>
inline void GM_SAFE_CLEANUP(T*& p)
{
    if(p) {
        p->cleanup();
        GM_SAFE_DELETE(p);
    }
}
