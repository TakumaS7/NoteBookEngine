
#pragma once

#include <windows.h>
#include "xaudio2.h"

#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

bool InitMediaFoundation();
bool InitSound(HWND hWnd);

void UninitSound(void);
void UninitMediaFoundation();

int LoadSound(const wchar_t* pFilename);
void PlaySound(int index, int loopCount);
void StopSound(int index);
void StopSoundAll(void);
void SetVolume(int index, float vol);

/* mp3→PCMデコード関数 */
bool LoadMP3WithMF(const wchar_t* filename, BYTE** outBuffer, DWORD* outSize, WAVEFORMATEX** outWfx);
