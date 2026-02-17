/*===============================================================*/
//
//	branchButton.h(分岐ボタン管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BRANCH_BUTTON_H
#define _BRANCH_BUTTON_H

#include "gameObject.h"
#include <string>
#include "transform.h"
#include "text.h"

class BranchButton : public GameObject
{
private:
	Transform m_transform{};								// トランスフォーム
	std::wstring m_texturePath = L"ボタンテクスチャパス";
	std::wstring m_text = L"ボタン内のテキスト";
	D2D1::ColorF m_textColor = { 0.0f, 0.0f, 0.0f, 0.0f };	// テキストの色

public:
	void SetTransform(const Transform& transform) { m_transform = transform; }
	void SetTexturePath(const std::wstring& texturePath) { m_texturePath = texturePath; }
	void SetText(const std::wstring& text) { m_text = text; }
	void SetTextColor(const XMFLOAT4 textColor) 
	{
		D2D1::ColorF colorF = { textColor.x, textColor.y, textColor.z, textColor.w };
		m_textColor = colorF;
	}

	void Init() override;
	void Uninit() override
	{
		for (auto component : GetComponentVector())
		{
			component->Uninit();
		}
	}

	void Update() override
	{
		for (auto component : GetComponentVector())
		{
			component->Update();
		}
	}

	void Draw() override
	{
		for (auto component : GetComponentVector())
		{
			component->Draw();
		}
	}
};

#endif //_BRANCH_BUTTON_H