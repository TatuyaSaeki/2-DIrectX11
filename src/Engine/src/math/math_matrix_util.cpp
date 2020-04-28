//===========================================================================
//! @file	math_matrix_util.cpp
//! @brief	行列のユーティリティ
//===========================================================================

//---------------------------------------------------------------------------
//! 与えられた行列をスクリーン座標にする
//---------------------------------------------------------------------------
Matrix matrixConvert2D(const Matrix& matWorld, f32 width, f32 height)
{
    Matrix matWorld0   = matWorld;
    Matrix matScreen   = Matrix::identity();
    matScreen.m_[0][0] = width * 0.5f;
    matScreen.m_[0][1] = 0.0f;
    matScreen.m_[0][2] = 0.0f;
    matScreen.m_[0][3] = 0.0f;

    matScreen.m_[1][0] = 0.0f;
    matScreen.m_[1][1] = -height * 0.5f;
    matScreen.m_[1][2] = 0.0f;
    matScreen.m_[1][3] = 0.0f;

    matScreen.m_[2][0] = 0.0f;
    matScreen.m_[2][1] = 0.0f;
    matScreen.m_[2][2] = 1.0f;
    matScreen.m_[2][3] = 0.0f;

    matScreen.m_[3][0] = width * 0.5f;
    matScreen.m_[3][1] = height * 0.5f;
    matScreen.m_[3][2] = 0.0f;
    matScreen.m_[3][3] = 1.0f;

	return Matrix::identity();
}