//===========================================================================
//!	@file	gpu_shader.cpp
//!	@brief	�V�F�[�_�[
//===========================================================================

namespace gpu::detail {

// ��
static constexpr const char* g_filePath = "../../Application/asset/shader/";

//! �J�X�^���C���N���[�h�N���X
class D3DInclude : public ID3DInclude
{
public:
    HRESULT __stdcall Open([[maybe_unused]] D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, [[maybe_unused]] LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
    {
        std::string path = g_filePath;
        path += pFileName;

        std::ifstream file(path);
        if(!file.is_open())
            return S_OK;

        _string = std::string{
            std::istreambuf_iterator<char>{ file },
            std::istreambuf_iterator<char>{}
        };

        *ppData = _string.c_str();
        *pBytes = static_cast<UINT>(_string.size());
        return S_OK;
    }

    HRESULT __stdcall Close([[maybe_unused]] LPCVOID pData) override
    {
        return S_OK;
    }

private:
    std::string _string;
};

}   // namespace gpu::detail

namespace gpu {

//------------------------------------------------------------------------------------------------
//!	HLSL�V�F�[�_�[���R���p�C��
//------------------------------------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const char* fileName, const char* functionName, const char* shaderModel)
{
    const std::string addPath  = detail::g_filePath;
    const std::string filePath = addPath + fileName;
	//---------------------------------------------------------------------------
    //	HLSL�V�F�[�_�[�\�[�X�R�[�h���������ɓǂݍ���
	//---------------------------------------------------------------------------
    std::vector<char> shaderSource;

    FILE* fp;
    if(fopen_s(&fp, filePath.c_str(), "rb") != 0) {
        MessageBox(NULL, "�V�F�[�_�[�̓ǂݍ��݂Ɏ��s", nullptr, MB_OK);
        return nullptr;
    }

    //---- �t�@�C���T�C�Y���擾
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // �t�@�C���T�C�Y
    fseek(fp, 0, SEEK_SET);

    //---- ���[�N�̈���m�ۂ��ēǂݍ���
    shaderSource.resize(fileSize);   // ���[�N�̈�̊m��
    if(shaderSource.empty()) {
        fclose(fp);
        return false;
    }
    fread(&shaderSource[0], fileSize, 1, fp);   // �ǂݍ���
    fclose(fp);

    //----------------------------------------------------------------------------------
    // �V�F�[�_�[�R���p�C���[�̃I�v�V�����ݒ�
    //----------------------------------------------------------------------------------
    u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

    //---- �s��̗�D��E�s�D��
    flags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;   // ��D��(�]�u)
                                                    //	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// �s�D��

    //---- ����R�[�h�̐�������(if, for���Ȃ�)
    //	flags |= D3DCOMPILE_AVOID_FLOW_CONTROL;			// ���򂵂Ȃ��t���b�g�ȃR�[�h�𐶐�
    flags |= D3DCOMPILE_PREFER_FLOW_CONTROL;   // ���I������܂ރR�[�h�𐶐�

    //---- �œK�����׃��i�V�F�[�_�[���\�ƃR���p�C�����Ԃɉe���j
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;		// �œK���Ȃ�
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;		// ��
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;		// ��
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // �ő���̍œK��

#if 0
	//---- �V�F�[�_�[�Ƀf�o�b�O����t��
	// ������	Release�łɂ͊܂߂Ȃ�����
		flags |= D3DCOMPILE_DEBUG;
#endif
    //----------------------------------------------------------------------------------
    // HLSL�\�[�X�R�[�h����define�}�N����`
    //----------------------------------------------------------------------------------
    std::array<D3D_SHADER_MACRO, 2> defineMacros = { { { "__WINDOWS__", "1" },
                                                       //---- �I�[��nullptr�w�肵�Ă���
                                                       { nullptr, nullptr } } };

    //----------------------------------------------------------------------------------
    // HLSL�V�F�[�_�[���R���p�C��
    //----------------------------------------------------------------------------------
    HRESULT            hr;
    ID3DBlob*          shaderBlob;   // �V�F�[�_�[�o�C�i��
    detail::D3DInclude myInclude;    // �g�p���Ȃ��ꍇ�� D3D_COMPILE_STANDARD_FILE_INCLUDE ��ݒ肷�邱��
    com_ptr<ID3DBlob>  errorBlob;    // �G���[���b�Z�[�W�o�͗p

    hr = D3DCompile(&shaderSource[0],      // [in]		�V�F�[�_�[�\�[�X�R�[�h
                    shaderSource.size(),   // [in]		�V�F�[�_�[�\�[�X�R�[�h�̃T�C�Y
                    functionName,          // [in]		�V�F�[�_�[���i�G���[���b�Z�[�W���ɏo�͂���閼�O�B�C�Ӂj
                    &defineMacros[0],      // [in]		�O������^����v���v���Z�b�T�}�N��#define
                    &myInclude,            // [in]		�C���N���[�h#include�̃J�X�^���n���h��
                    functionName,          // [in]		���s�J�n�_�̊֐���
                    shaderModel,           // [in]		�V�F�[�_�[���f���� ("vs_4_0", "ps_4_0", "cs_5_0" etc...)
                    flags,                 // [in]		�I�v�V�����t���O
                    0,                     // [in]		�G�t�F�N�g�I�v�V�����t���O(���0)
                    &shaderBlob,           // [out]	�R���p�C�����ꂽ�V�F�[�_�[�o�C�i��
                    &errorBlob);           // [out]	�R���p�C���G���[���b�Z�[�W

    //----------------------------------------------------------------------------------
    // �R���p�C���G���[�̏ꍇ�̓G���[���b�Z�[�W�̓��e���o��
    //----------------------------------------------------------------------------------
    if(errorBlob) {
        // VisualStudio �u�o�́v�E�B���h�E�ɕ\��
        OutputDebugString((const char*)errorBlob->GetBufferPointer());

        // �|�b�v�A�b�v�ł��\��
        MessageBox(nullptr, (const char*)errorBlob->GetBufferPointer(), filePath.c_str(), MB_OK);
    }

    return shaderBlob;
}

//---------------------------------------------------------------------------
//! ShaderBase
//! �V�F�[�_�[�o�C�i���̃A�h���X���擾
//---------------------------------------------------------------------------
const void* ShaderBase::getShaderBinary() const
{
    return shaderBinary_->GetBufferPointer();
}

//---------------------------------------------------------------------------
//! ShaderBase
//! �V�F�[�_�[�o�C�i���̃T�C�Y���擾
//---------------------------------------------------------------------------
size_t ShaderBase::getShaderBinarySize() const
{
    return shaderBinary_->GetBufferSize();
}

//---------------------------------------------------------------------------
//! ShaderVs
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderVs::ShaderVs(ID3DBlob* shaderBinary)
{
    // �V�F�[�_�[�̍쐬
    device::D3DDevice()->CreateVertexShader(shaderBinary->GetBufferPointer(),
                                            shaderBinary->GetBufferSize(),
                                            nullptr,
                                            &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderVs
//! D3D�V�F�[�_�[���擾
//---------------------------------------------------------------------------
ID3D11VertexShader* ShaderVs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderVs
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
gpu::ShaderVs* ShaderVs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "vs_5_0");   // ���_�V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderVs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderPs
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderPs::ShaderPs(ID3DBlob* shaderBinary)
{
    device::D3DDevice()->CreatePixelShader(shaderBinary->GetBufferPointer(),
                                           shaderBinary->GetBufferSize(),
                                           nullptr,
                                           &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderPs
//! D3D�V�F�[�_�[���擾
//---------------------------------------------------------------------------
ID3D11PixelShader* ShaderPs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderPs
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
gpu::ShaderPs* ShaderPs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "ps_5_0");   // �s�N�Z���V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderPs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderGs
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderGs::ShaderGs(ID3DBlob* shaderBinary)
{
    device::D3DDevice()->CreateGeometryShader(shaderBinary->GetBufferPointer(),
                                              shaderBinary->GetBufferSize(),
                                              nullptr,
                                              &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderGs
//! D3D�V�F�[�_�[���擾
//---------------------------------------------------------------------------
ID3D11GeometryShader* ShaderGs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderGs
//! �V�F�[�_�[�쐬
//---------------------------------------------------------------------------
gpu::ShaderGs* ShaderGs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "gs_5_0");   // �s�N�Z���V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderGs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderHs
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderHs::ShaderHs(ID3DBlob* shaderBinary)
{
    device::D3DDevice()->CreateHullShader(shaderBinary->GetBufferPointer(),
                                          shaderBinary->GetBufferSize(),
                                          nullptr,
                                          &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderDs
//! D3D�V�F�[�_�[���擾
//---------------------------------------------------------------------------
ID3D11HullShader* ShaderHs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderHs
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
gpu::ShaderHs* ShaderHs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "hs_5_0");   // �s�N�Z���V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderHs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderDs
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderDs::ShaderDs(ID3DBlob* shaderBinary)
{
    device::D3DDevice()->CreateDomainShader(shaderBinary->GetBufferPointer(),
                                            shaderBinary->GetBufferSize(),
                                            nullptr,
                                            &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderDs
//! D3D�V�F�[�_�[���擾
//---------------------------------------------------------------------------
ID3D11DomainShader* ShaderDs::get() const
{
    return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderDs
//! �V�F�[�_�[�쐬
//---------------------------------------------------------------------------
gpu::ShaderDs* ShaderDs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "ds_5_0");   // �s�N�Z���V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderDs(shaderBinary);
}

}   // namespace gpu
