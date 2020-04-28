//===========================================================================
//!	@file	constant_buffer.h
//!	@brief	定数バッファとか
//===========================================================================

// PI
static const float PI = 3.141592;

//	マテリアル
struct Material
{
	float3 diffuse_;
	float3 specular_;
};

// clang-format off
//---------------------------------------------------------------------------
//	モデル用テクスチャ
//---------------------------------------------------------------------------
Texture2D  BaseTexture			: register(t0);		//!< ベースにするテクスチャ(Albedo)
Texture2D  RoughnessTexture		: register(t1);		//!< ラフネス
Texture2D  MetalnessTexture		: register(t2);		//!< メタルネス
Texture2D  NormalTexture		: register(t3);		//!< 法線マップ
Texture2D  AoTexture			: register(t4);		//!< Ao
Texture2D  SubTexture			: register(t5);		//!< つかってないかも,
TextureCube iblDiffuseTexture	: register(t6);		//!< IBL用ディフューズ
TextureCube iblSpecularTexture	: register(t7);		//!< IBL用スペキュラー
Texture2D   DepthTexture		: register(t10);	//!< 深度テクスチャ
Texture2D   ToonTexture			: register(t11);	//!< Toon用
Texture2D   ThinTexture			: register(t12);	//!< Thin用(薄膜)
TextureCube cubeTexture			: register(t13);	//!< IBL用に作ったキューブテクスチャ
TextureCube cubeDepthTexture	: register(t14);	//!< IBL用に作った深度キューブテクスチャ
Texture2D   HeightTexture		: register(t15);	//!< 高さマップ

//---------------------------------------------------------------------------
// 水用
//---------------------------------------------------------------------------
Texture2D NormalTexture1 : register(t20);	//!< 水用法線マップ1
Texture2D NormalTexture2 : register(t21);	//!< 水用法線マップ2
Texture2D HeightTexture1 : register(t22);	//!< 水用高さマップ1
Texture2D HeightTexture2 : register(t23);	//!< 水用高さマップ2

//---------------------------------------------------------------------------
// サンプラー
//---------------------------------------------------------------------------
SamplerState LinearSampler : register(s0);	//!< 通常サンプラー
SamplerState DepthSampler  : register(s1);	//!< デプス用サンプラー(使ってない)
SamplerState ClampSampler  : register(s2);	//!< クランプ用サンプラー

//---------------------------------------------------------------------------
//	定数バッファ(ディふぁーどでいらなくなる?)
//---------------------------------------------------------------------------
static const int LIGHT_MAX_COUNT  = 16;	//!< ライト最大個数
static const int MODEL_MAX_COUNT  = 8;	//!< モデル最大個数
static const int CAMERA_MAX_COUNT = 8;	//!< カメラ最大個数

//  シーン単位で更新される定数バッファ
cbuffer cbCamera : register(b0)
{
	matrix matView_[CAMERA_MAX_COUNT];          //!< ビュー行列
	matrix matProj_[CAMERA_MAX_COUNT];          //!< 投影行列
	float3 cameraPosition_[CAMERA_MAX_COUNT];   //!< カメラのワールド座標
};

//  モデル単位で更新される定数バッファ
cbuffer cbModel : register(b1)
{
	matrix matWorld_;   //!< ワールド行列
};

// 平行光源
cbuffer cbDirectionalLight : register(b4)
{
	// dl = directionalLight
	float4 dlPosition_;		//!< ライトの位置(鏡面反射用)
	float4 dlColor_;		//!< ライトのカラー
	float3 dlLookAt_;		//!< ライトの向き
	float  dlIntensity_;	//!< ライトの強度(明るさ)
	bool   dlIsActive_;		//!< 状態
}

// 点光源
cbuffer cbPointLight : register(b5)
{
	// pl = pointLight
	int plCount_;   //!< 送る点光源の最大数

	float4 plIntensity_[LIGHT_MAX_COUNT];     //!< ライトの強度(明るさ)
	float4 plPosition_[LIGHT_MAX_COUNT];      //!< ライトの位置
	float4 plColor_[LIGHT_MAX_COUNT];         //!< ライトのカラー
	float4 plAttenuation_[LIGHT_MAX_COUNT];   //!< ライトの減衰パラメーター(0.0, 0.0, 0.3)
}

