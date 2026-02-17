/*===============================================================*/
//
//	mouse.h(マウス処理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#include "mouse.h"

#include <windowsx.h>
#include <assert.h>

#include "manager.h"
#include "scene.h"
#include "mouseCollision.h"

#define SAFE_CLOSEHANDLE(h) if(h){CloseHandle(h); h = NULL;}

// static変数宣言
Mouse::Mouse_State                  Mouse::m_State = {};
HWND                                Mouse::m_Window = NULL;
Mouse::Mouse_PositionMode_tag       Mouse::m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;
HANDLE                              Mouse::m_ScrollWheelValue = NULL;
HANDLE                              Mouse::m_RelativeRead = NULL;
HANDLE                              Mouse::m_AbsoluteMode = NULL;
HANDLE                              Mouse::m_RelativeMode = NULL;
int                                 Mouse::m_LastX = 0;
int                                 Mouse::m_LastY = 0;
int                                 Mouse::m_RelativeX = INT32_MAX;
int                                 Mouse::m_RelativeY = INT32_MAX;
bool                                Mouse::m_InFocus = true;
bool                                Mouse::m_anyHit = false;
GameObject*                         Mouse::m_dragTarget = nullptr;
bool                                Mouse::m_dragging = false;
bool                                Mouse::m_scrollFlag = false;
bool                                Mouse::m_clickFrag = true;


void Mouse::Mouse_Initialize(HWND window)
{
    RtlZeroMemory(&m_State, sizeof(m_State));

    assert(window != NULL);

    RAWINPUTDEVICE Rid;
    Rid.usUsagePage = 0x01 /* HID_USAGE_PAGE_GENERIC */;
    Rid.usUsage = 0x02     /* HID_USAGE_GENERIC_MOUSE */;
    Rid.dwFlags = RIDEV_INPUTSINK;
    Rid.hwndTarget = window;
    RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));

    m_Window = window;
    m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;

    if (!m_ScrollWheelValue) { m_ScrollWheelValue = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE); }
    if (!m_RelativeRead) { m_RelativeRead = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE); }
    if (!m_AbsoluteMode) { m_AbsoluteMode = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE); }
    if (!m_RelativeMode) { m_RelativeMode = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE); }

    m_LastX = 0;
    m_LastY = 0;
    m_RelativeX = INT32_MAX;
    m_RelativeY = INT32_MAX;

    m_InFocus = true;
}

void Mouse::Mouse_Finalize(void)
{
    SAFE_CLOSEHANDLE(m_ScrollWheelValue);
    SAFE_CLOSEHANDLE(m_RelativeRead);
    SAFE_CLOSEHANDLE(m_AbsoluteMode);
    SAFE_CLOSEHANDLE(m_RelativeMode);
}

void Mouse::Mouse_GetState(Mouse_State* pState)
{
    memcpy(pState, &m_State, sizeof(m_State));
    pState->m_positionMode = m_Mode;

    DWORD Result = WaitForSingleObjectEx(m_ScrollWheelValue, 0, FALSE);
    if (Result == WAIT_FAILED) { return; }

    if (Result == WAIT_OBJECT_0) {

        pState->m_scrollWheelValue = 0;
    }

    if (pState->m_positionMode == MOUSE_POSITION_MODE_RELATIVE) {

        Result = WaitForSingleObjectEx(m_RelativeRead, 0, FALSE);
        if (Result == WAIT_FAILED) { return; }

        if (Result == WAIT_OBJECT_0) {
            pState->m_x = 0;
            pState->m_y = 0;
        }
        else {
            SetEvent(m_RelativeRead);
        }
    }
}

void Mouse::Mouse_ResetScrollWheelValue(void)
{
    SetEvent(m_ScrollWheelValue);
}

