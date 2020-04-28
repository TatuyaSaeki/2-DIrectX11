//===========================================================================
//!	@file	file_watcher.cpp
//!	@brief	ファイル監視
//===========================================================================

// stdafx.hプリコンパイルヘッダでの定義は以下のとおり
// (XP SP2以降)
//#define _WIN32_WINNT 0x0502
//#include <stdio.h>
//#include <locale.h>
//#include <tchar.h>
//#include <vector>
//#include <conio.h>
//#include <Windows.h>

// 外部非公開
namespace {

// エラーの表示
void showError(LPCTSTR msg)
{
    DWORD errcode = GetLastError();
    _tprintf(_T("%s errorcode: %lx\r\n"), msg, errcode);
}

}   // namespace

//------------------------------------------------------------------------------------------------
//! デストラクタ
//------------------------------------------------------------------------------------------------
FileWatcher::~FileWatcher()
{
    // 途中終了するなら非同期I/Oも中止し、
    // Overlapped構造体をシステムが使わなくなるまで待機する必要がある.
    CancelIo(directoryHandle_);
    WaitForSingleObject(asyncFileIoEvent_, INFINITE);

    // ハンドルの解放
    CloseHandle(directoryHandle_);
    CloseHandle(asyncFileIoEvent_);
}

//------------------------------------------------------------------------------------------------
//! 監視開始
//------------------------------------------------------------------------------------------------
bool FileWatcher::initialize(char* watchPath)
{
    // 対象のディレクトリを監視用にオープンする.
    // 共有ディレクトリ使用可、対象フォルダを削除可
    // 非同期I/O使用
    directoryHandle_ = CreateFile(
        watchPath,                                                // 監視先
        FILE_LIST_DIRECTORY,                                      // 渡すのはディレクトリー一覧
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,   // アクセス件
        NULL,                                                     //
        OPEN_EXISTING,                                            //
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,        // ReadDirectoryChangesW用
        NULL);                                                    //
    if(directoryHandle_ == INVALID_HANDLE_VALUE) {
        showError(_T("CreateFileでの失敗"));
        return false;
    }

    // 監視条件 (FindFirstChangeNotificationと同じ)
    filter_ =
        FILE_NOTIFY_CHANGE_FILE_NAME |    // ファイル名の変更
        FILE_NOTIFY_CHANGE_DIR_NAME |     // ディレクトリ名の変更
        FILE_NOTIFY_CHANGE_ATTRIBUTES |   // 属性の変更
        FILE_NOTIFY_CHANGE_SIZE |         // サイズの変更
        FILE_NOTIFY_CHANGE_LAST_WRITE;    // 最終書き込み日時の変更

    // 変更されたファイルのリストを記録するためのバッファ.
    // 最初のReadDirectoryChangesWの通知から次のReadDirectoryChangesWまでの
    // 間に変更されたファイルの情報を格納できるだけのサイズが必要.
    // バッファオーバーとしてもファイルに変更が発生したことは感知できるが、
    // なにが変更されたかは通知できない。
    static const u32 bufferSize = 8 * 1024 * 1024;
    directoryNames_.resize(bufferSize);

    // 非同期I/Oの完了待機用, 手動リセットモード。
    // 変更通知のイベント発報とキャンセル完了のイベント発報の
    // 2つのイベントソースがあるためイベントの流れが予想できず
    // 自動リセットイベントにするのは危険。
    asyncFileIoEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);

    //----------------------------------------------------------------------------------
    // 初回に監視リクエストを実行しておく
    //----------------------------------------------------------------------------------
    requestFileWatch();

    return true;
}

//------------------------------------------------------------------------------------------------
//! ファイル監視をリクエスト
//------------------------------------------------------------------------------------------------
void FileWatcher::requestFileWatch()
{
    // Sleepを使い、イベントハンドリングが遅いケースをエミュレートする.
    // ループ2回目以降ならばSleep表示中にファイルを変更しても、
    // 変更が追跡されていることを確認できる.
    // またバッファサイズを超えるほどにたくさんのファイルを変更すると
    // バッファオーバーを確認できる。
    //    const int mx = waittime * 10;
    //    for(int idx = 0; idx < mx; idx++) {
    //        _tprintf(_T("sleep... %d/%d \r"), idx + 1, mx);
    //        Sleep(100);
    //    }
    //    _tprintf(_T("\r\nstart.\r\n"));

    // イベントの手動リセット
    ResetEvent(asyncFileIoEvent_);

    // 非同期I/O
    overlapped_.hEvent = asyncFileIoEvent_;

    // 変更を監視する.
    // 初回呼び出し時にシステムが指定サイズでバッファを確保し、そこに変更を記録する.
    // 完了通知後もシステムは変更を追跡しており、後続のReadDirectoryChangeWの
    // 呼び出しで、前回通知後からの変更をまとめて受け取ることができる.
    // バッファがあふれた場合はサイズ0で応答が返される.
    if(!ReadDirectoryChangesW(
           directoryHandle_,         // 対象ディレクトリ
           directoryNames_.data(),   // 通知を格納するバッファ
           DWORD(directoryNames_.size()),   // バッファサイズ
           true,                     // サブディレクトリを対象にするか?
           filter_,                  // 変更通知を受け取るフィルタ
           nullptr,                  // (結果サイズ, 非同期なので未使用)
           &overlapped_,             // 非同期I/Oバッファ
           nullptr                   // (完了ルーチン, 未使用)
           )) {
        // 開始できなかった場合のエラー
        showError(_T("ReadDirectoryChangesWでの失敗"));
        return;
    }
}

