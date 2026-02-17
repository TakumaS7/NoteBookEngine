/*===============================================================*/
//
// charaExitOutputComponent.h (キャラ退場コンポーネント)
//
// ---------------------------------- 2026/02/11 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CHARA_EXIT_OUTPUT_COMPONENT_H
#define _CHARA_EXIT_OUTPUT_COMPONENT_H

#include "component.h"

#include <vector>
#include "outputParameter.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "createCharaExit.h"
#include "charaAppearanceOutput.h"
#include "charaAppearanceOutputComponent.h"
#include "textureCharaComponent.h"

class CharaExitOutputComponent : public Component
{
private:
	enum FadeState
	{
		FADE_NONE,	// フェードしてないとき
		FADE_START,	// フェードが始まったとき
		FADE_OUT,	// フェードアウト
		FADE_END,	// フェードが終わったとき
	};

	FadeState m_fadeState = FADE_NONE;
	
	std::vector<OutputParameter> m_parameters;			// クリエイトリスト内の情報を実行順に入れていく
	OutputParameter* m_appearanceParameter = nullptr;   // 該当の名前のパラメーターを参照するためのポインタ

	float m_alpha = 0.0f;   // 画像のα値

	bool m_isFinished = false;   // 全ての状態変更が終えたらtrue

	unsigned int m_currentIndex = 0;    // 現在のパラメーターのインデックス

public:
	void CharaExitIndexSkip(unsigned int skipObjectIndex) 
	{
		bool find = true;   // 一回だけ探す

		/* スキップが走った場合 */
		/* 続きのインデックスから始める必要があるため、キャラ状態変更パラメーター全体から次になるものを探す */
		for (unsigned int i = 0; i < m_parameters.size(); ++i)
		{
			if (m_parameters[i].listIndex >= skipObjectIndex + 1)
			{
				/* falseだったら探さない */
				if (find == false) { return; }

				m_currentIndex = i; // 次に表示する画像のインデックスを代入

				find = false;			// 一回読込処理が走ったら、終了する
				m_isFinished = false;	// 終了フラグをオフにする
			}
		}
	}

	void CharaExitAnim() 
	{
		/* 全て終了したか */
		if (m_isFinished == true) { return; }

		CharaAppearanceOutput* charaAppearanceOutput{};                     // キャラ登場オブジェクト
		CharaAppearanceOutputComponent* charaAppearanceOutputComponent{};   // キャラ登場コンポーネント

		charaAppearanceOutput = Manager::GetDebugScene()->GetGameObject<CharaAppearanceOutput>();

		if (charaAppearanceOutput != nullptr)
		{
			charaAppearanceOutputComponent = charaAppearanceOutput->GetComponent<CharaAppearanceOutputComponent>();
		}

		if (charaAppearanceOutputComponent == nullptr) { return; }

		/* 退場処理 */
		if (m_fadeState == FADE_START)
		{
			m_alpha = 1.0f;	// α値の初期化

			/* どのキャラに向けてか */
			for (OutputParameter& appearanceParameter : charaAppearanceOutputComponent->GetCharaAppearanceParameter())
			{
				/* 該当のキャラを探す */
				if (m_parameters[m_currentIndex].charaName == appearanceParameter.charaName)
				{
					/* キャラが登場していれば退場へ */
					if (appearanceParameter.charaAppearance == false)
					{
						continue;
					}
					else
					{
						/* 登場している場合 */
						m_appearanceParameter = &appearanceParameter;
						m_fadeState = FADE_OUT; // フェードアウトへ
						break;  // 探索終了
					}
				}
			}

			if (m_appearanceParameter == nullptr)
			{
				/* 該当の名前が見つからなかったか、登場していなかった場合 */
				m_fadeState = FADE_END;
				m_currentIndex++;
				return;
			}
		}
		else if (m_fadeState == FADE_OUT)
		{
			if (m_alpha > 0)
			{
				m_alpha -= 0.01f;
				m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);
			}
			else
			{
				/* 退場したら登場フラグをfalseにしてENDへ */
				m_alpha = 0.0f;
				m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);
				m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetDrawable(false);
				m_appearanceParameter = nullptr;

				for (OutputParameter& appearanceParameter : charaAppearanceOutputComponent->GetCharaAppearanceParameter())
				{
					/* 該当のキャラを探す */
					if (m_parameters[m_currentIndex].charaName == appearanceParameter.charaName)
					{
						if (appearanceParameter.charaAppearance == false)
						{
							continue;
						}
						else
						{
							/* 登場している場合、登場フラグをfalseにする */
							m_appearanceParameter = &appearanceParameter;
							m_appearanceParameter->charaAppearance = false;
						}
					}
				}

				m_fadeState = FADE_END;
				m_currentIndex++;
			}
		}

		/* 全て終わったらフィニッシュ */
		if (m_currentIndex >= m_parameters.size())
		{
			m_isFinished = true;
		}
	}

	void LeftClickCharaExit()
	{
		/* フェード開始 */
		if (m_fadeState == FADE_NONE)
		{
			m_fadeState = FADE_START;
		}
	}

	/* フェードがNONEかチェック */
	bool CheckCharaExitFadeNone() const { return m_fadeState == FADE_NONE; }
	/* フェードがENDかチェック */
	bool CheckCharaExitFadeEnd() const { return m_fadeState == FADE_END; }
	/* フェードをEndの次にNoneにセット */
	void  SetCharaExitFadeNone() { m_fadeState = FADE_NONE; }

	void Init() override
	{
		/* リストの取得 */
		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

		if (createList == nullptr) { return; }

		unsigned int i = 0;	// クリエイトリストのインデックス

		/* 各パラメーターの取得 */
		for (ObjectEntry listObject : createList->GetListObjects())
		{
			if (listObject.objectType == ObjectType::CharaExit)
			{
				CreateCharaExit* charaExit = dynamic_cast<CreateCharaExit*>(listObject.listObject);

				if (charaExit != nullptr)
				{
					OutputParameter parameter{};
					parameter.listIndex = i;							// 全体リストのインデックス
					parameter.charaName = charaExit->GetCharaName();	// キャラの名前取得

					/* ここで退場パラメーターを格納する */
					m_parameters.push_back(parameter);
				}
			}

			i++;	// インデックスの加算
		}
	}

	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_CHARA_EXIT_OUTPUT_COMPONENT_H