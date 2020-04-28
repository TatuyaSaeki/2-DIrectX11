//===========================================================================
//!	@file	pointer.h
//!	@brief	ポインタ
//===========================================================================
#pragma once

//===========================================================================
// 中継ポインタ
//===========================================================================
template<typename T>
class ptr
{
public:
    ptr() = default;

    //! @brief コンストラクタ(ポインタ)
    ptr(T* p)
    : ptr_(p)
    {
    }

    // @brief コンストラクタ(実体)
    ptr(T& p)
    : ptr_(&p)
    {
    }

    // @brief コンストラクタ unique_ptr<T>
    template<typename U>
    ptr(std::unique_ptr<U>& p)
    : ptr_(p.get())
    {
    }

	// @brief コンストラクタ shared_ptr<T>
	template<typename S>
	ptr(std::shared_ptr<S>& p)
		: ptr_(p.get())
	{
	}

    //! @brief ポインタ演算
    T* operator->() const { return ptr_; }

    //! @brief ポインタ演算
    T& operator*() const { return ptr_; }

    //! @brief キャスト
    operator T*() const { return ptr_; }

private:
    T* ptr_;
};

//! 安全な解放（二重解放・無効ポインタ防止）
template<class T>
inline void GM_SAFE_RELEASE(T*& p)
{
    if(p) {
        p->Release();
        p = nullptr;
    }
}

//! 安全な削除
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