//------------------------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------------------------
void FileWatcher::update()
{
    //----------------------------------------------------------------------------------
    // もし変更通知が来ていたらファイル名を取得。
    // 来ていなかったら即座に抜ける
    //----------------------------------------------------------------------------------

    // 変更通知を取得
    u32 waitResult = WaitForSingleObject(asyncFileIoEvent_, 0);   // 即時チェック

    if(waitResult == WAIT_TIMEOUT) {
        return;   // 変更通知がなかった場合
    }

    //----------------------------------------------------------------------------------
    // 非同期I/Oの結果を取得する.
    //----------------------------------------------------------------------------------
    DWORD resultSize = 0;
    if(!GetOverlappedResult(directoryHandle_, &overlapped_, &resultSize, false)) {
        // 結果取得に失敗した場合
        showError(_T("GetOverlappedResultでの失敗"));
        return;
    }

    // 変更通知をコンソールにダンプする.
    _tprintf(_T("returned size=%ld\r\n"), resultSize);

    if(resultSize == 0) {
        // 返却サイズ、0ならばバッファオーバーを示す
        _tprintf(_T("buffer overflow!!\r\n"));
    }
    else {
        // 最初のエントリに位置付ける
        FILE_NOTIFY_INFORMATION* data   = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(directoryNames_.data());
        //size_t                   offset = 0;

        // エントリの末尾まで繰り返す
        for(;;) {
            // アクションタイプを可読文字に変換
            wchar_t* actionName = L"UNKNOWN";
            switch(data->Action) {
                case FILE_ACTION_ADDED: actionName = L"Added"; break;
                case FILE_ACTION_REMOVED: actionName = L"Removed"; break;
                case FILE_ACTION_MODIFIED: actionName = L"Modified"; break;
                case FILE_ACTION_RENAMED_OLD_NAME: actionName = L"Rename Old"; break;
                case FILE_ACTION_RENAMED_NEW_NAME: actionName = L"Rename New"; break;
                default: break;
            }

            // ファイル名はヌル終端されていないので
            // 長さから終端をつけておく.
            DWORD                byteSize = data->FileNameLength;            // 文字数ではなく, バイト数
            std::vector<wchar_t> fileName(byteSize / sizeof(wchar_t) + 1);   // ヌル終端用に+1
            memcpy(fileName.data(), data->FileName, byteSize);

            //---------------------------------------------------------------------------------------
            // リソースパスに変換
            //---------------------------------------------------------------------------------------

            // バックスラッシュをスラッシュに置換
            for(auto& x : fileName) {
                if(x == '\\')
                    x = '/';
            }

            // すべて小文字に変更
			_wcslwr_s(fileName.data(), fileName.size());   // 小文字に変換
			//_wcsupr_s(fileName.data(), fileName.size());  // 大文字に変換

            // パスと拡張子に分離
            wchar_t drive[MAX_PATH];
            wchar_t dir[MAX_PATH];
            wchar_t name[MAX_PATH];
            wchar_t ext[MAX_PATH];
            _wsplitpath_s(fileName.data(), drive, dir, name, ext);

            // リソースパスに変換
            wchar_t resourcePath[MAX_PATH];
            _wmakepath_s(resourcePath, nullptr, dir, name, ext);


            // アクションと対象ファイルを表示.
            // (ファイル名は指定ディレクトリからの相対パスで通知される.)
            wchar_t message[512];

            wsprintfW(message, L"ファイル変更がありました. ACTION[%s] FILE<%s> RESOURCE<%s> EXT<%s>", actionName, &fileName[0], resourcePath, ext);
            MessageBoxW(nullptr, message, L"ファイル変更", MB_OK);

			// リロード発行
			//reloadResource(resourcePath);

            if(data->NextEntryOffset == 0) {
                // 次のエントリは無し
                break;
            }
            // 次のエントリの位置まで移動する. (現在アドレスからの相対バイト数)
            data = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                reinterpret_cast<unsigned char*>(data) + data->NextEntryOffset);
        }
    }   // for

    //----------------------------------------------------------------------------------
    // 再度監視リクエストを登録
    //----------------------------------------------------------------------------------
    requestFileWatch();
}
