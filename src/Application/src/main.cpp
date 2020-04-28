//===========================================================================
//!	@file	main.cpp
//!	@brief	�A�v���P�[�V�����G���g���[�|�C���g
//===========================================================================

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

static s32 mouseScrollCount = 0; //!< �}�E�X�X�N���[���p

s32 getMouseScroll()
{
    return mouseScrollCount;
}

//---------------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//!	@param	[in]	hWnd	�E�B���h�E�n���h��
//!	@param	[in]	message	�E�B���h�E���b�Z�[�W
//!	@param	[in]	wparam	�p�����[�^1
//!	@param	[in]	lparam	�p�����[�^2
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wparam, lparam))
        return true;

	// ESC�ŏI��
    if(GetKeyState(VK_ESCAPE) & 0x80000) {
        message = WM_DESTROY;
    }

    u32 nWheelFraction = 0;

    switch(message) {
        case WM_PAINT:   // WM_PAINT		�E�B���h�E�ĕ`��
        {
            //	PAINTSTRUCT ps;
            //	HDC			hdc = BeginPaint(hWnd, &ps);
            //
            //	EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:   //	WM_DESTROY	�E�B���h�E�̔j��
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEWHEEL:   // �}�E�X
        {
            s32 zDelta = GET_WHEEL_DELTA_WPARAM(wparam);   // ��]��

            // �O��̒[����ǉ�
            zDelta += nWheelFraction;

            // �m�b�`�������߂�
            s32 nNotch = zDelta / WHEEL_DELTA;

            nWheelFraction = zDelta % WHEEL_DELTA;

            if(nNotch > 0) {
                // ��ɉ�]�i�`���g�j����
                mouseScrollCount = (mouseScrollCount < 3) ? ++mouseScrollCount : mouseScrollCount;
            }
            else if(nNotch < 0) {
                // ���ɉ�]�i�`���g�j����
                mouseScrollCount = (mouseScrollCount > -3) ? --mouseScrollCount : mouseScrollCount;
            }
        }
        default:   // ���̑��f�t�H���g
            break;
    }
    return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	�A�v���P�[�V�����G���g���[�|�C���g
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //SystemRender systemRender = SystemRender::getInstance();

    static const char* titleName = "DirectX11 Sample";   // �E�B���h�E�^�C�g����
    static const char* className = "DX11Sample";         // �E�B���h�E�N���X��

	//=============================================================
    // �E�B���h�E�N���X�̓o�^
	//=============================================================
    {
        WNDCLASSEX wcex;
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.hInstance     = GetModuleHandle(nullptr);
        wcex.hIcon         = nullptr;
        wcex.hCursor       = LoadCursor(nullptr, IDC_CROSS);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName  = nullptr;
        wcex.lpszClassName = className;
        wcex.hIconSm       = nullptr;
        //---- �o�^
        RegisterClassEx(&wcex);
    }

	//=============================================================
    // �E�B���h�E�\���ʒu����ʒ����ɃZ���^�����O
	//=============================================================
    //u32  style   = WS_OVERLAPPEDWINDOW;//
    u32  style   = WS_VISIBLE | WS_POPUP;
    u32  styleEx = 0;
    RECT rect    = { 0, 0, WINDOW_W, WINDOW_H };   // �E�B���h�E�̕`��̈�̃T�C�Y

#if LIVE_CODING
    rect.right /= 2;
    rect.bottom /= 2;
    styleEx |= WS_EX_TOPMOST;
#endif
    //---- �E�B���h�E�T�C�Y�̘g���݂ł̃T�C�Y���v�Z���␳
    AdjustWindowRect(&rect, style, false);

    s32 w = rect.right - rect.left;
    s32 h = rect.bottom - rect.top;
    s32 x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    s32 y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

#if LIVE_CODING
    x = 0;   // ��ʍ���Ɋ񂹂�
    y = 0;
#endif

	//=============================================================
    // �E�B���h�E�̍쐬
	//=============================================================
    HWND hWnd = CreateWindowEx(styleEx,
                               className,                  // [in] �E�B���h�E�N���X��
                               titleName,                  // [in]	�E�B���h�E�^�C�g����
                               style,                      // [in]	�E�B���h�E�n���h��
                               x, y,                       // [in] XY���W
                               w, h,                       // [in]	���E����
                               nullptr,                    // [in] �e�E�B���h�E�n���h��
                               nullptr,                    // [in] ���j���[�n���h��
                               GetModuleHandle(nullptr),   // [in] �A�v���P�[�V�����C���X�^���X�n���h��
                               nullptr);                   // [in] WM_CREATE�̈����i�C�Ӂj

    if(hWnd == nullptr)
        return 0;

    //---- �E�B���h�E�̕\���J�n�i������Ԃ͔�\���̂��߂����ŕ\��ON)
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

	//=============================================================
    // DirectX������
	//=============================================================
    if(DirectX11::getInstance().setup(WINDOW_W, WINDOW_H, hWnd) == false) {
        return false;
    }

    if(!PRIM_Initialize()) {
        return false;
    }

	//=============================================================
    // �A�v����������
	//=============================================================
    std::unique_ptr<AppMainSystem> appMainSystem(new AppMainSystem());
    if(!appMainSystem) {
        return false;
    }
    if(!appMainSystem->initialize(WINDOW_W, WINDOW_H, hWnd)) {
        return false;
    }

	//=============================================================
    // ���C�����b�Z�[�W���[�v
    //=============================================================
    MSG msg;
    for(;;) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if(GetMessage(&msg, nullptr, 0, 0) == 0) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // �A�v�����J�n
            appMainSystem->run();
        }
    }

    PRIM_Finalize();

	//=============================================================
    // �A�v���P�[�V�����̉��
	//=============================================================
    appMainSystem->finalize();
    appMainSystem.reset();

    // DirectX���
    DirectX11::getInstance().cleanup();

    //---- �I���R�[�h
    return (int)msg.wParam;
}
