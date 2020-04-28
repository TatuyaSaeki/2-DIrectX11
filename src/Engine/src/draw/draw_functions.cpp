//===========================================================================
//!	@file	draw_funcions.cpp
//!	@brief	�`��֐��܂Ƃ�
//===========================================================================

//---------------------------------------------------------------------------
//!	�O���E���h�`��
//---------------------------------------------------------------------------
void gridRender(f32 size)
{
    dxLoadMatrixf(Matrix::identity());

    //---------------------------------------------------------------------------
    // �O���b�h�`��
    //---------------------------------------------------------------------------
    dxBegin(PT_LINES);
    dxColor4ub(0, 50, 0, 255);

    // �c�����O���b�h
    for(f32 x = -size; x <= +size; x++) {
        dxVertex3f(x, +0.05f, -size);
        dxVertex3f(x, +0.05f, +size);
    }
    // �������O���b�h
    for(f32 z = -size; z <= +size; z++) {
        dxVertex3f(-size, +0.05f, z);
        dxVertex3f(+size, +0.05f, z);
    }
    dxEnd();

    Line3D line[3] = {
        // X
        { Vector3(0.0f, 0.0f, 0.0f), Vector3(64.0f, 0.0f, 0.0f) },
        // Y			 0
        { Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 64.0f, 0.0f) },
        // Z			 0
        { Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 64.0f) },
    };

    Color color[3] = {
        Color(255, 0, 0, 255),
        Color(0, 255, 0, 255),
        Color(0, 0, 255, 255),
    };

    dxBegin(PT_LINES);

    for(u32 i = 0; i < 3; i++) {
        dxColor4ub(color[i]);

        dxVertex3fv(line[i].getPosition(0));
        dxVertex3fv(line[i].getPosition(1));
    }

    dxEnd();
}

//---------------------------------------------------------------------------
//!	���̕`�� (Vector4)
//---------------------------------------------------------------------------
void drawSphere(const Vector3& position, f32 radius, const Vector4& color, u32 divCount)
{
    dxColor4f(color);
    drawSphereEx(position, radius, divCount);
}

//---------------------------------------------------------------------------
//!	���̕`�� (Color)
//---------------------------------------------------------------------------
void drawSphere(const Vector3& position, f32 radius, const Color& color, u32 divCount)
{
    dxColor4ub(color);
    drawSphereEx(position, radius, divCount);
}

//---------------------------------------------------------------------------
//!	���̕`��(�s��K���ς݂̏ꍇ�͂�����)  (Vector4)
//---------------------------------------------------------------------------
void drawSphere(f32 radius, const Vector4& color, u32 divCount)
{
    dxColor4f(color);
    drawSphereEx(radius, divCount);
}

//---------------------------------------------------------------------------
//!	���̕`��(�s��K���ς݂̏ꍇ�͂�����)	(Color)
//---------------------------------------------------------------------------
void drawSphere(f32 radius, const Color& color, u32 divCount)
{
    dxColor4ub(color);
    drawSphereEx(radius, divCount);
}

//---------------------------------------------------------------------------
//! ��̊֐��ŃJ���[�ݒ肵�Ă��炱���Ŗ{�̕`��
//---------------------------------------------------------------------------
void drawSphereEx(const Vector3& position, f32 radius, u32 divCount)
{
    f32 invDivCount = 1.0f / divCount;

    // XY����
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);

        Vector3 p = position + Vector3(cosf(angle), sinf(angle), 0.0f) * radius;

        dxVertex3fv(p);
    }
    dxEnd();

    // ZY����
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);

        Vector3 p = position + Vector3(0.0f, sinf(angle), cosf(angle)) * radius;

        dxVertex3fv(p);
    }
    dxEnd();

    // XZ����
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);

        Vector3 p = position + Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

        dxVertex3fv(p);
    }
    dxEnd();
}

