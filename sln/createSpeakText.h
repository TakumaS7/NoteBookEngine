/*===============================================================*/
//
//	createSpeakText.h(作成セリフテキスト管理)
// 
// 
// ---------------------------------- 2025/09/28 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_SPEAK_TEXT_H
#define _CREATE_SPEAK_TEXT_H

#include "gameObject.h"
#include <string>
#include "vector3.h"

class CreateSpeakText : public GameObject
{
private:
	std::wstring m_text = L"これはサンプルである。名前はまだない。吾輩はサンプルである。";
	std::wstring m_name = L"名前";
	Vector3 m_nameOffset = { 0.0f, -20.0f, 0.0f };	// セリフを言っている人の名前を表示する場所調整

public:
	void SetText(const std::wstring& text) { m_text = text; }
	const std::wstring& GetText() const { return m_text; }

	void SetName(const std::wstring& name) { m_name = name; }
	const std::wstring& GetName() const { return m_name; }

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

#endif //_CREATE_SPEAK_TEXT_H