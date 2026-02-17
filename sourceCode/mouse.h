/*===============================================================*/
//
//	mouse.h(マウス処理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/
#ifndef _MOUSE_H
#define _MOUSE_H

#include <windows.h>
#include <memory>
#include "vector3.h"
#include "gameObject.h"
#include <list>

#define HIT_LIST_MAX 1

class Mouse
{
private:
    // マウスモード
    enum Mouse_PositionMode_tag
    {
        MOUSE_POSITION_MODE_ABSOLUTE, // 絶対座標モード
        MOUSE_POSITION_MODE_RELATIVE, // 相対座標モード
    };

    struct Mouse_State
    {
        // マウス状態構造体
        bool m_leftButton;
        bool m_middleButton;
        bool m_rightButton;
        bool m_xButton1;
        bool m_xButton2;
        bool m_oldLeftButton;
        float m_x;
        float m_y;
        int m_scrollWheelValue;
        Mouse_PositionMode_tag m_positionMode;
    };

    static HWND                         m_Window;
    static Mouse_State                  m_State;
    static Mouse_PositionMode_tag       m_Mode;
    static HANDLE                       m_ScrollWheelValue;
    static HANDLE                       m_RelativeRead;
    static HANDLE                       m_AbsoluteMode;
    static HANDLE                       m_RelativeMode;
    static int                          m_LastX;
    static int                          m_LastY;
    static int                          m_RelativeX;
    static int                          m_RelativeY;
    static bool                         m_InFocus;
    static bool                         m_anyHit;
    static GameObject*                  m_dragTarget;
    static bool                         m_dragging;
    static bool                         m_scrollFlag;
    static bool                         m_clickFrag;

public:

    // マウスモジュールの初期化
    static void Mouse_Initialize(HWND window);

    // マウスモジュールの終了処理
    static void Mouse_Finalize(void);

    // マウスの状態を取得する
    static void Mouse_GetState(Mouse_State* pState);

    // 累積したマウススクロールホイール値をリセットする
    static void Mouse_ResetScrollWheelValue(void);

    // マウスのポジションモードを設定する（デフォルトは絶対座標モード）
    static void Mouse_SetMode(Mouse_PositionMode_tag mode);

    // マウスの接続を検出する
    static bool Mouse_IsConnected(void);

    // マウスカーソルが表示されているか確認する
    static bool Mouse_IsVisible(void);

    // マウスカーソル表示を設定する
    static void Mouse_SetVisible(bool visible);

    // マウス制御のためのウィンドウメッセージプロシージャフック関数
    static void Mouse_ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

    static void clipToWindow();

    /* マウスのポジション取得 */
    static Vector3 GetMousePosition() { return { m_State.m_x, m_State.m_y, 0.0f }; }

    /* マウスの左クリックフラグ取得 */
    static bool GetLeftClick();

    /* マウス左クリックして離したフラグのセッター・ゲッター */
    static bool GetLeftClickTrigger();

    /* カーソルのデザイン変更 */
    static void ChangeCursor();

    static void SetMousePosition(float x, float y) {
        m_State.m_x = x;
        m_State.m_y = y;
    }

    /* ドラッグするターゲットのセット・ゲット・クリア */
    static void SetDragTarget(GameObject* obj) { m_dragTarget = obj; }
    static GameObject* GetDragTarget() { return m_dragTarget; }
    static void ClearDragTarget() { m_dragTarget = nullptr; }

    /* ドラッグしているかフラグのセッター・ゲッター */
    static void SetDragging(bool dragging) { m_dragging = dragging; }
    static bool GetDragging() { return m_dragging; }

    /* マウスホイール操作のゲッター */
    static int GetScrollWheelValue() { 
        if (m_scrollFlag) {
            return m_State.m_scrollWheelValue;
        }
        else {
            return 0;
        }
    }
    /* マウスホイールの状態リセット */
    static void ResetScrollWheelValue() { m_State.m_scrollWheelValue = 0; }
    /* マウススクロール許可セッター（ImGuiの上に乗っていたらfalse） */
    static void SetScrollFlag(bool flag) { m_scrollFlag = flag; }
    static void SetClickFlag(bool flag) { m_clickFrag = flag; }
};

// 導入方法
//
// 対象のウィンドウが生成されたらそのウィンドウハンドルを引数に初期化関数を呼ぶ
//
// Mouse_Initialize(hwnd);
//
// ウィンドウメッセージプロシージャからマウス制御用フック関数を呼び出す
//
// LResult CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     switch (message)
//     {
//     case WM_ACTIVATEAPP:
//     case WM_INPUT:
//     case WM_MOUSEMOVE:
//     case WM_LBUTTONDOWN:
//     case WM_LBUTTONUP:
//     case WM_RBUTTONDOWN:
//     case WM_RBUTTONUP:
//     case WM_MBUTTONDOWN:
//     case WM_MBUTTONUP:
//     case WM_MOUSEWHEEL:
//     case WM_XBUTTONDOWN:
//     case WM_XBUTTONUP:
//     case WM_MOUSEHOVER:
//         Mouse_ProcessMessage(message, wParam, lParam);
//         break;
//
//     }
// }
//

#endif //_MOUSE_H
