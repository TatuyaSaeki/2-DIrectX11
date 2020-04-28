//===========================================================================
//!	@file	gpu_buffer.cpp
//!	@brief	GPUバッファー
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Buffer::initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData)
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth      = UINT((size + 15) & ~(16ul - 1));   // バッファのサイズ(16の倍数)
    desc.Usage          = usage;                             // バッファの配置場所(VRAM内)
    desc.BindFlags      = bindFlags;                         // 頂点バッファとして設定
    desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

    // バッファの初期データを渡して頂点バッファを作成
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = initialData;   // 初期データの先頭アドレス

    if(FAILED(device::D3DDevice()->CreateBuffer(&desc, initialData ? &initData : nullptr, d3dBuffer_.GetAddressOf()))) {
        return false;
    }
    return true;
}

}   // namespace gpu
