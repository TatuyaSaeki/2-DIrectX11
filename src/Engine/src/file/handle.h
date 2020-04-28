//===========================================================================
//!	@brief	オブジェクトハンドル
// オブジェクトを直接のポインタではなく弱参照で保持します
//===========================================================================
#pragma once

template<typename T>
class Handle
{
public:
    // 初期化
    Handle() = default;

    // 初期化
    explicit Handle(T* p)
    : ptr_(p)
    {
    }

    // move演算子
    explicit Handle(Handle&& other)
    {
		std::swap(*this, other);
    }

	// 解放
    ~Handle()
    {
        reset();
    }

    // ポインタを解放
    void reset(T* p = nullptr)
    {
       ptr_.reset(p);
    }


    // ポインタが有効かどうか
    bool expired() { return !ptr_.get(); }

    // ポインタ参照
    T* operator->() { return ptr_.get(); }

    // ポインタ参照
    T* get() { return ptr_.get(); }

private:
    std::unique_ptr<T> ptr_;
};