//===========================================================================
//!	@file	constant_buffer.h
//!	@brief	�萔�o�b�t�@�Ƃ�
//===========================================================================

// PI
static const float PI = 3.141592;

//	�}�e���A��
struct Material
{
	float3 diffuse_;
	float3 specular_;
};

// clang-format off
//---------------------------------------------------------------------------
//	���f���p�e�N�X�`��
//---------------------------------------------------------------------------
Texture2D  BaseTexture			: register(t0);		//!< �x�[�X�ɂ���e�N�X�`��(Albedo)
Texture2D  RoughnessTexture		: register(t1);		//!< ���t�l�X
Texture2D  MetalnessTexture		: register(t2);		//!< ���^���l�X
Texture2D  NormalTexture		: register(t3);		//!< �@���}�b�v
Texture2D  AoTexture			: register(t4);		//!< Ao
Texture2D  SubTexture			: register(t5);		//!< �����ĂȂ�����,
TextureCube iblDiffuseTexture	: register(t6);		//!< IBL�p�f�B�t���[�Y
TextureCube iblSpecularTexture	: register(t7);		//!< IBL�p�X�y�L�����[
Texture2D   DepthTexture		: register(t10);	//!< �[�x�e�N�X�`��
Texture2D   ToonTexture			: register(t11);	//!< Toon�p
Texture2D   ThinTexture			: register(t12);	//!< Thin�p(����)
TextureCube cubeTexture			: register(t13);	//!< IBL�p�ɍ�����L���[�u�e�N�X�`��
TextureCube cubeDepthTexture	: register(t14);	//!< IBL�p�ɍ�����[�x�L���[�u�e�N�X�`��
Texture2D   HeightTexture		: register(t15);	//!< �����}�b�v

//---------------------------------------------------------------------------
// ���p
//---------------------------------------------------------------------------
Texture2D NormalTexture1 : register(t20);	//!< ���p�@���}�b�v1
Texture2D NormalTexture2 : register(t21);	//!< ���p�@���}�b�v2
Texture2D HeightTexture1 : register(t22);	//!< ���p�����}�b�v1
Texture2D HeightTexture2 : register(t23);	//!< ���p�����}�b�v2

//---------------------------------------------------------------------------
// �T���v���[
//---------------------------------------------------------------------------
SamplerState LinearSampler : register(s0);	//!< �ʏ�T���v���[
SamplerState DepthSampler  : register(s1);	//!< �f�v�X�p�T���v���[(�g���ĂȂ�)
SamplerState ClampSampler  : register(s2);	//!< �N�����v�p�T���v���[

//---------------------------------------------------------------------------
//	�萔�o�b�t�@(�f�B�ӂ��[�ǂł���Ȃ��Ȃ�?)
//---------------------------------------------------------------------------
static const int LIGHT_MAX_COUNT  = 16;	//!< ���C�g�ő��
static const int MODEL_MAX_COUNT  = 8;	//!< ���f���ő��
static const int CAMERA_MAX_COUNT = 8;	//!< �J�����ő��

//  �V�[���P�ʂōX�V�����萔�o�b�t�@
cbuffer cbCamera : register(b0)
{
	matrix matView_[CAMERA_MAX_COUNT];          //!< �r���[�s��
	matrix matProj_[CAMERA_MAX_COUNT];          //!< ���e�s��
	float3 cameraPosition_[CAMERA_MAX_COUNT];   //!< �J�����̃��[���h���W
};

//  ���f���P�ʂōX�V�����萔�o�b�t�@
cbuffer cbModel : register(b1)
{
	matrix matWorld_;   //!< ���[���h�s��
};

// ���s����
cbuffer cbDirectionalLight : register(b4)
{
	// dl = directionalLight
	float4 dlPosition_;		//!< ���C�g�̈ʒu(���ʔ��˗p)
	float4 dlColor_;		//!< ���C�g�̃J���[
	float3 dlLookAt_;		//!< ���C�g�̌���
	float  dlIntensity_;	//!< ���C�g�̋��x(���邳)
	bool   dlIsActive_;		//!< ���
}

// �_����
cbuffer cbPointLight : register(b5)
{
	// pl = pointLight
	int plCount_;   //!< ����_�����̍ő吔

	float4 plIntensity_[LIGHT_MAX_COUNT];     //!< ���C�g�̋��x(���邳)
	float4 plPosition_[LIGHT_MAX_COUNT];      //!< ���C�g�̈ʒu
	float4 plColor_[LIGHT_MAX_COUNT];         //!< ���C�g�̃J���[
	float4 plAttenuation_[LIGHT_MAX_COUNT];   //!< ���C�g�̌����p�����[�^�[(0.0, 0.0, 0.3)
}