// スポットライト
cbuffer cbSpotLight : register(b6)
{
	// sl = spotLight
	int slCount_;   //!< 送る光源の最大数

	float4 slIntensity_[LIGHT_MAX_COUNT];     //!< ライトの強度(明るさ)
	float4 slPosition_[LIGHT_MAX_COUNT];      //!< ライトの位置
	float4 slLookAt_[LIGHT_MAX_COUNT];        //!< ライトの注視点
	float4 slColor_[LIGHT_MAX_COUNT];         //!< ライトのカラー
	float4 slAttenuation_[LIGHT_MAX_COUNT];   //!< ライトの減衰パラメーター(0.0, 0.0, 0.3)
}

// フィルタ用
cbuffer cbFilter : register(b7)
{
	float  stride_;
	float  attenuation_;	//!< 減衰度
	float2 dir_;			//!< グレアの方向
}

//　影 & SSS
cbuffer cbShadow : register(b8)
{
	matrix matLightView_;	//!< ライトビュー行列
	matrix matLightProj_;	//!< ライトプロジェクション行列
}

// 波用
cbuffer cbWave : register(b10)
{
	float waveDistance_;	//!< 1フレームで進ませる距離
	float waveFrequency_;	//!< 細かさ
	float waveSharp_;		//!< 尖り度
	float waveHeight_;		//!< 高さ
};

//--------------------------------------------------------------
//	シェーダーへの入力構造体
//--------------------------------------------------------------

// 頂点シェーダー
struct VS_INPUT
{
	float4 position_	: POSITION;   // 座標変換前の位置座標
	float3 normal_		: NORMAL;
	float3 tangent_		: TANGENT;
	float3 binormal_	: BINORMAL;
	float4 color_		: COLOR;
	float2 uv_			: TEXCOORD0;   // テクスチャUV座標
};

// ピクセルシェーダー
struct PS_INPUT
{
	float4 position_		: SV_Position;   // 頂点シェーダーからの出力はSV_Positionに出力すること
	float3 normal_			: NORMAL;
	float4 color_			: COLOR;
	float3 tangent_			: TANGENT;
	float3 binormal_		: BINORMAL;
	float2 uv_				: TEXCOORD0;   // テクスチャUV座標
	float3 worldPosition_	: TEXCOORD1;
};

// 頂点シェーダー(2D)
struct VS_INPUT_2D
{
	float2 position_	: POSITION;
	float2 uv_			: TEXCOORD0;
};

// ピクセルシェーダー(2D)
struct PS_INPUT_2D
{
	float4 position_	: SV_Position;   // 頂点シェーダーからの出力はSV_Positionに出力すること
	float2 uv_			: TEXCOORD0;   // テクスチャUV座標
};

// シャドウ
struct PS_INPUT_SHADOW
{
	float4 position_		: SV_Position;   // 頂点シェーダーからの出力はSV_Positionに出力すること
	float3 normal_			: NORMAL;
	float3 tangent_			: TANGENT;
	float3 binormal_		: BINORMAL;
	float4 color_			: COLOR;
	float2 uv_				: TEXCOORD0;   // テクスチャUV座標
	float3 worldPosition_	: TEXCOORD1;
	float4 shadowMapUv_		: TEXCOORD2;
};

// 頂点シェーダー
struct VS_INPUT_BUMP
{
	float4 position_	: POSITION;   // 座標変換前の位置座標
	float3 normal_		: NORMAL;       // 
	float4 color_		: COLOR;
	float2 uv_			: TEXCOORD0;   // テクスチャUV座標
	float4 tangent		: TEXCOORD1;
};

// ハルシェーダーパッチ定数関数(四角)
struct HS_CONSTANT_QUAD_INPUT
{
	float factor[4]			: SV_TessFactor;
	float inner_factor[2]	: SV_InsideTessFactor;
};

// ハルシェーダーパッチ定数関数(三角)
struct HS_CONSTANT_TRI_INPUT
{
	float factor[3]		: SV_TessFactor;
	float inner_factor	: SV_InsideTessFactor;
};

// ハルシェーダー入力用
struct HS_INPUT
{
	float4 position_	: POSITION;
	float2 uv_			: TEXCOORD0;
	float3 normal_		: NORMAL;
	float3 tangent_		: TANGENT;
	float3 binormal_	: BINORMAL;
};

// ドメインシェーダー入力用
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

// ジオメトリシェーダー入力用
struct GS_DEPTH_INPUT
{
	float4 position_ : SV_Position;
};

// clang-format on