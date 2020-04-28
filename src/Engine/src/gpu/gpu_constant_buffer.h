//===========================================================================
//!	@file	gpu_constant_buffer.h
//!	@brief	GPU�萔�o�b�t�@�[
//===========================================================================
#pragma once

static const u32 LIGHT_MAX_COUNT  = 16;   //!< ���C�g�̍ő吔
static const u32 MODEL_MAX_COUNT  = 8;    //!< ���f���̍ő吔
static const u32 CAMERA_MAX_COUNT = 8;    //!< ���C�g�̍ő吔

// �V�[���P�ʂōX�V����萔�o�b�t�@
struct cbCamera
{
    Matrix  matView_[CAMERA_MAX_COUNT];          //!< �r���[�s��
    Matrix  matProj_[CAMERA_MAX_COUNT];          //!< ���e�s��
    Vector3 cameraPosition_[CAMERA_MAX_COUNT];   //!< �J�����̃��[���h���W
    u32     cameraCount_;
};

// ���f���P�ʂōX�V����萔�o�b�t�@
struct cbModel
{
    Matrix matWorld_;   //!< ���[���h�s��
};

// ���f���P�ʂōX�V����萔�o�b�t�@
struct cbAnimation
{
    Matrix matrix_[MODEL_MAX_COUNT];   //!< ���[���h�s��
};

struct cbDirectionalLight
{
    Vector4 position_;
    Vector4 color_;       //!< ���C�g�̃J���[
    Vector3 lookAt_;      //!< ���C�g�̒����_
    f32     intensity_;   //!< ���C�g�̋��x(���邳)
    bool    isActive_;
};

struct cbPointLight
{
    u32 lightCount_;   //!< ����_�����̍ő吔

    int padding1;
    int padding2;
    int padding3;

    Vector4 intensity_[LIGHT_MAX_COUNT];     //!< ���C�g�̋��x(���邳)
    Vector4 position_[LIGHT_MAX_COUNT];      //!< ���C�g�̈ʒu
    Vector4 color_[LIGHT_MAX_COUNT];         //!< ���C�g�̃J���[
    Vector4 attenuation_[LIGHT_MAX_COUNT];   //!< ���C�g�̌����p�����[�^�[(0.0, 0.0, 0.3)
};

struct cbSpotLight
{
    u32 lightCount_;   //!< ����_�����̍ő吔

    int padding1;
    int padding2;
    int padding3;

    Vector4 intensity_[LIGHT_MAX_COUNT];     //!< ���C�g�̋��x(���邳)
    Vector4 position_[LIGHT_MAX_COUNT];      //!< ���C�g�̈ʒu
    Vector4 lookAt_[LIGHT_MAX_COUNT];        //!< ���C�g�̌���
    Vector4 color_[LIGHT_MAX_COUNT];         //!< ���C�g�̃J���[
    Vector4 attenuation_[LIGHT_MAX_COUNT];   //!< ���C�g�̌����p�����[�^�[(0.0, 0.0, 0.3)
};

struct cbSSS
{
    Matrix matTex_;
    //Matrix matLight_;
    f32 transparent_;
};

struct cbShadow
{
    Matrix matLightView_;
    Matrix matLightProj_;
};

struct cbToon   // ����
{
    bool edge_;
};

//
struct cbSeaWave
{
    f32 moveDistance_ = 0.0f;   //!< �i�ރX�s�[�h
    f32 fineness_     = 0.1f;   //!< �ׂ���
    f32 sharp_        = 0.5f;   //!< ���x
    f32 height_       = 0.1f;   //!< ����
};

struct CBTexture2D
{
    Matrix  m;
    Vector2 viewProtSize_ = Vector2(1280.0f, 720.0f);
};
