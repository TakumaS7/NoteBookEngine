/*===============================================================*/
//
//	createCharaExit.h(作成キャラ退場管理)
// 
// 
// ---------------------------------- 2026/02/011 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_CHARA_EXIT_H
#define _CREATE_CHARA_EXIT_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "gameObject.h"
#include <string>
#include <experimental/filesystem>

class CreateCharaExit : public GameObject
{
private:
	std::wstring m_charaName = L"キャラの名前";		// 自分でつけたキャラの名前
	std::wstring m_frameName = L"キャラ画像名";		// キャラの名前

public:
	void Set(const std::wstring& charaName);

	/* 各ゲッター */
	std::wstring GetCharaName() const;		// キャラの名前

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

#endif //_CREATE_CHARA_EXIT_H