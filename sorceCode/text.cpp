/*===============================================================*/
//
//	text.cpp(テキスト処理)
// 
// 
// ---------------------------------- 2025/09/27 製作者：佐藤琢磨
//
/*===============================================================*/

#include "text.h"
#include "renderer.h"
#include "manager.h"


HRESULT Text::Init(bool debug, D2D1::ColorF color, float fontSize, TextAlignmentType textAlignmentType, ParagraphAlignmentType paragraphAlignmentType)
{
    HRESULT hr;

    /* D2DFactory */
    D2D1_FACTORY_OPTIONS options = {};

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory1),
        &options,
        (void**)&m_D2DFactory);

    if (FAILED(hr)) {
        return hr;
    }



    /* IDXGIDeviceを取得 */
    IDXGIDevice* dxgiDevice = nullptr;
    
    hr = Renderer::GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

    if (FAILED(hr)) {
        return hr;
    }



    /* D2DDevice, Context作成 */
    hr = m_D2DFactory->CreateDevice(dxgiDevice, &m_D2DDevice);

    dxgiDevice->Release();  // 参照カウントのリセット

    if (FAILED(hr)) {
        return hr;
    }



    hr = m_D2DDevice->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        &m_D2DContext);

    if (FAILED(hr)) {
        return hr;
    }



    /* DirectWriteFactory */
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        (IUnknown**)&m_DWriteFactory);

    if (FAILED(hr)) {
        return hr;
    }



    /* フォーマットの作成 */

    // 関数CreateTextFormat()
    // 第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
    // 第2引数：フォントコレクション（nullptr）
    // 第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
    // 第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
    // 第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
    // 第6引数：フォントサイズ（20, 30等）
    // 第7引数：ロケール名（L""）
    // 第8引数：テキストフォーマット（&g_pTextFormat）

    hr = m_DWriteFactory->CreateTextFormat(
        L"Meiryo",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"ja-jp",
        &m_TextFormat);

    if (FAILED(hr)) {
        return hr;
    }

    switch (textAlignmentType)
    {
    case TEXT_ALIGNMENT_LEADING:
        m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);  // 左寄せ
        break;
    case TEXT_ALIGNMENT_CENTER:
        m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);   // 中央
        break;
    case TEXT_ALIGNMENT_TRAILING:
        m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING); // 右寄せ
        break;
    }

    switch (paragraphAlignmentType)
    {
    case PARAGRAPH_ALIGNMENT_NEAR:
        m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);   // 左寄せ
        break;
    case PARAGRAPH_ALIGNMENT_CENTER:
        m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // 中央
        break;
    case PARAGRAPH_ALIGNMENT_FAR:
        m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);    // 右寄せ
        break;
    }

    /* バックバッファから、D2Dターゲット作成 */
    IDXGISurface* dxgiBackBuffer = nullptr;

    if (debug) {
        if (GetDebugWindow()) {
            hr = Renderer::GetDebugSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiBackBuffer);
        }
    }
    else {
        if (GetWindow()) {
            hr = Renderer::GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiBackBuffer);
        }    
    }

    if (FAILED(hr)) {
        return hr;
    }



    UINT dpi = GetDpiForWindow(GetWindow()); // DPIを取得（96が標準）

    FLOAT dpiX = static_cast<FLOAT>(dpi);
    FLOAT dpiY = static_cast<FLOAT>(dpi);

    /* 実行画面ではsRGB(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)設定に。それ以外はリニア設定 */
    if (Manager::GetDebugScene())
    {
        D2D1_BITMAP_PROPERTIES1 props =
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
                    D2D1_ALPHA_MODE_PREMULTIPLIED),
                dpiX, dpiY);

        hr = m_D2DContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &props, &m_D2DTarget);
    }
    else
    {
        D2D1_BITMAP_PROPERTIES1 props =
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                    D2D1_ALPHA_MODE_PREMULTIPLIED),
                dpiX, dpiY);

        hr = m_D2DContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &props, &m_D2DTarget);
    }

    dxgiBackBuffer->Release();  // 参照カウントのリセット

    if (FAILED(hr)) {
        return hr;
    }

    m_D2DContext->SetTarget(m_D2DTarget);



    /* ブラシ作成 */
    hr = m_D2DContext->CreateSolidColorBrush(D2D1::ColorF(color), &m_Brush);

    if (FAILED(hr)) {
        return hr;
    }

    return S_OK;
}



void Text::Uninit()
{
    if (m_Brush) { 
        m_Brush->Release(); 
        m_Brush = nullptr; 
    }

    if (m_TextFormat) { 
        m_TextFormat->Release();
        m_TextFormat = nullptr;
    }

    if (m_DWriteFactory) {
        m_DWriteFactory->Release();
        m_DWriteFactory = nullptr;
    }

    if (m_D2DTarget) 
    { 
        m_D2DTarget->Release();
        m_D2DTarget = nullptr;
    }

    if (m_D2DContext) {
        m_D2DContext->Release();
        m_D2DContext = nullptr;
    }

    if (m_D2DDevice) {
        m_D2DDevice->Release();
        m_D2DDevice = nullptr;
    }

    if (m_D2DFactory) {
        m_D2DFactory->Release();
        m_D2DFactory = nullptr;
    }
}



void Text::Draw(const wchar_t* text, Vector3 position, Vector3 size)
{
    m_D2DContext->BeginDraw();

    D2D1_RECT_F layoutRect = D2D1::RectF(position.x - size.x, position.y - size.y, position.x + size.x, position.y + size.y); // 左・上・右・下

    m_D2DContext->DrawText(
        text,
        (UINT32)wcslen(text),
        m_TextFormat,
        &layoutRect,
        m_Brush);

    HRESULT hr = m_D2DContext->EndDraw();

    // エラー処理（リサイズで描画先が無効になる場合あり）
    if (FAILED(hr)){
        MessageBox(NULL, "Text::Drawでエラー発生", "Error", MB_OK);
    }
}
