/*===============================================================*/
//
//	deleteSpeakText.h(セリフテキスト削除管理)
// 
// 
// ---------------------------------- 2025/10/04 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _DELETE_SPEAK_TEXT_H
#define _DELETE_SPEAK_TEXT_H

#include "gameObject.h"

class DeleteSpeakText : public GameObject
{
private:
	GameObject* m_deleteObject{};

public:
	void Set(GameObject* deleteObject) { m_deleteObject = deleteObject; }

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

#endif //_DELETE_SPEAK_TEXT_H