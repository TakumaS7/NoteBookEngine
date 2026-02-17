/*===============================================================*/
//
//	createRavel.h(作成ラベル管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/
//	分岐で使用
//
/*===============================================================*/

#ifndef _CREATE_RAVEL_H
#define _CREATE_RAVEL_H

#include "gameObject.h"
#include <string>

class CreateRavel : public GameObject
{
private:
	std::wstring m_ravelName = L"ラベル名";		// ラベル名

	std::wstring m_frameName = L"フレーム表示名";	// ターゲットラベル名、ボタンテキスト、座標、回転、大きさ

public:
	void Set(const std::wstring& ravelName);

	/* 各ゲッター */
	std::wstring GetRavelName() const;	// ラベル名

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

#endif //_CREATE_RAVEL_H