/*===============================================================*/
//
//	speakText.h(セリフテキスト管理)
// 
// 
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SPEAK_TEXT_H
#define _SPEAK_TEXT_H

#include "gameObject.h"
#include <string>
#include "textComponent.h"
#include "vector3.h"
#include "transform.h"

class SpeakText : public GameObject
{
private:
	std::wstring m_text;
	Vector3 m_position = { 0.0f, 0.0f, 0.0f };

public:
	void Set(const std::wstring& text, Transform* transform)
	{
		m_text = text;
		TextComponent* textComp = GetComponent<TextComponent>();
		if (textComp) {
			textComp->ChangeText(m_text);	// テキスト更新
		}

		m_position = transform->position;
	}

	const std::wstring& GetText() const { return m_text; }

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

#endif //_SPEAK_TEXT_H