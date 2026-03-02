/*===============================================================*/
//
// speakTextDisplayOutputComponent.h(テキストフレーム表示コンポーネント)
//
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SPEAK_TEXT_DISPLAY_OUTPUT_COMPONENT
#define _SPEAK_TEXT_DISPLAY_OUTPUT_COMPONENT

#include "component.h"
#include "manager.h"
#include "scene.h"
#include "textBox.h"
#include "textureComponent.h"
#include "nameText.h"
#include "speakText.h"
#include "textComponent.h"
#include "textBoxComponent.h"

class SpeakTextDisplayOutputComponent : public Component
{
private:
	enum DisplayState
	{
		DISPLAY_NONE,	// 表示処理をしてないとき
		DISPLAY_START,	// 表示処理を始めたとき
		DISPLAY_END,	// 表示処理を終えたとき
	};

	const float m_alphaOne = 1.0f;		// テキストフレームの表示用α変数

	TextBox* m_textBox = nullptr;		// テキストボックス
	NameText* m_nameText = nullptr;		// 名前テキスト
	SpeakText* m_speakText = nullptr;	// セリフテキスト

	DisplayState m_displayState = DISPLAY_NONE;	// 非表示処理状態

public:
	/* 非表示処理状態がNONEかチェック */
	bool CheckDisplayNone() const { return m_displayState == DISPLAY_NONE; }
	/* 非表示処理状態がENDかチェック */
	bool CheckDisplayEnd() const { return m_displayState == DISPLAY_END; }
	/* 非表示処理状態をEndの次にNoneにセット */
	void  SetDisplayNone() { m_displayState = DISPLAY_NONE; }

	void SpeakTextDisplayAnim()
	{
		if (m_displayState == DISPLAY_START)
		{
			/* それぞれのオブジェクトを取得 */
			m_textBox = Manager::GetDebugScene()->GetGameObject<TextBox>();
			m_nameText = Manager::GetDebugScene()->GetGameObject<NameText>();
			m_speakText = Manager::GetDebugScene()->GetGameObject<SpeakText>();

			if (m_textBox && m_nameText && m_speakText)
			{
				/* テキストフレームとテキストを非表示にする */
				m_textBox->GetComponent<TextureComponent>()->SetAlpha(m_alphaOne);
				m_nameText->GetComponent<TextComponent>()->SetAlpha(m_alphaOne);
				m_speakText->GetComponent<TextComponent>()->SetAlpha(m_alphaOne);

				/* 表示フラグをONにする */
				m_textBox->GetComponent<TextBoxComponent>()->SetDisplay();

				m_displayState = DISPLAY_END;	// 非表示処理を終える
			}
		}
	}

	void LeftClickSpeakTextDisplay()
	{
		/* 処理を開始する */
		if (m_displayState == DISPLAY_NONE)
		{
			m_displayState = DISPLAY_START;
		}
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {};
};

#endif //_SPEAK_TEXT_DISPLAY_OUTPUT_COMPONENT