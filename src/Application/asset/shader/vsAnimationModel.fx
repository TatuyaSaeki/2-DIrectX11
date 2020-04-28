//===========================================================================
//!	@file	vsAnimationModel.fx
//!	@brief	アニメーション用(未完成)
//===========================================================================
#include "../shader/header/shader.h"

//===========================================================================
//!	頂点シェーダー(モデルアニメーション)
//===========================================================================
PS_INPUT main(VS_ANIMATION_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;

	matrix boneMatrix = (matrix)0;

	for (int i = 0; i < 4; i++) {
		boneMatrix += boneMatrixs[input.boneIndex_[i]] * input.boneWeight_[i];
	}

	float4	position = input.position_;

	position = mul(boneMatrix, position);

	position = mul(matWorld_, position);
	position = mul(matView_[0], position);
	position = mul(matProj_[0], position);

	//=============================================================
	// 出力
	//=============================================================
	output.position_ = position;	// 座標は投影後のxyzw座標値 (GPUによって頂点シェーダー実行後に自動でxyz÷wが実行される)
	output.color_ = input.color_;
	output.uv_ = input.uv_;	// UVを加工しない場合はそのまま代入でスルーさせる

	return output;
}