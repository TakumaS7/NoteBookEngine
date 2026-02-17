/*===============================================================*/
//
//	main.cpp(メイン処理)
// 
// 
// ---------------------------------- 2025/05/14 製作者：佐藤琢磨
//
/*===============================================================*/


#include "main.h"
#include "manager.h"
#include <thread>
#include "time.h"
#include "mouse.h"
#include "imGuiManager.h"
#include "renderer.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";
HWND g_Window;

/* デバッグウィンドウ */
const char* DEBUG_WINDOW_CLASS_NAME = "DebugWindowClass";
HWND g_DebugWindow;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DebugWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND GetWindow()
{
	return g_Window;
}

HWND GetDebugWindow()
{
	return g_DebugWindow;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	/* デバッグウィンドウ登録 */
	WNDCLASSEX wcex2{};
	{
		wcex2.cbSize = sizeof(WNDCLASSEX);
		wcex2.style = 0;
		wcex2.lpfnWndProc = DebugWndProc;
		wcex2.cbClsExtra = 0;
		wcex2.cbWndExtra = 0;
		wcex2.hInstance = hInstance;
		wcex2.hIcon = nullptr;
		wcex2.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex2.lpszMenuName = nullptr;
		wcex2.lpszClassName = DEBUG_WINDOW_CLASS_NAME;
		wcex2.hIconSm = nullptr;

		RegisterClassEx(&wcex2);
	}



	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	//デルタタイム用変数も初期化
	auto previousTime = std::chrono::high_resolution_clock::now();
	Manager::Init();



	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);




	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;



	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//現在の時間を取得
				auto currentTime = std::chrono::high_resolution_clock::now();
				//デルタタイムの計算（ミリ秒単位）
				Time::m_deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count());
				//秒単位に変換
				Time::m_deltaTime *= 0.001f;
				//現在の時間を次のループの基準時間に更新
				previousTime = currentTime;

				dwExecLastTime = dwCurrentTime;

				Manager::Update();

				Manager::Draw();
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();
	Mouse::Mouse_Finalize();

	CoUninitialize();

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		Mouse::Mouse_Initialize(hWnd);
		return true;
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}

		if (wParam == VK_RETURN)
		{
			// Enterで未確定文字を確定
			ImGuiManager::ConfirmComposition();
		}

		if (wParam == VK_BACK)
		{
			ImGuiManager::Backspace();
		}

		if (wParam == VK_DELETE)
		{
			ImGuiManager::Delete();
		}
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		/* マウスメッセージ処理 */
		Mouse::Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SETCURSOR:
		break;
	case WM_CHAR:
	{
		// IMEの状態をチェック
		HIMC hIMC = ImmGetContext(hWnd);
		bool imeActive = (ImmGetOpenStatus(hIMC) != FALSE);
		ImmReleaseContext(hWnd, hIMC);

		if (!imeActive) // IMEがオフのときだけ処理
		{
			if (!ImGuiManager::HasImeActive()) { break; }

			wchar_t ch = (wchar_t)wParam;

			// 制御文字は無視
			if (ch >= 0x20 && ch != 0x7F)
			{
				std::wstring buffer(1, ch);
				ImGuiManager::AddInputBuffer(buffer);
			}
		}
	}
	break;
	case WM_IME_COMPOSITION:
	{
		if (!ImGuiManager::HasImeActive()) { break; }

		HIMC hIMC = ImmGetContext(hWnd);
		if (lParam & GCS_RESULTSTR)
		{
			LONG size = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, nullptr, 0);
			if (size > 0)
			{
				std::wstring buffer(size / sizeof(wchar_t), L'\0');
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, &buffer[0], size);
				ImGuiManager::AddInputBuffer(buffer);
			}
			ImGuiManager::SetCompositionBuffer(L"");
		}
		else if (lParam & GCS_COMPSTR)
		{
			LONG size = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, nullptr, 0);
			if (size > 0)
			{
				std::wstring buffer(size / sizeof(wchar_t), L'\0');
				ImmGetCompositionStringW(hIMC, GCS_COMPSTR, &buffer[0], size);
				ImGuiManager::SetCompositionBuffer(buffer);
			}
			else
			{
				ImGuiManager::SetCompositionBuffer(L"");
			}
		}
		ImmReleaseContext(hWnd, hIMC);
	}
	break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* デバッグウィンドウ用プロシージャ */
LRESULT CALLBACK DebugWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		g_DebugWindow = nullptr; // 閉じたらハンドルをリセット
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}

		if (wParam == VK_RETURN)
		{
			// Enterで未確定文字を確定
			ImGuiManager::ConfirmComposition();
		}

		if (wParam == VK_BACK)
		{
			ImGuiManager::Backspace();
		}

		if (wParam == VK_DELETE)
		{
			ImGuiManager::Delete();
		}
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		/* マウスメッセージ処理 */
		Mouse::Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SETCURSOR:
		break;
	case WM_CHAR:
	{
		// IMEの状態をチェック
		HIMC hIMC = ImmGetContext(hWnd);
		bool imeActive = (ImmGetOpenStatus(hIMC) != FALSE);
		ImmReleaseContext(hWnd, hIMC);

		if (!imeActive) // IMEがオフのときだけ処理
		{
			wchar_t ch = (wchar_t)wParam;

			// 制御文字は無視
			if (ch >= 0x20 && ch != 0x7F)
			{
				std::wstring buffer(1, ch);
				ImGuiManager::AddInputBuffer(buffer);
			}
		}
	}
	break;
	case WM_IME_COMPOSITION:
	{
		HIMC hIMC = ImmGetContext(hWnd);
		if (lParam & GCS_RESULTSTR)
		{
			LONG size = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, nullptr, 0);
			if (size > 0)
			{
				std::wstring buffer(size / sizeof(wchar_t), L'\0');
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, &buffer[0], size);
				ImGuiManager::AddInputBuffer(buffer);
			}
			ImGuiManager::SetCompositionBuffer(L"");
		}
		else if (lParam & GCS_COMPSTR)
		{
			LONG size = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, nullptr, 0);
			if (size > 0)
			{
				std::wstring buffer(size / sizeof(wchar_t), L'\0');
				ImmGetCompositionStringW(hIMC, GCS_COMPSTR, &buffer[0], size);
				ImGuiManager::SetCompositionBuffer(buffer);
			}
			else
			{
				ImGuiManager::SetCompositionBuffer(L"");
			}
		}
		ImmReleaseContext(hWnd, hIMC);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

// サブウィンドウ作成用の関数
void CreateDebugWindow()
{
	if (g_DebugWindow) return; // すでにあるなら再生成しない

	RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_DebugWindow = CreateWindowEx(
		0,
		DEBUG_WINDOW_CLASS_NAME,
		"デバッグウィンドウ",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	ShowWindow(g_DebugWindow, SW_SHOW);
	UpdateWindow(g_DebugWindow);

	/* デバッグ用スワップチェーン */
	Renderer::InitDebugSwapChain();
}
