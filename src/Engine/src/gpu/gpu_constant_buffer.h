//===========================================================================
//!	@file	gpu_constant_buffer.h
//!	@brief	GPU定数バッファー
//===========================================================================
#pragma once

static const u32 LIGHT_MAX_COUNT  = 16;   //!< ライトの最大数
static const u32 MODEL_MAX_COUNT  = 8;    //!< モデルの最大数
static const u32 CAMERA_MAX_COUNT = 8;    //!< ライトの最大数

// シーン単位で更新する定数バッファ
struct cbCamera
{
    Matrix  matView_[CAMERA_MAX_COUNT];          //!< ビュー行列
    Matrix  matProj_[CAMERA_MAX_COUNT];          //!< 投影行列
    Vector3 cameraPosition_[CAMERA_MAX_COUNT];   //!< カメラのワールド座標
    u32     cameraCount_;
};

// モデル単位で更新する定数バッファ
struct cbModel
{
    Matrix matWorld_;   //!< ワールド行列
};

// モデル単位で更新する定数バッファ
struct cbAnimation
{
    Matrix matrix_[MODEL_MAX_COUNT];   //!< ワールド行列
};

struct cbDirectionalLight
{
    Vector4 position_;
    Vector4 color_;       //!< ライトのカラー
    Vector3 lookAt_;      //!< ライトの注視点
    f32     intensity_;   //!< ライトの強度(明るさ)
    bool    isActive_;
};

struct cbPointLight
{
    u32 lightCount_;   //!< 送る点光源の最大数

    int padding1;
    int padding2;
    int padding3;

    Vector4 intensity_[LIGHT_MAX_COUNT];     //!< ライトの強度(明るさ)
    Vector4 position_[LIGHT_MAX_COUNT];      //!< ライトの位置
    Vector4 color_[LIGHT_MAX_COUNT];         //!< ライトのカラー
    Vector4 attenuation_[LIGHT_MAX_COUNT];   //!< ライトの減衰パラメーター(0.0, 0.0, 0.3)
};

struct cbSpotLight
{
    u32 lightCount_;   //!< 送る点光源の最大数

    int padding1;
    int padding2;
    int padding3;

    Vector4 intensity_[LIGHT_MAX_COUNT];     //!< ライトの強度(明るさ)
    Vector4 position_[LIGHT_MAX_COUNT];      //!< ライトの位置
    Vector4 lookAt_[LIGHT_MAX_COUNT];        //!< ライトの向き
    Vector4 color_[LIGHT_MAX_COUNT];         //!< ライトのカラー
    Vector4 attenuation_[LIGHT_MAX_COUNT];   //!< ライトの減衰パラメーター(0.0, 0.0, 0.3)
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

struct cbToon   // けす
{
    bool edge_;
};

//
struct cbSeaWave
{
    f32 moveDistance_ = 0.0f;   //!< 進むスピード
    f32 fineness_     = 0.1f;   //!< 細かさ
    f32 sharp_        = 0.5f;   //!< 尖り度
    f32 height_       = 0.1f;   //!< 高さ
};

struct CBTexture2D
{
    Matrix  m;
    Vector2 viewProtSize_ = Vector2(1280.0f, 720.0f);
};
