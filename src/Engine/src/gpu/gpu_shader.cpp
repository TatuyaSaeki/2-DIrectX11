//===========================================================================
//!	@file	gpu_shader.cpp
//!	@brief	シェーダー
//===========================================================================

namespace gpu::detail {

// 仮
static constexpr const char* g_filePath = "../../Application/asset/shader/";

//! カスタムインクルードクラス
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
//!	HLSLシェーダーをコンパイル
//------------------------------------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const char* fileName, const char* functionName, const char* shaderModel)
{
    const std::string addPath  = detail::g_filePath;
    const std::string filePath = addPath + fileName;
	//---------------------------------------------------------------------------
    //	HLSLシェーダーソースコードをメモリに読み込み
	//---------------------------------------------------------------------------
    std::vector<char> shaderSource;

    FILE* fp;
    if(fopen_s(&fp, filePath.c_str(), "rb") != 0) {
        MessageBox(NULL, "シェーダーの読み込みに失敗", nullptr, MB_OK);
        return nullptr;
    }

    //---- ファイルサイズを取得
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // ファイルサイズ
    fseek(fp, 0, SEEK_SET);

    //---- ワーク領域を確保して読み込み
    shaderSource.resize(fileSize);   // ワーク領域の確保
    if(shaderSource.empty()) {
        fclose(fp);
        return false;
    }
    fread(&shaderSource[0], fileSize, 1, fp);   // 読み込み
    fclose(fp);

    //----------------------------------------------------------------------------------
    // シェーダーコンパイラーのオプション設定
    //----------------------------------------------------------------------------------
    u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

    //---- 行列の列優先・行優先
    flags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;   // 列優先(転置)
                                                    //	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// 行優先

    //---- 分岐コードの生成制御(if, for文など)
    //	flags |= D3DCOMPILE_AVOID_FLOW_CONTROL;			// 分岐しないフラットなコードを生成
    flags |= D3DCOMPILE_PREFER_FLOW_CONTROL;   // 動的分岐を含むコードを生成

    //---- 最適化レべル（シェーダー性能とコンパイル時間に影響）
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;		// 最適化なし
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;		// ↑
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;		// ↓
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // 最大限の最適化

#if 0
	//---- シェーダーにデバッグ情報を付加
	// ※注意	Release版には含めないこと
		flags |= D3DCOMPILE_DEBUG;
#endif
    //----------------------------------------------------------------------------------
    // HLSLソースコード内のdefineマクロ定義
    //----------------------------------------------------------------------------------
    std::array<D3D_SHADER_MACRO, 2> defineMacros = { { { "__WINDOWS__", "1" },
                                                       //---- 終端にnullptr指定しておく
                                                       { nullptr, nullptr } } };

    //----------------------------------------------------------------------------------
    // HLSLシェーダーをコンパイル
    //----------------------------------------------------------------------------------
    HRESULT            hr;
    ID3DBlob*          shaderBlob;   // シェーダーバイナリ
    detail::D3DInclude myInclude;    // 使用しない場合は D3D_COMPILE_STANDARD_FILE_INCLUDE を設定すること
    com_ptr<ID3DBlob>  errorBlob;    // エラーメッセージ出力用

    hr = D3DCompile(&shaderSource[0],      // [in]		シェーダーソースコード
                    shaderSource.size(),   // [in]		シェーダーソースコードのサイズ
                    functionName,          // [in]		シェーダー名（エラーメッセージ内に出力される名前。任意）
                    &defineMacros[0],      // [in]		外部から与えるプリプロセッサマクロ#define
                    &myInclude,            // [in]		インクルード#includeのカスタムハンドラ
                    functionName,          // [in]		実行開始点の関数名
                    shaderModel,           // [in]		シェーダーモデル名 ("vs_4_0", "ps_4_0", "cs_5_0" etc...)
                    flags,                 // [in]		オプションフラグ
                    0,                     // [in]		エフェクトオプションフラグ(常に0)
                    &shaderBlob,           // [out]	コンパイルされたシェーダーバイナリ
                    &errorBlob);           // [out]	コンパイルエラーメッセージ

    //----------------------------------------------------------------------------------
    // コンパイルエラーの場合はエラーメッセージの内容を出力
    //----------------------------------------------------------------------------------
    if(errorBlob) {
        // VisualStudio 「出力」ウィンドウに表示
        OutputDebugString((const char*)errorBlob->GetBufferPointer());

        // ポップアップでも表示
        MessageBox(nullptr, (const char*)errorBlob->GetBufferPointer(), filePath.c_str(), MB_OK);
    }

    return shaderBlob;
}

//---------------------------------------------------------------------------
//! ShaderBase
//! シェーダーバイナリのアドレスを取得
//---------------------------------------------------------------------------
const void* ShaderBase::getShaderBinary() const
{
    return shaderBinary_->GetBufferPointer();
}

//---------------------------------------------------------------------------
//! ShaderBase
//! シェーダーバイナリのサイズを取得
//---------------------------------------------------------------------------
size_t ShaderBase::getShaderBinarySize() const
{
    return shaderBinary_->GetBufferSize();
}

//---------------------------------------------------------------------------
//! ShaderVs
//! コンストラクタ
//---------------------------------------------------------------------------
ShaderVs::ShaderVs(ID3DBlob* shaderBinary)
{
    // シェーダーの作成
    device::D3DDevice()->CreateVertexShader(shaderBinary->GetBufferPointer(),
                                            shaderBinary->GetBufferSize(),
                                            nullptr,
                                            &shader_);
    shaderBinary_ = shaderBinary;
}

//---------------------------------------------------------------------------
//! ShaderVs
//! D3Dシェーダーを取得
//---------------------------------------------------------------------------
ID3D11VertexShader* ShaderVs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderVs
//! シェーダーの作成
//---------------------------------------------------------------------------
gpu::ShaderVs* ShaderVs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "vs_5_0");   // 頂点シェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderVs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderPs
//! コンストラクタ
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
//! D3Dシェーダーを取得
//---------------------------------------------------------------------------
ID3D11PixelShader* ShaderPs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderPs
//! シェーダーの作成
//---------------------------------------------------------------------------
gpu::ShaderPs* ShaderPs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "ps_5_0");   // ピクセルシェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderPs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderGs
//! コンストラクタ
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
//! D3Dシェーダーを取得
//---------------------------------------------------------------------------
ID3D11GeometryShader* ShaderGs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderGs
//! シェーダー作成
//---------------------------------------------------------------------------
gpu::ShaderGs* ShaderGs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "gs_5_0");   // ピクセルシェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderGs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderHs
//! コンストラクタ
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
//! D3Dシェーダーを取得
//---------------------------------------------------------------------------
ID3D11HullShader* ShaderHs::get() const
{
	return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderHs
//! シェーダーの作成
//---------------------------------------------------------------------------
gpu::ShaderHs* ShaderHs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "hs_5_0");   // ピクセルシェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderHs(shaderBinary);
}

//---------------------------------------------------------------------------
//! ShaderDs
//! コンストラクタ
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
//! D3Dシェーダーを取得
//---------------------------------------------------------------------------
ID3D11DomainShader* ShaderDs::get() const
{
    return shader_.Get();
}

//---------------------------------------------------------------------------
//! ShaderDs
//! シェーダー作成
//---------------------------------------------------------------------------
gpu::ShaderDs* ShaderDs::create(const std::string& fileName)
{
    auto shaderBinary = compileShaderFromFile((fileName + ".fx").c_str(), "main", "ds_5_0");   // ピクセルシェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    return new gpu::ShaderDs(shaderBinary);
}

}   // namespace gpu
