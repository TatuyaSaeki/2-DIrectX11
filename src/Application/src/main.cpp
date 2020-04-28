//===========================================================================
//!	@file	main.cpp
//!	@brief	アプリケーションエントリーポイント
//===========================================================================

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

static s32 mouseScrollCount = 0; //!< マウススクロール用

s32 getMouseScroll()
{
    return mouseScrollCount;
}

//---------------------------------------------------------------------------
//	ウィンドウプロシージャ
//!	@param	[in]	hWnd	ウィンドウハンドル
//!	@param	[in]	message	ウィンドウメッセージ
//!	@param	[in]	wparam	パラメータ1
//!	@param	[in]	lparam	パラメータ2
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wparam, lparam))
        return true;

	// ESCで終了
    if(GetKeyState(VK_ESCAPE) & 0x80000) {
        message = WM_DESTROY;
    }

    u32 nWheelFraction = 0;

    switch(message) {
        case WM_PAINT:   // WM_PAINT		ウィンドウ再描画
        {
            //	PAINTSTRUCT ps;
            //	HDC			hdc = BeginPaint(hWnd, &ps);
            //
            //	EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:   //	WM_DESTROY	ウィンドウの破棄
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEWHEEL:   // マウス
        {
            s32 zDelta = GET_WHEEL_DELTA_WPARAM(wparam);   // 回転量

            // 前回の端数を追加
            zDelta += nWheelFraction;

            // ノッチ数を求める
            s32 nNotch = zDelta / WHEEL_DELTA;

            nWheelFraction = zDelta % WHEEL_DELTA;

            if(nNotch > 0) {
                // 上に回転（チルト）した
                mouseScrollCount = (mouseScrollCount < 3) ? ++mouseScrollCount : mouseScrollCount;
            }
            else if(nNotch < 0) {
                // 下に回転（チルト）した
                mouseScrollCount = (mouseScrollCount > -3) ? --mouseScrollCount : mouseScrollCount;
            }
        }
        default:   // その他デフォルト
            break;
    }
    return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	アプリケーションエントリーポイント
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //SystemRender systemRender = SystemRender::getInstance();

    static const char* titleName = "DirectX11 Sample";   // ウィンドウタイトル名
    static const char* className = "DX11Sample";         // ウィンドウクラス名

	//=============================================================
    // ウィンドウクラスの登録
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
        //---- 登録
        RegisterClassEx(&wcex);
    }

	//=============================================================
    // ウィンドウ表示位置を画面中央にセンタリング
	//=============================================================
    //u32  style   = WS_OVERLAPPEDWINDOW;//
    u32  style   = WS_VISIBLE | WS_POPUP;
    u32  styleEx = 0;
    RECT rect    = { 0, 0, WINDOW_W, WINDOW_H };   // ウィンドウの描画領域のサイズ

#if LIVE_CODING
    rect.right /= 2;
    rect.bottom /= 2;
    styleEx |= WS_EX_TOPMOST;
#endif
    //---- ウィンドウサイズの枠込みでのサイズを計算＆補正
    AdjustWindowRect(&rect, style, false);

    s32 w = rect.right - rect.left;
    s32 h = rect.bottom - rect.top;
    s32 x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    s32 y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

#if LIVE_CODING
    x = 0;   // 画面左上に寄せる
    y = 0;
#endif

	//=============================================================
    // ウィンドウの作成
	//=============================================================
    HWND hWnd = CreateWindowEx(styleEx,
                               className,                  // [in] ウィンドウクラス名
                               titleName,                  // [in]	ウィンドウタイトル名
                               style,                      // [in]	ウィンドウハンドル
                               x, y,                       // [in] XY座標
                               w, h,                       // [in]	幅・高さ
                               nullptr,                    // [in] 親ウィンドウハンドル
                               nullptr,                    // [in] メニューハンドル
                               GetModuleHandle(nullptr),   // [in] アプリケーションインスタンスハンドル
                               nullptr);                   // [in] WM_CREATEの引数（任意）

    if(hWnd == nullptr)
        return 0;

    //---- ウィンドウの表示開始（初期状態は非表示のためここで表示ON)
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

	//=============================================================
    // DirectX初期化
	//=============================================================
    if(DirectX11::getInstance().setup(WINDOW_W, WINDOW_H, hWnd) == false) {
        return false;
    }

    if(!PRIM_Initialize()) {
        return false;
    }

	//=============================================================
    // アプリ側初期化
	//=============================================================
    std::unique_ptr<AppMainSystem> appMainSystem(new AppMainSystem());
    if(!appMainSystem) {
        return false;
    }
    if(!appMainSystem->initialize(WINDOW_W, WINDOW_H, hWnd)) {
        return false;
    }

	//=============================================================
    // メインメッセージループ
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
            // アプリ側開始
            appMainSystem->run();
        }
    }

    PRIM_Finalize();

	//=============================================================
    // アプリケーションの解放
	//=============================================================
    appMainSystem->finalize();
    appMainSystem.reset();

    // DirectX解放
    DirectX11::getInstance().cleanup();

    //---- 終了コード
    return (int)msg.wParam;
}
