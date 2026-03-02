/*===============================================================*/
//
// speakTextHiddenOutputComponent.h(テキストフレーム非表示コンポーネント)
//
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SPEAK_TEXT_HIDDEN_OUTPUT_COMPONENT
#define _SPEAK_TEXT_HIDDEN_OUTPUT_COMPONENT

#include "component.h"
#include "manager.h"
#include "scene.h"
#include "textBox.h"
#include "textureComponent.h"
#include "nameText.h"
#include "speakText.h"
#include "textComponent.h"
#include "textBoxComponent.h"

class SpeakTextHiddenOutputComponent : public Component
{
private:
	enum HiddenState
	{
		HIDDEN_NONE,	// 非表示処理をしてないとき
		HIDDEN_START,	// 非表示処理を始めたとき
		HIDDEN_END,		// 非表示処理を終えたとき
	};

	const float m_alphaZero = 0.0f;		// テキストフレームの非表示用α変数

	TextBox* m_textBox = nullptr;		// テキストボックス
	NameText* m_nameText = nullptr;		// 名前テキスト
	SpeakText* m_speakText = nullptr;	// セリフテキスト

	HiddenState m_hiddenState = HIDDEN_NONE;	// 非表示処理状態

public:
	/* 非表示処理状態がNONEかチェック */
	bool CheckHiddenNone() const { return m_hiddenState == HIDDEN_NONE; }
	/* 非表示処理状態がENDかチェック */
	bool CheckHiddenEnd() const { return m_hiddenState == HIDDEN_END; }
	/* 非表示処理状態をEndの次にNoneにセット */
	void  SetHiddenNone() { m_hiddenState = HIDDEN_NONE; }

	void SpeakTextHiddenAnim()
	{
		if (m_hiddenState == HIDDEN_START)
		{
			/* それぞれのオブジェクトを取得 */
			m_textBox = Manager::GetDebugScene()->GetGameObject<TextBox>();
			m_nameText = Manager::GetDebugScene()->GetGameObject<NameText>();
			m_speakText = Manager::GetDebugScene()->GetGameObject<SpeakText>();

			if (m_textBox && m_nameText && m_speakText)
			{
				/* テキストフレームとテキストを非表示にする */
				m_textBox->GetComponent<TextureComponent>()->SetAlpha(m_alphaZero);
				m_nameText->GetComponent<TextComponent>()->SetAlpha(m_alphaZero);
				m_speakText->GetComponent<TextComponent>()->SetAlpha(m_alphaZero);

				/* 表示フラグをOFFにする */
				m_textBox->GetComponent<TextBoxComponent>()->SetHidden();

				m_hiddenState = HIDDEN_END;	// 非表示処理を終える
			}
		}
	}

	void LeftClickSpeakTextHidden()
	{
		/* 処理を開始する */
		if (m_hiddenState == HIDDEN_NONE)
		{
			m_hiddenState = HIDDEN_START;
		}
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {};
};

#endif //_SPEAK_TEXT_HIDDEN_OUTPUT_COMPONENT