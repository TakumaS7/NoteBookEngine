/*===============================================================*/
//
//	main.h(ÉÅÉCÉìèàóù)
// 
// 
// ---------------------------------- 2025/05/14 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _MAIN_H
#define _MAIN_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "vector3.h"

#include "DirectXTex.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif


#pragma comment (lib, "winmm.lib")

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "comdlg32.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)


HWND GetWindow();
HWND GetDebugWindow();

void Invoke(std::function<void()> Function, int Time);
void CreateDebugWindow();


#endif //_MAIN_H