// �X�|�b�g���C�g
cbuffer cbSpotLight : register(b6)
{
	// sl = spotLight
	int slCount_;   //!< ��������̍ő吔

	float4 slIntensity_[LIGHT_MAX_COUNT];     //!< ���C�g�̋��x(���邳)
	float4 slPosition_[LIGHT_MAX_COUNT];      //!< ���C�g�̈ʒu
	float4 slLookAt_[LIGHT_MAX_COUNT];        //!< ���C�g�̒����_
	float4 slColor_[LIGHT_MAX_COUNT];         //!< ���C�g�̃J���[
	float4 slAttenuation_[LIGHT_MAX_COUNT];   //!< ���C�g�̌����p�����[�^�[(0.0, 0.0, 0.3)
}

// �t�B���^�p
cbuffer cbFilter : register(b7)
{
	float  stride_;
	float  attenuation_;	//!< �����x
	float2 dir_;			//!< �O���A�̕���
}

//�@�e & SSS
cbuffer cbShadow : register(b8)
{
	matrix matLightView_;	//!< ���C�g�r���[�s��
	matrix matLightProj_;	//!< ���C�g�v���W�F�N�V�����s��
}

// �g�p
cbuffer cbWave : register(b10)
{
	float waveDistance_;	//!< 1�t���[���Ői�܂��鋗��
	float waveFrequency_;	//!< �ׂ���
	float waveSharp_;		//!< ���x
	float waveHeight_;		//!< ����
};

//--------------------------------------------------------------
//	�V�F�[�_�[�ւ̓��͍\����
//--------------------------------------------------------------

// ���_�V�F�[�_�[
struct VS_INPUT
{
	float4 position_	: POSITION;   // ���W�ϊ��O�̈ʒu���W
	float3 normal_		: NORMAL;
	float3 tangent_		: TANGENT;
	float3 binormal_	: BINORMAL;
	float4 color_		: COLOR;
	float2 uv_			: TEXCOORD0;   // �e�N�X�`��UV���W
};

// �s�N�Z���V�F�[�_�[
struct PS_INPUT
{
	float4 position_		: SV_Position;   // ���_�V�F�[�_�[����̏o�͂�SV_Position�ɏo�͂��邱��
	float3 normal_			: NORMAL;
	float4 color_			: COLOR;
	float3 tangent_			: TANGENT;
	float3 binormal_		: BINORMAL;
	float2 uv_				: TEXCOORD0;   // �e�N�X�`��UV���W
	float3 worldPosition_	: TEXCOORD1;
};

// ���_�V�F�[�_�[(2D)
struct VS_INPUT_2D
{
	float2 position_	: POSITION;
	float2 uv_			: TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[(2D)
struct PS_INPUT_2D
{
	float4 position_	: SV_Position;   // ���_�V�F�[�_�[����̏o�͂�SV_Position�ɏo�͂��邱��
	float2 uv_			: TEXCOORD0;   // �e�N�X�`��UV���W
};

// �V���h�E
struct PS_INPUT_SHADOW
{
	float4 position_		: SV_Position;   // ���_�V�F�[�_�[����̏o�͂�SV_Position�ɏo�͂��邱��
	float3 normal_			: NORMAL;
	float3 tangent_			: TANGENT;
	float3 binormal_		: BINORMAL;
	float4 color_			: COLOR;
	float2 uv_				: TEXCOORD0;   // �e�N�X�`��UV���W
	float3 worldPosition_	: TEXCOORD1;
	float4 shadowMapUv_		: TEXCOORD2;
};

// ���_�V�F�[�_�[
struct VS_INPUT_BUMP
{
	float4 position_	: POSITION;   // ���W�ϊ��O�̈ʒu���W
	float3 normal_		: NORMAL;       // 
	float4 color_		: COLOR;
	float2 uv_			: TEXCOORD0;   // �e�N�X�`��UV���W
	float4 tangent		: TEXCOORD1;
};

// �n���V�F�[�_�[�p�b�`�萔�֐�(�l�p)
struct HS_CONSTANT_QUAD_INPUT
{
	float factor[4]			: SV_TessFactor;
	float inner_factor[2]	: SV_InsideTessFactor;
};

// �n���V�F�[�_�[�p�b�`�萔�֐�(�O�p)
struct HS_CONSTANT_TRI_INPUT
{
	float factor[3]		: SV_TessFactor;
	float inner_factor	: SV_InsideTessFactor;
};

// �n���V�F�[�_�[���͗p
struct HS_INPUT
{
	float4 position_	: POSITION;
	float2 uv_			: TEXCOORD0;
	float3 normal_		: NORMAL;
	float3 tangent_		: TANGENT;
	float3 binormal_	: BINORMAL;
};

// �h���C���V�F�[�_�[���͗p
struct DS_INPUT
{
	float4 position_		: SV_Position;
	float2 uv_				: TEXCOORD0;
	float3 worldPosition_	: TEXCOORD1;
	float4 grapPosition_	: TEXCOORD2;

	float3 normal_		: NORMAL;
	float3 tangent_		: TANGENT;
	float3 binormal_	: BINORMAL;
};

// �W�I���g���V�F�[�_�[���͗p
struct GS_DEPTH_INPUT
{
	float4 position_ : SV_Position;
};

// clang-format on