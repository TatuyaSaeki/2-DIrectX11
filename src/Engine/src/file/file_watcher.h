//===========================================================================
//!	@file	file_watcher.h
//!	@brief	ファイル監視
//===========================================================================
#pragma once

//===========================================================================
//!	ファイル監視
//===========================================================================
class FileWatcher
{
public:
    FileWatcher() = default;

    virtual ~FileWatcher();

    //! 監視開始
    bool initialize(char* watchPath);

    //! 更新
    void update();

private:
    //! ファイル監視をリクエスト
    void requestFileWatch();

private:
    HANDLE            directoryHandle_  = INVALID_HANDLE_VALUE;   //!< ディレクトリ監視用ハンドル(非同期)
    HANDLE            asyncFileIoEvent_ = INVALID_HANDLE_VALUE;   //!< 非同期I/O完了待機用の同期オブジェクト
    u32               filter_           = 0;                      //!< 監視条件フィルター
    OVERLAPPED        overlapped_       = {};                     //!< 非同期I/Oオブジェクト
    std::vector<char> directoryNames_;                            //!< 変更されたファイル一覧を格納するバッファ
};
