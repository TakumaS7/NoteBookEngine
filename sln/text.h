/*===============================================================*/
//
//	text.h(テキスト処理)
// 
// 
// ---------------------------------- 2025/09/27 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TEXT_H
#define _TEXT_H

#include "main.h"
#include "vector3.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d2d1_1.h>
#include <dwrite.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

enum TextAlignmentType
{
	/* 横 */
	TEXT_ALIGNMENT_LEADING,		// 左寄せ
	TEXT_ALIGNMENT_CENTER,		// 中央
	TEXT_ALIGNMENT_TRAILING,	// 右寄せ
};

enum ParagraphAlignmentType
{
	/* 縦 */
	PARAGRAPH_ALIGNMENT_NEAR,	// 上寄せ
	PARAGRAPH_ALIGNMENT_CENTER,	// 中央
	PARAGRAPH_ALIGNMENT_FAR,	// 下寄せ
};

class Text
{
private:
	/* Direct2D */
	ID2D1Factory1* m_D2DFactory = nullptr;
	ID2D1Device* m_D2DDevice = nullptr;
	ID2D1DeviceContext* m_D2DContext = nullptr;
	ID2D1SolidColorBrush* m_Brush = nullptr;
	ID2D1Bitmap1* m_D2DTarget = nullptr;

	/* DirectWrite */
	IDWriteFactory* m_DWriteFactory = nullptr;
	IDWriteTextFormat* m_TextFormat = nullptr;

	TextAlignmentType m_textAlignmentType = TEXT_ALIGNMENT_LEADING;
	ParagraphAlignmentType m_paragraphAlignmentType = PARAGRAPH_ALIGNMENT_NEAR;

public:
	HRESULT Init(bool debug = false, D2D1::ColorF color = D2D1::ColorF::Black, float fontSize = 20.0f, TextAlignmentType textAlignmentType = TEXT_ALIGNMENT_LEADING, ParagraphAlignmentType paragraphAlignmentType = PARAGRAPH_ALIGNMENT_NEAR);
	void Uninit();
	void Draw(const wchar_t* text, Vector3 position, Vector3 size, D2D1::ColorF color);
};

#endif //_TEXT_H