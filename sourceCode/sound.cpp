/*==============================================================================

   サウンド処理 [sound.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "sound.h"

/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define MAX_SOUND_NUM 100


/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

/*------------------------------------------------------------------------------
   グローバル変数の定義
------------------------------------------------------------------------------*/
static IXAudio2* g_pXAudio2 = NULL;									// XAudio2オブジェクトへのインターフェイス
static IXAudio2MasteringVoice* g_pMasteringVoice = NULL;			// マスターボイス
static IXAudio2SourceVoice* g_apSourceVoice[MAX_SOUND_NUM] = {};	// ソースボイス
static BYTE* g_apDataAudio[MAX_SOUND_NUM] = {};						// オーディオデータ
static DWORD g_aSizeAudio[MAX_SOUND_NUM] = {};						// オーディオデータサイズ

static wchar_t g_SoundName[MAX_SOUND_NUM][256] = {};				// サウンド名バッファ
static DWORD g_SoundIndex = 0;										// ソースボイス配列の末尾を示すインデックス

/*------------------------------------------------------------------------------
   初期化関数
------------------------------------------------------------------------------*/
bool InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBoxW(hWnd, L"XAudio2オブジェクトの作成に失敗！", L"警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBoxW(hWnd, L"マスターボイスの生成に失敗！", L"警告！", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	return true;
}

bool InitMediaFoundation()
{
	HRESULT hr = MFStartup(MF_VERSION);
	return SUCCEEDED(hr);
}

