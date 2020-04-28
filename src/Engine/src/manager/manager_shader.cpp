//===========================================================================
//! @file	shader_manager.cpp
//!	@brief	�V�F�[�_�[�Ǘ�
//===========================================================================

namespace {
std::unordered_map<std::string, std::unique_ptr<ShaderVs>>      shaderVs_;
std::unordered_map<std::string, std::unique_ptr<gpu::ShaderPs>> shaderPs_;
std::unordered_map<std::string, std::unique_ptr<gpu::ShaderGs>> shaderGs_;
std::unordered_map<std::string, std::unique_ptr<gpu::ShaderHs>> shaderHs_;
std::unordered_map<std::string, std::unique_ptr<gpu::ShaderDs>> shaderDs_;
}   // namespace

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
ShaderManager::~ShaderManager()
{
    shaderHs_.clear();
    shaderDs_.clear();
    shaderPs_.clear();
    shaderVs_.clear();
}

//---------------------------------------------------------------------------
//! ���̓��C�A�E�g�擾
//---------------------------------------------------------------------------
gpu::InputLayout* const ShaderManager::getInputLayout(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬
    if(!shaderVs_.count(fileName)) {
        getShaderVs(fileName);
    }
    return shaderVs_[fileName]->inputLayout_.get();
}

//---------------------------------------------------------------------------
//! ���_�V�F�[�_�[���擾
//---------------------------------------------------------------------------
gpu::ShaderVs* const ShaderManager::getShaderVs(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬
    if(!shaderVs_.count(fileName)) {
        // ���_�V�F�[�_�[�쐬
        gpu::ShaderVs* shaderVs = gpu::ShaderVs::create(fileName.c_str());
        if(!shaderVs) {
            return nullptr;
        }

        // ���̓��C�A�E�g�쐬
        auto inputLayout = createInputLayout(fileName);
        if(!inputLayout) {
            GM_ASSERT_MESSAGE(false, "���̓��C�A�E�g�𐶐��ł��܂���ł����B");
            GM_SAFE_DELETE(shaderVs);
            return nullptr;
        }
        if(!inputLayout->create(*shaderVs))
            return nullptr;

        //
        //    // �}�b�v�ɒǉ�
        ShaderVs* shader = new ShaderVs();
        if(!shader) {
            GM_ASSERT_MESSAGE(false, "���_�V�F�[�_�[�𐶐��ł��܂���ł����B");
            GM_SAFE_DELETE(shaderVs);
            GM_SAFE_DELETE(inputLayout);
            return nullptr;
        }

        shader->inputLayout_.reset(inputLayout);
        shader->shaderVs_.reset(shaderVs);
        shaderVs_.insert(std::make_pair(fileName, shader));
        shader = nullptr;
    }
    //}

    return shaderVs_[fileName]->shaderVs_.get();
}

//---------------------------------------------------------------------------
//! �s�N�Z���V�F�[�_�[���擾
//---------------------------------------------------------------------------
gpu::ShaderPs* const ShaderManager::getShaderPs(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬
    if(!shaderPs_.count(fileName)) {
        gpu::ShaderPs* shaderPs = gpu::ShaderPs::create(fileName.c_str());
        if(!shaderPs) {
            GM_ASSERT_MESSAGE(false, "�s�N�Z���V�F�[�_�[�𐶐��ł��܂���ł����B");
            return nullptr;
        }
        shaderPs_.insert(std::make_pair(fileName, shaderPs));
    }

    return shaderPs_[fileName].get();
}

//---------------------------------------------------------------------------
//! �W�I���g���V�F�[�_�[���擾
//---------------------------------------------------------------------------
gpu::ShaderGs* const ShaderManager::getShaderGs(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬B
    if(!shaderGs_.count(fileName)) {
        gpu::ShaderGs* shaderGs = gpu::ShaderGs::create(fileName.c_str());
        if(!shaderGs) {
            GM_ASSERT_MESSAGE(false, "�W�I���g���V�F�[�_�[�����ł��܂���ł����B");
            return nullptr;
        }
        shaderGs_.insert(std::make_pair(fileName, shaderGs));
    }

    return shaderGs_[fileName].get();
}

//---------------------------------------------------------------------------
//! �n���V�F�[�_�[���擾
//---------------------------------------------------------------------------
gpu::ShaderHs* const ShaderManager::getShaderHs(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬
    if(!shaderHs_.count(fileName)) {
        gpu::ShaderHs* shaderHs = gpu::ShaderHs::create(fileName.c_str());
        if(!shaderHs) {
            GM_ASSERT_MESSAGE(false, "�n���V�F�[�_�[�����ł��܂���ł����B");
            return nullptr;
        }
        shaderHs_.insert(std::make_pair(fileName, shaderHs));
    }

    return shaderHs_[fileName].get();
}

//---------------------------------------------------------------------------
//! �h���C���V�F�[�_�[���擾
//---------------------------------------------------------------------------
gpu::ShaderDs* const ShaderManager::getShaderDs(const std::string& fileName)
{
    // �Ȃ��ꍇ�쐬
    if(!shaderDs_.count(fileName)) {
        gpu::ShaderDs* shaderDs = gpu::ShaderDs::create(fileName.c_str());
        if(!shaderDs) {
            GM_ASSERT_MESSAGE(false, "�h���C���V�F�[�_�[�����ł��܂���ł����B");
            return nullptr;
        }
        shaderDs_.insert(std::make_pair(fileName, shaderDs));
    }

    return shaderDs_[fileName].get();
}

//---------------------------------------------------------------------------
//!
//---------------------------------------------------------------------------
gpu::InputLayout* ShaderManager::createInputLayout(const std::string& fileName)
{
    gpu::InputLayout* inputLayout = nullptr;

     
	// clang-format off
    if(fileName == "vsAnimationModel") {
        D3D11_INPUT_ELEMENT_DESC layout[]{
            { "POSITION"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexModel, position_  ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL"     , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexModel, normal_    ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR"      , 0, DXGI_FORMAT_R8G8B8A8_UNORM , 0, offsetof(VertexModel, color_     ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, offsetof(VertexModel, uv_        ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONE_INDEX" , 0, DXGI_FORMAT_R32_SINT       , 0, offsetof(VertexModel, boneIndex_ ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONE_INFL_INDEX" , 0, DXGI_FORMAT_R32_SINT  , 0, offsetof(VertexModel, boneInflIndex_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONE_WEIGHT", 0, DXGI_FORMAT_R32_FLOAT      , 0, offsetof(VertexModel, boneWeight_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        inputLayout = new gpu::InputLayout(layout);
    }
    else if(fileName == "vsTexture2D") {
        D3D11_INPUT_ELEMENT_DESC layout[]{
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexTexture2D, position_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexTexture2D, uv_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        inputLayout = new gpu::InputLayout(layout);
    }
	else {

		D3D11_INPUT_ELEMENT_DESC layout[]{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal_)  , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, tangent_) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, binormal_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R8G8B8A8_UNORM , 0, offsetof(Vertex, color_)   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, offsetof(Vertex, uv_)      , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inputLayout = new gpu::InputLayout(layout);
	}
	// clang-format on

    return inputLayout;
}
