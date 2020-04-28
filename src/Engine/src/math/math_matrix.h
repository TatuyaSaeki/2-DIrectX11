//===========================================================================
//!	@file	math_matrix.h
//!	@brief	行列
//===========================================================================
#pragma once

//===========================================================================
//!	@struct Matrix
//===========================================================================
struct Matrix
{
    // デフォルトコンストラクタ
    Matrix() = default;

    //! コンストラクタ
    Matrix(f32 m11, f32 m12, f32 m13, f32 m14,
           f32 m21, f32 m22, f32 m23, f32 m24,
           f32 m31, f32 m32, f32 m33, f32 m34,
           f32 m41, f32 m42, f32 m43, f32 m44)
    {
        m_[0][0] = m11;
        m_[0][1] = m12;
        m_[0][2] = m13;
        m_[0][3] = m14;

        m_[1][0] = m21;
        m_[1][1] = m22;
        m_[1][2] = m23;
        m_[1][3] = m24;

        m_[2][0] = m31;
        m_[2][1] = m32;
        m_[2][2] = m33;
        m_[2][3] = m34;

        m_[3][0] = m41;
        m_[3][1] = m42;
        m_[3][2] = m43;
        m_[3][3] = m44;
    }

    Matrix(const DirectX::XMMATRIX& m)
    {
        memcpy(m_, &m, sizeof(Matrix));
    }

