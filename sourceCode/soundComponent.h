/*===============================================================*/
//
//	soundComponent.h(BGM・SE)
// 
// 
// ---------------------------------- 2026/01/29 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SOUND_COMPONENT_H
#define _SOUND_COMPONENT_H

#include "main.h"
#include <string>
#include "component.h"
#include "sound.h"

class SoundComponent : public Component
{
private:
	bool m_loop;	// ループするかどうか
	int m_music;	// ロードした音楽を保管する変数

public:
	/* 音楽をセットする */
	void SetMusic(const std::wstring& pathName, bool loop)
	{
		m_music = LoadSound(pathName.c_str());

		/* ループフラグを適用 */
		m_loop = loop;
	}

	/* 音楽を再生する */
	void SetPlayMusic()
	{
		PlaySoundA(m_music, m_loop);
	}

	/* 音楽の停止 */
	void SetStopMusic()
	{
		StopSoundAll();
	}

	/* UTF-16からUTF-8へ変換関数 */
	std::string WStringToUtf8(const std::wstring& ws)
	{
		if (ws.empty()) return {};

		int size = WideCharToMultiByte(
			CP_UTF8, 0,
			ws.c_str(), -1,
			nullptr, 0,
			nullptr, nullptr
		);

		std::string result(size - 1, 0);
		WideCharToMultiByte(
			CP_UTF8, 0,
			ws.c_str(), -1,
			&result[0], size,
			nullptr, nullptr
		);

		return result;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_SOUND_COMPONENT_H