/*------------------------------------------------------------------------------
   終了処理をする関数
------------------------------------------------------------------------------*/
void UninitSound(void)
{
	// 全てのサウンドを停止する
	for (int nCntSound = 0; nCntSound < (int)g_SoundIndex; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	if (g_pMasteringVoice)
	{
		g_pMasteringVoice->DestroyVoice();
		g_pMasteringVoice = NULL;
	}

	if (g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

void UninitMediaFoundation()
{
	MFShutdown();
}

/*------------------------------------------------------------------------------
   サウンドファイルの読み込み
------------------------------------------------------------------------------*/
int LoadSound(const wchar_t* pFilename)
{
	// 既に読み込み済みかチェック
	for (unsigned int i = 0; i < g_SoundIndex; i++)
	{
		if (wcscmp(g_SoundName[i], pFilename) == 0)
			return i;
	}

	if (g_SoundIndex >= MAX_SOUND_NUM)
	{
		MessageBoxW(NULL, L"最大読み込み数を超過", L"警告！", MB_ICONWARNING);
		return -1;
	}

	HRESULT hr;

	/*==============================
	  拡張子判定
	==============================*/
	const wchar_t* ext = wcsrchr(pFilename, L'.');
	if (!ext)
		return -1;

	/*==============================
	  MP3 読み込み
	==============================*/
	if (_wcsicmp(ext, L".mp3") == 0)
	{
		BYTE* pcmData = nullptr;
		DWORD pcmSize = 0;
		WAVEFORMATEX* wfx = nullptr;

		if (!LoadMP3WithMF(pFilename, &pcmData, &pcmSize, &wfx))
		{
			MessageBoxW(NULL, L"mp3の読み込みに失敗", L"警告！", MB_ICONWARNING);
			return -1;
		}

		// SourceVoice 作成
		hr = g_pXAudio2->CreateSourceVoice(
			&g_apSourceVoice[g_SoundIndex],
			wfx
		);
		CoTaskMemFree(wfx);

		if (FAILED(hr))
			return -1;

		g_apDataAudio[g_SoundIndex] = pcmData;
		g_aSizeAudio[g_SoundIndex] = pcmSize;
	}

	/*==============================
	  WAV 読み込み（既存）
	==============================*/
	else if (_wcsicmp(ext, L".wav") == 0)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx = {};

		hFile = CreateFileW(
			pFilename,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE)
			return -1;

		CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);

		CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

		CheckChunk(hFile, 'atad', &g_aSizeAudio[g_SoundIndex], &dwChunkPosition);

		g_apDataAudio[g_SoundIndex] =
			(BYTE*)malloc(g_aSizeAudio[g_SoundIndex]);

		if (!g_apDataAudio[g_SoundIndex])
		{
			CloseHandle(hFile);
			return -1;
		}

		ReadChunkData(
			hFile,
			g_apDataAudio[g_SoundIndex],
			g_aSizeAudio[g_SoundIndex],
			dwChunkPosition
		);

		CloseHandle(hFile);

		hr = g_pXAudio2->CreateSourceVoice(
			&g_apSourceVoice[g_SoundIndex],
			&wfx.Format
		);

		if (FAILED(hr))
			return -1;
	}
	else
	{
		MessageBoxW(NULL, L"未対応の音声形式", L"警告！", MB_ICONWARNING);
		return -1;
	}

	// サウンド名保存
	wcscpy_s(g_SoundName[g_SoundIndex], 256, pFilename);

	int ret = g_SoundIndex;
	g_SoundIndex++;

	return ret;
}

void SetVolume(int index, float vol)
{
	if (index < 0 || index >= (int)g_SoundIndex)
		return;

	if (!g_apSourceVoice[index])
		return;

	g_apSourceVoice[index]->SetVolume(vol);
}

bool LoadMP3WithMF(const wchar_t* filename, BYTE** outBuffer, DWORD* outSize, WAVEFORMATEX** outWfx)
{
	IMFSourceReader* reader = nullptr;
	IMFMediaType* mediaType = nullptr;

	HRESULT hr = MFCreateSourceReaderFromURL(filename, NULL, &reader);
	if (FAILED(hr)) return false;

	// 出力フォーマットを PCM に指定
	hr = MFCreateMediaType(&mediaType);
	mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	reader->SetCurrentMediaType(
		(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
		NULL,
		mediaType
	);

	mediaType->Release();

	// 実際のフォーマット取得
	IMFMediaType* actualType = nullptr;
	reader->GetCurrentMediaType(
		MF_SOURCE_READER_FIRST_AUDIO_STREAM,
		&actualType
	);

	UINT32 size = 0;
	MFCreateWaveFormatExFromMFMediaType(
		actualType,
		outWfx,
		&size
	);
	actualType->Release();

	// PCMデータ読み込み
	BYTE* buffer = nullptr;
	DWORD bufferSize = 0;

	while (true)
	{
		IMFSample* sample = nullptr;
		IMFMediaBuffer* mediaBuffer = nullptr;
		DWORD flags = 0;

		hr = reader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0,
			NULL,
			&flags,
			NULL,
			&sample
		);

		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
			break;

		if (sample)
		{
			sample->ConvertToContiguousBuffer(&mediaBuffer);

			BYTE* data = nullptr;
			DWORD cb = 0;
			mediaBuffer->Lock(&data, NULL, &cb);

			buffer = (BYTE*)realloc(buffer, bufferSize + cb);
			memcpy(buffer + bufferSize, data, cb);
			bufferSize += cb;

			mediaBuffer->Unlock();
			mediaBuffer->Release();
			sample->Release();
		}
	}

	reader->Release();

	*outBuffer = buffer;
	*outSize = bufferSize;

	return true;
}

/*------------------------------------------------------------------------------
   音声の再生
------------------------------------------------------------------------------*/
void PlaySound(int index, int loopCount)
{
	if (index < 0 || index >= (int)g_SoundIndex)
		return;

	if (!g_apSourceVoice[index])
		return;

	if (!g_apDataAudio[index])
		return;

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[index];
	buffer.pAudioData = g_apDataAudio[index];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount > 0)
		loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;

	// 状態取得
	g_apSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[index]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[index]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[index]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[index]->Start(0);
}

/*------------------------------------------------------------------------------
   音声の停止
------------------------------------------------------------------------------*/
void StopSound(int index)
{
	if (index < 0 || index >= (int)g_SoundIndex)
		return;

	if (!g_apSourceVoice[index])
		return;

	XAUDIO2_VOICE_STATE xa2state;

	g_apSourceVoice[index]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		g_apSourceVoice[index]->Stop(0);
		g_apSourceVoice[index]->FlushSourceBuffers();
	}
}

/*------------------------------------------------------------------------------
   読み込んでいるすべての音声を停止
------------------------------------------------------------------------------*/
void StopSoundAll(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < (int)g_SoundIndex; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

/*------------------------------------------------------------------------------
   WAVEフォーマットのチェック
------------------------------------------------------------------------------*/
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

/*------------------------------------------------------------------------------
   WAVEフォーマットの読み込み
------------------------------------------------------------------------------*/
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

