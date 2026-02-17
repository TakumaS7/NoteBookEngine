/*===============================================================*/
//
//	createSe.h(作成BGM管理)
// 
// 
// ---------------------------------- 2026/02/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_SE_H
#define _CREATE_SE_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "gameObject.h"
#include <string>
#include <experimental/filesystem>

class CreateSe : public GameObject
{
private:
	std::wstring m_seName = L"これはサンプルである。名前はまだない。吾輩はサンプルである。";	// パス全体
	std::wstring m_frameName = L"これはサンプルである。名前はまだない。吾輩はサンプルである。";	// ファイル名のみ

public:
	void Set(const std::wstring& text) {
		m_seName = text;
		m_frameName = std::experimental::filesystem::path(text).filename().wstring();
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

	std::wstring GetSe() const;
};

#endif //_CREATE_SE_H