//---------------------------------------------------------------------------
//! ��̊֐��ŃJ���[�ݒ肵�Ă��炱�����Ŗ{�̕`��(�s�����p�ς݂̏ꍇ������)
//---------------------------------------------------------------------------
void drawSphereEx(f32 radius, u32 divCount)
{
    f32 invDivCount = 1.0f / divCount;

    gpu::setShader("vsStandard", "psStandard");
    // XY����
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);   // ���������悭��炩��

        Vector3 p = Vector3(cosf(angle), sinf(angle), 0.0f) * radius;

        dxVertex3fv(p);
    }
    dxEnd();

    // ZY����
    gpu::setShader("vsStandard", "psStandard");
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);   // ���������悭��炩��

        Vector3 p = Vector3(0.0f, sinf(angle), cosf(angle)) * radius;

        dxVertex3fv(p);
    }
    dxEnd();

    // XZ����
    gpu::setShader("vsStandard", "psStandard");
    dxBegin(PT_LINES);
    for(u32 i = 0; i < divCount; ++i) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);   // ���������悭��炩��

        Vector3 p = Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

        dxVertex3fv(p);
    }
    dxEnd();
}

//---------------------------------------------------------------------------
//!	�|���S���ŋ��`��
//---------------------------------------------------------------------------
void drawSphare(u32 slices, u32 stacks)
{
    dxBegin(PT_TRIANGLES);

    for(u32 i = 0; i <= stacks; ++i) {
        f32 ph = 3.141593f * f32(i) / f32(stacks);
        f32 y  = cosf(ph);
        f32 r  = sinf(ph);

        for(u32 j = 0; j <= slices; ++j) {
            f32 th = 2.0f * 3.141593f * f32(j) / f32(slices);
            f32 x  = r * cosf(th);
            f32 z  = r * sinf(th);

            dxVertex3f(x, y, z);
        }
    }

    dxEnd();
}

//---------------------------------------------------------------------------
//! ���[�J�����W�Ń��C���`��
//---------------------------------------------------------------------------
void drawLocalLine(f32 scale)
{
    Line3D lines[] = {
        { Vector3::ZERO, Vector3(1.0f * scale, 0.0f, 0.0f) },
        { Vector3::ZERO, Vector3(0.0f, 1.0f * scale, 0.0f) },
        { Vector3::ZERO, Vector3(0.0f, 0.0f, 1.0f * scale) },
    };

    Color color[3] = {
        Color(255, 0, 0, 255),
        Color(0, 255, 0, 255),
        Color(0, 0, 255, 255),
    };

    dxBegin(PT_LINES);

    for(u32 i = 0; i < 3; i++) {
        dxColor4ub(color[i]);

        dxVertex3fv(lines[i].getPosition(0));
        dxVertex3fv(lines[i].getPosition(1));
    }

    dxEnd();
}

//---------------------------------------------------------------------------
//!	���[���h���W�Ń��C���`��
//---------------------------------------------------------------------------
void drawLine(const Vector3& startPoint, const Vector3& dir, f32 scale)
{
    auto endPoint = dir.normalize() * scale + startPoint;

    Line3D line = {
        startPoint,
        endPoint,
    };
    gpu::setShader("vsStandard", "psStandard");

    dxBegin(PT_LINES);
    dxColor4ub(Color(255, 0, 0));

    dxVertex3fv(line.getPosition(0));
    dxVertex3fv(line.getPosition(1));

    dxEnd();
}

//---------------------------------------------------------------------------
//! �~���`��
//---------------------------------------------------------------------------
void drawCone(f32 radius, f32 height, const Color& color, u32 divCount)
{
    dxBegin(PT_LINES);

    dxColor4ub(color);

    f32 invDivCount = 1.0f / divCount;

    for(u32 i = 0; i < divCount; i++) {
        f32 angle = ((f32)i * invDivCount) * (2.0f * PI);

        dxVertex3fv(Vector3(Vector3(cosf(angle), sinf(angle), 0.0f) * radius));
        dxVertex3f(0.0f, 0.0f, height);
    }
    dxEnd();
}