void Mouse::Mouse_SetMode(Mouse_PositionMode_tag mode)
{
    if (m_Mode == mode)
        return;

    SetEvent((mode == MOUSE_POSITION_MODE_ABSOLUTE) ? m_AbsoluteMode : m_RelativeMode);

    assert(m_Window != NULL);

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_HOVER;
    tme.hwndTrack = m_Window;
    tme.dwHoverTime = 1;
    TrackMouseEvent(&tme);
}

bool Mouse::Mouse_IsConnected(void)
{
    return GetSystemMetrics(SM_MOUSEPRESENT) != 0;
}

bool Mouse::Mouse_IsVisible(void)
{
    if (m_Mode == MOUSE_POSITION_MODE_RELATIVE) {
        return false;
    }

    CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
    GetCursorInfo(&info);

    return (info.flags & CURSOR_SHOWING) != 0;
}

void Mouse::Mouse_SetVisible(bool visible)
{
    if (m_Mode == MOUSE_POSITION_MODE_RELATIVE) {
        return;
    }

    CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
    GetCursorInfo(&info);

    bool isVisible = (info.flags & CURSOR_SHOWING) != 0;

    if (isVisible != visible) {
        ShowCursor(visible);
    }
}

void Mouse::Mouse_ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    HANDLE evts[3] = {
        m_ScrollWheelValue,
        m_AbsoluteMode,
        m_RelativeMode
    };

    switch (WaitForMultipleObjectsEx(_countof(evts), evts, FALSE, 0, FALSE))
    {
    case WAIT_OBJECT_0:
        m_State.m_scrollWheelValue = 0;
        ResetEvent(evts[0]);
        break;

    case (WAIT_OBJECT_0 + 1):
    {
        m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;
        ClipCursor(nullptr);

        POINT point;
        point.x = m_LastX;
        point.y = m_LastY;

        // リモートディスクトップに対応するために移動前にカーソルを表示する
        ShowCursor(TRUE);

        if (MapWindowPoints(m_Window, nullptr, &point, 1)) {
            SetCursorPos(point.x, point.y);
        }

        m_State.m_x = static_cast<float>(m_LastX);
        m_State.m_y = static_cast<float>(m_LastY);
    }
    break;

    case (WAIT_OBJECT_0 + 2):
    {
        ResetEvent(m_RelativeRead);

        m_Mode = MOUSE_POSITION_MODE_RELATIVE;
        m_State.m_x = m_State.m_y = 0;
        m_RelativeX = INT32_MAX;
        m_RelativeY = INT32_MAX;

        ShowCursor(FALSE);

        clipToWindow();
    }
    break;

    case WAIT_FAILED:
        return;
    }

    switch (message)
    {
    case WM_ACTIVATEAPP:
        if (wParam) {

            m_InFocus = true;

            if (m_Mode == MOUSE_POSITION_MODE_RELATIVE) {

                m_State.m_x = m_State.m_y = 0;
                ShowCursor(FALSE);
                clipToWindow();
            }
        }
        else {
            int scrollWheel = m_State.m_scrollWheelValue;
            memset(&m_State, 0, sizeof(m_State));
            m_State.m_scrollWheelValue = scrollWheel;
            m_InFocus = false;
        }
        return;

    case WM_INPUT:
        if (m_InFocus && m_Mode == MOUSE_POSITION_MODE_RELATIVE) {

            RAWINPUT raw;
            UINT rawSize = sizeof(raw);

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));

            if (raw.header.dwType == RIM_TYPEMOUSE) {

                if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)) {

                    m_State.m_x = static_cast<float>(raw.data.mouse.lLastX);
                    m_State.m_y = static_cast<float>(raw.data.mouse.lLastY);

                    ResetEvent(m_RelativeRead);
                }
                else if (raw.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) {

                    // リモートディスクトップなどに対応
                    const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                    const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

                    int x = (int)((raw.data.mouse.lLastX / 65535.0f) * width);
                    int y = (int)((raw.data.mouse.lLastY / 65535.0f) * height);

                    if (m_RelativeX == INT32_MAX) {
                        m_State.m_x = m_State.m_y = 0;
                    }
                    else {
                        m_State.m_x = static_cast<float>(x - m_RelativeX);
                        m_State.m_y = static_cast<float>(y - m_RelativeY);
                    }

                    m_RelativeX = x;
                    m_RelativeY = y;

                    ResetEvent(m_RelativeRead);
                }
            }
        }
        return;


    case WM_MOUSEMOVE:
        Mouse::ChangeCursor();
        break;

    case WM_LBUTTONDOWN:
        m_State.m_leftButton = true;
        break;

    case WM_LBUTTONUP:
        m_State.m_leftButton = false;
        break;

    case WM_RBUTTONDOWN:
        m_State.m_rightButton = true;
        break;

    case WM_RBUTTONUP:
        m_State.m_rightButton = false;
        break;

    case WM_MBUTTONDOWN:
        m_State.m_middleButton = true;
        break;

    case WM_MBUTTONUP:
        m_State.m_middleButton = false;
        break;

    case WM_MOUSEWHEEL:
        m_State.m_scrollWheelValue += GET_WHEEL_DELTA_WPARAM(wParam);
        return;

    case WM_XBUTTONDOWN:
        switch (GET_XBUTTON_WPARAM(wParam))
        {
        case XBUTTON1:
            m_State.m_xButton1 = true;
            break;

        case XBUTTON2:
            m_State.m_xButton2 = true;
            break;
        }
        break;

    case WM_XBUTTONUP:
        switch (GET_XBUTTON_WPARAM(wParam))
        {
        case XBUTTON1:
            m_State.m_xButton1 = false;
            break;

        case XBUTTON2:
            m_State.m_xButton2 = false;
            break;
        }
        break;

    case WM_MOUSEHOVER:
        break;

    default:
        // マウスに対するメッセージは無かった…
        return;
    }

    if (m_Mode == MOUSE_POSITION_MODE_ABSOLUTE) {

        // すべてのマウスメッセージに対して新しい座標を取得する
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        // ウィンドウサイズ取得
        RECT rc;
        GetClientRect(m_Window, &rc);
        float windowWidth = static_cast<float>(rc.right - rc.left);
        float windowHeight = static_cast<float>(rc.bottom - rc.top);

        // 論理解像度に変換
        m_State.m_x = (xPos / windowWidth) * SCREEN_WIDTH;
        m_State.m_y = (yPos / windowHeight) * SCREEN_HEIGHT;

        m_LastX = xPos;
        m_LastY = yPos;
    }
}

