//===========================================================================
//! @file	psFilterGlare_Glare.fx
//! @breif	グレアフィルター
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//! ピクセルシェーダー (テクスチャあり)
//===========================================================================
float4 main(PS_INPUT input)
: SV_Target
{
    uint w;
    uint h;
    uint mipLevels;
    BaseTexture.GetDimensions(0, w, h, mipLevels);

    float2 dir = dir_ / float2(w, h) * stride_;   // 1ピクセル分の移動量

    float4 color = 0;

    static const int SAMPLE_COUNT = 8;
    for(int i = 0; i < SAMPLE_COUNT; ++i) {
        float a = pow(saturate(attenuation_), i);
        color += BaseTexture.Sample(ClampSampler, input.uv_ + dir * i * (1.0 / SAMPLE_COUNT)) * a;
    }
    return color;
}
