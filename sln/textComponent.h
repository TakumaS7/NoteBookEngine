/*===============================================================*/
//
//	textComponent.h(テキスト管理)
// 
// 
// ---------------------------------- 2025/09/27 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TEXT_COMPONENT_H
#define _TEXT_COMPONENT_H

#include "component.h"
#include "text.h"
#include <string>
#include "renderer.h"
#include "transform.h"
#include "vector3.h"

class TextComponent : public Component
{
private:
	Text* m_text{};
	std::wstring m_wstring{};		// テキスト入力内容
	Transform* m_transform{};		// 自身のtransform

	static constexpr float TextSizeSmall() { return 30.0f; }	// テキストサイズを枠より少し小さくする

	bool m_debugFrag = false;							// デバッグウィンドウでの表示か
	D2D1::ColorF m_color = D2D1::ColorF::Black;			// テキストの色
	D2D1::ColorF m_baseColor = D2D1::ColorF::Black;		// テキストの基本色（α値変更用）
	Vector3 m_positionOffset = { 0.0f, 0.0f, 0.0f };	// ポジションの位置変更
	float m_fontSize = 20.0f;							// テキストのサイズ

	TextAlignmentType m_textAlignmentType = TEXT_ALIGNMENT_LEADING;				// 横方向
	ParagraphAlignmentType m_paragraphAlignmentType = PARAGRAPH_ALIGNMENT_NEAR;	// 縦方向

public:
	const std::wstring& GetText() { return m_wstring; }

	void Set(const std::wstring& wstring, Transform* transform, bool debug, D2D1::ColorF color, Vector3 positionOffset,
		float fontSize = 20.0f, TextAlignmentType textAlignmentType = TEXT_ALIGNMENT_LEADING, ParagraphAlignmentType paragraphAlignmentType = PARAGRAPH_ALIGNMENT_NEAR)
	{
		m_wstring = wstring;
		m_transform = transform;
		m_debugFrag = debug;
		m_color = color;
		m_positionOffset = positionOffset;
		m_fontSize = fontSize;
		m_textAlignmentType = textAlignmentType;
		m_paragraphAlignmentType = paragraphAlignmentType;
	}

	/* デバッグウィンドウで表示かのフラグセット */
	void SetDebugFlag(bool debugFrag)
	{
		m_debugFrag = debugFrag;
	}

	/* テキスト更新用 */
	void ChangeText(const std::wstring& str)
	{
		m_wstring = str;
	}

	/* α値変更用 */
	void SetAlpha(float alpha)
	{
		/* テキストの色設定で、プリマルチプライドを使用している為RGBにも乗算する */
		alpha = std::max(alpha, 0.0f);
		alpha = std::min(1.0f, alpha);

		m_color.r = m_baseColor.r * alpha;
		m_color.g = m_baseColor.g * alpha;
		m_color.b = m_baseColor.b * alpha;
		m_color.a = alpha;
	}

	void Init() override 
	{
		m_text = new Text();

		HRESULT hr = m_text->Init(m_debugFrag, m_color, m_fontSize, m_textAlignmentType, m_paragraphAlignmentType);
		if (FAILED(hr)) {
			MessageBox(NULL, "Text 初期化失敗", "Error", MB_OK);
		}

		m_baseColor = m_color;	// 基本色を保存
	}

	void Uninit() override 
	{
		m_text->Uninit();
		delete m_text;
	}

	void Update() override {}

	void Draw() override 
	{
		if (Renderer::GetSwapChain()) {
			m_text->Draw(
				m_wstring.c_str(),
				{ m_transform->position.x + m_positionOffset.x, m_transform->position.y + m_positionOffset.y, m_transform->position.z + m_positionOffset.z },
				{ m_transform->scale.x - TextSizeSmall(),  m_transform->scale.y - TextSizeSmall(), m_transform->scale.z - TextSizeSmall()},
				m_color);
		}
		
		if (Renderer::GetDebugSwapChain()) {
			m_text->Draw(
				m_wstring.c_str(),
				{ m_transform->position.x + m_positionOffset.x, m_transform->position.y + m_positionOffset.y, m_transform->position.z + m_positionOffset.z },
				{ m_transform->scale.x - TextSizeSmall(),  m_transform->scale.y - TextSizeSmall(), m_transform->scale.z - TextSizeSmall() },
				m_color);
		}
	}
};

#endif //_TEXT_COMPONENT_H