void Mouse::clipToWindow()
{
    assert(m_Window != NULL);

    RECT rect;
    GetClientRect(m_Window, &rect);

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(m_Window, NULL, &ul, 1);
    MapWindowPoints(m_Window, NULL, &lr, 1);

    rect.left = ul.x;
    rect.top = ul.y;

    rect.right = lr.x;
    rect.bottom = lr.y;

    ClipCursor(&rect);
}

bool Mouse::GetLeftClick()
{
    if (m_clickFrag) {
        return m_State.m_leftButton;
    }
    else {
        return false;
    }
}

bool Mouse::GetLeftClickTrigger()
{
    bool trigger = false;

    // 現在のボタン状態を取得
    bool current = m_State.m_leftButton;

    // 前回が離されていて、今回押されたらトリガー
    if (!m_State.m_oldLeftButton && current && m_clickFrag)
    {
        trigger = true;
    }

    // 状態を更新
    m_State.m_oldLeftButton = current;

    return trigger;
}

void Mouse::ChangeCursor()
{
    m_anyHit = false;

    auto gameObjects = Manager::GetScene()->GetAllGameObject2D();

    for (auto gameObject : gameObjects)
    {
        auto button = gameObject->GetComponent<MouseCollision>();
        if (!button) continue;

        if (button->GetMouseHit()) {
            m_anyHit = true;
        }
    }

    if (m_anyHit) {
        SetCursor(LoadCursor(NULL, IDC_HAND));
    }
    else {
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
}

