/*===============================================================*/
//
//	createBgTexture.h(作成背景画像管理)
// 
// 
// ---------------------------------- 2025/12/04 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_BG_TEXTURE_H
#define _CREATE_BG_TEXTURE_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "gameObject.h"
#include <string>
#include <experimental/filesystem>

class CreateBgTexture : public GameObject
{
private:
	std::wstring m_textureName = L"これはサンプルである。名前はまだない。吾輩はサンプルである。";	// パス全体
	std::wstring m_frameName = L"これはサンプルである。名前はまだない。吾輩はサンプルである。";		// ファイル名のみ

public:
	void Set(const std::wstring& text) { 
		m_textureName = text;
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

	std::wstring GetTexture() const;
};

#endif //_CREATE_BG_TEXTURE_H