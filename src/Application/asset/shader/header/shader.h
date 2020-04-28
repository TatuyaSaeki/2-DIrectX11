//===========================================================================
//!	@file	shader.h
//!	@brief	HLSL�\�[�X�R�[�h
//===========================================================================
#include "../shader/header/constant_buffer.h"
#include "../shader/header/ibl.h"
#include "../shader/header/pbr.h"

//---------------------------------------------------------------------------
//! Lambert�g�U����
//!	@param	[in]	N	���K���@��
//!	@param	[in]	L	���K�����C�g�ւ̃x�N�g��
//!	@return	�f�B�t�[�Y�l
//---------------------------------------------------------------------------
float3 diffuseLambert(float3 N, float3 L)
{
    return saturate(dot(N, L));
}

//---------------------------------------------------------------------------
//! blinn-Phong���ʔ���
//!	@param	[in]	N			���K���@��
//!	@param	[in]	L			���K�����C�g�ւ̃x�N�g��
//!	@param	[in]	V			���K���J�����ւ̃x�N�g��
//!	@param	[in]	intensity	���C�g�̋���
//!	@return	�X�y�L�����[�x����
//---------------------------------------------------------------------------
float specularBlinnPhong(float3 N, float3 L, float3 V, float intensity)
{
    float3 H = normalize(L + V);

    return pow(saturate(dot(H, N)), intensity);
}

//---------------------------------------------------------------------------
//! ���s����
//!	@param	[in]	roughness	�e��
//!	@param	[in]	metalness	�����x
//!	@param	[in]	N			���K���@��
//!	@param	[in]	L			���K�����C�g�ւ̃x�N�g��
//!	@param	[in]	V			���K���J�����ւ̃x�N�g��
//!	@param	[in]	H			L,V�̃n�[�t�x�N�g��
//!	@param	[in]	baseColor	���ƂȂ�F
//!	@return	�f�B�t�[�Y�A�X�y�L�����[�x����
//---------------------------------------------------------------------------
Material directionalLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float3 baseColor)
{
    Material returnMaterial;

    // Lambert�g�U����
    float3 diffuse = diffuseLambertPBR(N, L);
    // CookTorrance���ʔ���
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    returnMaterial.diffuse_  = diffuse;
    returnMaterial.specular_ = specular;

    return returnMaterial;
}
//---------------------------------------------------------------------------
//! �_����
//!	@param	[in]	roughness	�e��
//!	@param	[in]	metalness	�����x
//!	@param	[in]	N			���K���@��
//!	@param	[in]	L			���K�����C�g�ւ̃x�N�g��
//!	@param	[in]	V			���K���J�����ւ̃x�N�g��
//!	@param	[in]	H			L,V�̃n�[�t�x�N�g��
//!	@param	[in]	len			�I�u�W�F�N�g�ƃ��C�g�̋���
//!	@return	�f�B�t�[�Y�A�X�y�L�����[�x����
//---------------------------------------------------------------------------
Material pointLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float len, float3 baseColor)
{
    Material returnMaterial;

    float3 diffuse  = diffuseLambertPBR(N, L);
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    float attenuation = saturate(1.0 / (0.0 * 0.0 * len + 0.1 * len * len));

    returnMaterial.diffuse_  = diffuse * attenuation;
    returnMaterial.specular_ = specular * attenuation;

    return returnMaterial;
}

//---------------------------------------------------------------------------
//! �X�|�b�g���C�g
//!	@param	[in]	roughness	�e��
//!	@param	[in]	metalness	�����x
//!	@param	[in]	N			���K���@��
//!	@param	[in]	L			���K�����C�g�ւ̃x�N�g��
//!	@param	[in]	V			���K���J�����ւ̃x�N�g��
//!	@param	[in]	H			L,V�̃n�[�t�x�N�g��
//!	@param	[in]	len			�I�u�W�F�N�g�ƃ��C�g�̋���
//!	@param	[in]	L2			���K���I�u�W�F�N�g�ƃ��C�g�̋���
//!	@return	�f�B�t�[�Y�A�X�y�L�����[�x����
//---------------------------------------------------------------------------
Material spotLight(float roughness, float metalness, float3 N, float3 L, float3 V, float3 H, float len, float3 baseColor, float3 L2 = 0)
{
    Material returnMaterial;

    float3 diffuse  = diffuseLambertPBR(N, -L);
    float3 specular = CookTorrance(roughness, metalness, N, L, V, H, baseColor);

    float attenuation = saturate(1.0 / (0.0 * 0.0 * len + 0.1 * len * len));

    returnMaterial.diffuse_  = diffuse * attenuation;
    returnMaterial.specular_ = specular * attenuation;

    // �͈�
    float cos = saturate(dot(L, -L2));
    if(cos < 0.95) {
        returnMaterial.diffuse_ *= pow(cos / 4.0, 30.0 * (0.95 - cos));
    }
    returnMaterial.diffuse_ *= 8;

    return returnMaterial;
}

//---------------------------------------------------------------------------
//! �V���h�E�p�֐�
//!	@param	[in]	shadowmapUv		UV�l	
//!	@return	���̏ꏊ�̃s�N�Z���J���[
//---------------------------------------------------------------------------
float shadowSample(float3 shadowmapUv)
{
    float shadowDepth = DepthTexture.Sample(LinearSampler, shadowmapUv.xy).r;

    float              shadow      = 1.0;
    static const float SHADOW_BIAS = 0.00005;

    shadow = (shadowDepth + SHADOW_BIAS < shadowmapUv.z) ? 0.01 : 1.0;

    return shadow;
}

//---------------------------------------------------------------------------
//! ���������֐�
//!	@param	[in]	st	
//!	@return	����
//---------------------------------------------------------------------------
float rand(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

//---------------------------------------------------------------------------
//! vogelDisk
//!	@param	[in]	sampleIndex	
//!	@param	[in]	sampleCount
//!	@param	[in]	phi	
//!	@return	
//---------------------------------------------------------------------------
float2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float goldenAngle = 2.4;   // �����p

    float r     = sqrt(sampleIndex + 0.5) / sqrt(samplesCount);
    float theta = sampleIndex * goldenAngle + phi;

    float sine, cosine;
    sincos(theta, sine, cosine);

    return float2(r * cosine, r * sine);
}

//-------------------------------------------------------------
//! �@���}�b�s���O
//!	@param	[in]	input	PS_INPUT_SHADOW���͏��
//! @return	�v�Z�ςݖ@��
//-------------------------------------------------------------
float3 normalMapping(PS_INPUT_SHADOW input)
{
    float3 normalTexture = NormalTexture.Sample(LinearSampler, input.uv_).rgb;
    normalTexture        = normalize(normalTexture * 2.0 - 1.0);

    // �@���x�N�g���̉�]�s����쐬
    float3 N = normalize(input.normal_);
    float3 T = normalize(input.tangent_);
    float3 B = normalize(input.binormal_);

    float3x3 TBN = { T, B, N };
    //TBN = transpose(TBN);	// �t�s��쐬
    N = mul(normalTexture, TBN);

    return normalize(N);
}