    // 行列 x 行列
    Matrix operator*(const Matrix& other)
    {
        Matrix result;

        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                result.m_[i][j] = (m_[i][0] * other.m_[0][j]) +
                                  (m_[i][1] * other.m_[1][j]) +
                                  (m_[i][2] * other.m_[2][j]) +
                                  (m_[i][3] * other.m_[3][j]);
            }
        }

        return result;
    }

    // 逆行列 (直交座標系の場合のみの高速逆行列)
    Matrix inverseFast() const
    {
        Matrix result;

        result.m_[0][0] = m_[0][0];
        result.m_[0][1] = m_[1][0];
        result.m_[0][2] = m_[2][0];
        result.m_[0][3] = 0.0f;

        result.m_[1][0] = m_[0][1];
        result.m_[1][1] = m_[1][1];
        result.m_[1][2] = m_[2][1];
        result.m_[1][3] = 0.0f;

        result.m_[2][0] = m_[0][2];
        result.m_[2][1] = m_[1][2];
        result.m_[2][2] = m_[2][2];
        result.m_[2][3] = 0.0f;

        f32 tx = Vector3::dot(translate(), axisX()) * -1.0f;
        f32 ty = Vector3::dot(translate(), axisY()) * -1.0f;
        f32 tz = Vector3::dot(translate(), axisZ()) * -1.0f;

        result.m_[3][0] = tx;
        result.m_[3][1] = ty;
        result.m_[3][2] = tz;
        result.m_[3][3] = 1.0f;

        return result;
    }

    static Matrix lookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f))
    {
        //----------------------------------------------------------------------------------
        // カメラの姿勢行列(ワールド行列)を求める
        //----------------------------------------------------------------------------------

        // 視線方向からZ軸を計算
        Vector3 axisZ = (position - lookAt).normalize();

        // 基準軸（worldUp）でX軸を計算
        Vector3 axisX = Vector3::cross(worldUp, axisZ).normalize();

        // Y軸を計算
        Vector3 axisY = Vector3::cross(axisZ, axisX);

        Matrix result;
        result.m_[0][0] = axisX.x_;
        result.m_[0][1] = axisY.x_;
        result.m_[0][2] = axisZ.x_;
        result.m_[0][3] = 0.0f;

        result.m_[1][0] = axisX.y_;
        result.m_[1][1] = axisY.y_;
        result.m_[1][2] = axisZ.y_;
        result.m_[1][3] = 0.0f;

        result.m_[2][0] = axisX.z_;
        result.m_[2][1] = axisY.z_;
        result.m_[2][2] = axisZ.z_;
        result.m_[2][3] = 0.0f;

        result.m_[3][0] = Vector3::dot(position, axisX) * -1.0f;
        result.m_[3][1] = Vector3::dot(position, axisY) * -1.0f;
        result.m_[3][2] = Vector3::dot(position, axisZ) * -1.0f;
        result.m_[3][3] = 1.0f;

        return result;
    }

    // 単位行列
    static Matrix identity()
    {
        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 平行移動
    static Matrix translate(const Vector3& v)
    {
        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            v.x_, v.y_, v.z_, 1.0f);
    }

    // 拡大縮小
    static Matrix scale(f32 s)
    {
		return scale(Vector3(s, s, s));
    }

    // 拡大縮小
    static Matrix scale(const Vector3& s)
    {
        return Matrix(
            s.x_, 0.0f, 0.0f, 0.0f,
            0.0f, s.y_, 0.0f, 0.0f,
            0.0f, 0.0f, s.z_, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // X軸中心の回転
    static Matrix rotateX(f32 radian)
    {
        f32 s = sinf(radian);
        f32 c = cosf(radian);

        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Y軸中心の回転
    static Matrix rotateY(f32 radian)
    {
        f32 s = sinf(radian);
        f32 c = cosf(radian);

        return Matrix(
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Z軸中心の回転
    static Matrix rotateZ(f32 radian)
    {
        f32 s = sinf(radian);
        f32 c = cosf(radian);

        return Matrix(
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 任意軸中心の回転
    static Matrix rotateAxis(
        const Vector3& v,   // 中心軸
        f32          radian)       // 回転角
    {
        f32 s = sinf(radian);
        f32 c = cosf(radian);

        Vector3 axis = v.normalize();
        f32   x    = axis.x_;
        f32   y    = axis.y_;
        f32   z    = axis.z_;

        Matrix matrix;
        matrix.m_[0][0] = x * x * (1.0f - c) + c;
        matrix.m_[0][1] = x * y * (1.0f - c) + z * s;
        matrix.m_[0][2] = x * z * (1.0f - c) - y * s;
        matrix.m_[0][3] = 0.0f;

        matrix.m_[1][0] = x * y * (1.0f - c) - z * s;
        matrix.m_[1][1] = y * y * (1.0f - c) + c;
        matrix.m_[1][2] = y * z * (1.0f - c) + x * s;
        matrix.m_[1][3] = 0.0f;

        matrix.m_[2][0] = x * z * (1.0f - c) + y * s;
        matrix.m_[2][1] = y * z * (1.0f - c) - x * s;
        matrix.m_[2][2] = z * z * (1.0f - c) + c;
        matrix.m_[2][3] = 0.0f;

        matrix.m_[3][0] = 0.0f;
        matrix.m_[3][1] = 0.0f;
        matrix.m_[3][2] = 0.0f;
        matrix.m_[3][3] = 1.0f;

        return matrix;
    }

    const Vector3 axisX() const
    {
        return Vector3(m_[0][0], m_[0][1], m_[0][2]);
    }

    const Vector3 axisY() const
    {
        return Vector3(m_[1][0], m_[1][1], m_[1][2]);
    }

    const Vector3 axisZ() const
    {
        return Vector3(m_[2][0], m_[2][1], m_[2][2]);
    }

    const Vector3 translate() const
    {
        return Vector3(m_[3][0], m_[3][1], m_[3][2]);
    }

    static Matrix transpose(const Matrix& m)
    {
        return { m.m_[0][0], m.m_[1][0], m.m_[2][0], m.m_[3][0],
                 m.m_[0][1], m.m_[1][1], m.m_[2][1], m.m_[3][1],
                 m.m_[0][2], m.m_[1][2], m.m_[2][2], m.m_[3][2],
                 m.m_[0][3], m.m_[1][3], m.m_[2][3], m.m_[3][3] };
    }

    f32 m_[4][4];
};