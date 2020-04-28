//===========================================================================
//! @file	directX.cpp
//!	@brief	DirectX11
//===========================================================================

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
DirectX11::~DirectX11()
{
    cleanup();
}

//---------------------------------------------------------------------------
//! ������
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	hWnd	�`��Ώۂ̃E�B���h�E�n���h��
//!	@return	true	����I��	
//!	@return	false	�G���[�I��
//---------------------------------------------------------------------------
bool DirectX11::setup(u32 width, u32 height, HWND hWnd)
{
    HRESULT hr                = S_OK;
    u32     createDeviceFlags = 0;
#if 1
    //=============================================================
    // �f�o�b�O���C���[�f�o�C�X���g�p
    //=============================================================
    // �i�p�t�H�[�}���X�ƈ��������ɃG���[�ڍ׃��b�Z�[�W�o�͂��s���j
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //-------------------------------------------------------------
    // �f�o�C�X�쐬�̂��߂̃I�v�V�����ݒ�
    //-------------------------------------------------------------
    {
        DXGI_SWAP_CHAIN_DESC desc{};
        DXGI_MODE_DESC&      buffer = desc.BufferDesc;

        buffer.Width                   = width;                                  // ��ʉ𑜓x(��)
        buffer.Height                  = height;                                 // ��ʉ𑜓x(����)
        buffer.RefreshRate.Numerator   = 60;                                     // ���t���b�V�����[�g(���q) 60.0fps
        buffer.RefreshRate.Denominator = 1;                                      // ���t���b�V�����[�g(����)
        buffer.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;             // �s�N�Z���t�H�[�}�b�g
        buffer.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // �f���X�L�������C���̎w��
        buffer.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;          // �摜�X�P�[�����O
        desc.SampleDesc.Count          = 1;                                      // �}���`�T���v���A���`�G�C���A�V���O�̃T���v����(1�2�4�8)
        desc.SampleDesc.Quality        = 0;                                      // �@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@�@�̕i��(0�`)
        desc.BufferCount               = 3;                                      // �t�����g�o�b�t�@���܂߂��o�b�t�@�� 2:�_�u���o�b�t�@ 3:�g���v���o�b�t�@
        desc.OutputWindow              = hWnd;                                   // �o�͑Ώۂ̃E�B���h�E�n���h��
        desc.Windowed                  = true;                                   // true:�E�B���h�E���[�h false:�t���X�N���[�����[�h
        desc.SwapEffect                = DXGI_SWAP_EFFECT_DISCARD;               // ��ʍX�V���[�h

        //---- �o�b�N�o�b�t�@�̗p�r
        desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |           // �e�N�X�`��
                           DXGI_USAGE_RENDER_TARGET_OUTPUT |   // �`��^�[�Q�b�g
                           DXGI_USAGE_BACK_BUFFER;             // �o�b�N�o�b�t�@

        //---- �o�b�N�o�b�t�@�̓���t���O
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // �𑜓x�ύX������

        //-------------------------------------------------------------
        // D3D�f�o�C�X�쐬
        // �����Ƀo�b�N�o�b�t�@�X���b�v�`�F�C����Immediate�R���e�L�X�g���쐬�����
        //-------------------------------------------------------------
        //---- D3D�f�o�C�X�������̎��s����(Hardware��Software)
        static const std::array<D3D_DRIVER_TYPE, 3> driverTypes = { {
            D3D_DRIVER_TYPE_HARDWARE,    // �n�[�h�E�F�AGPU
            D3D_DRIVER_TYPE_WARP,        // �\�t�g�E�F�AWARP�f�o�C�X (Windows Advanced Rasterization Platform)
            D3D_DRIVER_TYPE_REFERENCE,   // ���t�@�����X���X�^���C�U
        } };

        //---- D3D�f�o�C�X�ŃT�|�[�g����@�\���x���B�쐬���s����
        static const std::array<D3D_FEATURE_LEVEL, 2> featureLevels = { {
            D3D_FEATURE_LEVEL_11_0,   // DirectX 11.0 (�e�Z���[�^�[, ComputeShader, �x���R���e�L�X�g)
            D3D_FEATURE_LEVEL_10_0,   // DirectX 10.0	(�W�I���g���V�F�[�_�[, �X�g���[���o��)
        } };

        // �n�[�h�E�F�A�T�|�[�g���Ă���GPU���珇�ɍ쐬�����s���Ă���
        for(auto it = driverTypes.begin(); it != driverTypes.end(); ++it) {
            driverType_ = *it;

            hr = D3D11CreateDeviceAndSwapChain(
                nullptr,                                   //	[in]	�Ώۂ̃f�B�X�v���C�A�_�v�^GPU
                driverType_,                               //	[in]	�f�o�C�X�̎��
                nullptr,                                   //	[in]	�\�t�g�E�F�A���X�^���C�U
                createDeviceFlags,                         //	[in]	�I�v�V�����t���O
                &featureLevels[0],                         //	[in]	���s����@�\���x���̔z��
                static_cast<UINT>(featureLevels.size()),   //	[in]	�@�@�@�@�V�@�@�@�@�@�z��
                D3D11_SDK_VERSION,                         //	[in]	SDK�o�[�W���� (D3D11_SDK_VERSION�Œ�)
                &desc,                                     //	[in]	�X���b�v�`�F�C���̐ݒ�p�^���[�^
                &d3dSwapChain_,                            //	[out]	�쐬���ꂽ�X���b�v�`�F�C��
                &d3dDevice_,                               //	[out]	�쐬���ꂽDirect3D�f�o�C�X
                &featureLevel_,                            //	[out]	�쐬���ꂽ�f�o�C�X�̋@�\���x��
                &d3dImmediateContext_);                    //	[out]	�쐬���ꂽ�C�~�f�B�G�C�g�R���e�L�X�g

            //---- ���������ꍇ�͂���ȍ~�̎��s�͍s�킸�ɍ쐬���ꂽ�f�o�C�X�𗘗p�B
            if(hr == S_OK) {
                break;
            }
        }
        // ���ׂĎ��s�����ꍇ�̓G���[
        if(hr != S_OK) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void DirectX11::cleanup()
{
    //---- �f�o�C�X�R���e�L�X�g�ɃZ�b�g����Ă���I�u�W�F�N�g�����ׂĉ�������B
    if(d3dImmediateContext_) {
        d3dImmediateContext_->ClearState();
    }
}
