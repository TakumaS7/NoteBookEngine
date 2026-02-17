/*===============================================================*/
//
//	createJump.h(作成ジャンプ管理)
// 
// 
// ---------------------------------- 2026/02/14 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_JUMP_H
#define _CREATE_JUMP_H

#include "gameObject.h"
#include <string>
#include "transform.h"
#include "vector3.h"

class CreateJump : public GameObject
{
private:
	std::wstring m_targetRavelName = L"ラベル名";	// 指定したラベルまでとぶ

	std::wstring m_frameName = L"フレーム表示名";	// ターゲットラベル名

public:
	void Set(const std::wstring& targetRavelName) 
	{
		m_targetRavelName = targetRavelName; 
		m_frameName = L"ジャンプ先ラベル名：" + m_targetRavelName;
	}

	std::wstring GetTargetRavelName() const { return m_targetRavelName; }

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

#endif //_CREATE_JUMP_H