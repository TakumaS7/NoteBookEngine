/*===============================================================*/
//
// branchButtonOutputComponent.h(分岐ボタンコンポーネント)
//
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BRANCH_BUTTON_OUTPUT_COMPONENT_H
#define _BRANCH_BUTTON_OUTPUT_COMPONENT_H

#include "component.h"
#include <vector>
#include "outputParameter.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "createBranchButton.h"
#include "branchButton.h"
#include "transform.h"
#include "textureComponent.h"
#include "textComponent.h"
#include "branchButtonComponent.h"
#include "createRavel.h"

class BranchButtonOutputComponent : public Component
{
private:
	/* 表示状態 */
	enum DisplayState
	{
		DISPLAY_NONE,			// 表示が始まっていないとき
		DISPLAY_START,			// 表示が始まったとき
		DISPLAY_END,			// 表示し終えたとき
	};

	DisplayState m_displayState = DISPLAY_NONE;

	unsigned int m_currentIndex = 0;	// 現在の分岐ボタンインデックス

	std::vector<OutputParameter> m_parameters{};	// ボタンごと

	bool m_firstDisplay = false;	// 最初の表示が始まったか
	bool m_firstDisplayEnd = false;	// 最初のボタン表示が終えたか

	bool m_isFinished = false; // m_currentIndexが最後まで行ったらtrueで終了

	bool m_buttonPush = false;			// ボタンが押されたらtrue
	unsigned int m_targetRavelIndex{};	// 押されたボタンのターゲットラベルのインデックス

public:
	void BranchButtonIndexSkip(unsigned int skipObjectIndex) 
	{
		bool find = true;   // 一回だけ探す

		/* スキップが走った場合 */
		/* 続きのインデックスから始める必要があるため、分岐ボタンパラメーター全体から次になるものを探す */
		for (unsigned int i = 0; i < m_parameters.size(); ++i)
		{
			if (m_parameters[i].listIndex >= skipObjectIndex + 1)
			{
				/* falseだったら探さない */
				if (find == false) { return; }

				m_currentIndex = i; // 次に表示するボタンのインデックスを代入

				find = false;			// 一回読込処理が走ったら、終了する
				m_isFinished = false;	// 終了フラグをオフにする
			}
		}
	}

	void BranchButtonAnim() 
	{
		/* ボタンが押されたかの確認 */
		BranchButtonPush();

		/* 全て終了したか */
		if (m_isFinished == true) { return; }

		/* DISPLAY_STARTで表示開始 */
		if (m_displayState == DISPLAY_START)
		{
			BranchButtonDisplay();	// ボタンの表示

			m_displayState = DISPLAY_END;

			/* はじめのフェードのみ */
			if (m_firstDisplay == true)
			{
				m_firstDisplay = false;
				m_firstDisplayEnd = true;
			}

			m_currentIndex++;
		}

		/* インデックスが最後まで行ったらfinish */
		if (m_currentIndex >= m_parameters.size())
		{
			m_isFinished = true;
		}
	}

	void LeftClickBranchButton() 
	{
		if (m_firstDisplay == false && m_firstDisplayEnd == false && m_displayState == DISPLAY_NONE)
		{
			m_firstDisplay = true;
			m_displayState = DISPLAY_START;   // 表示開始
		}
		else if (m_displayState == DISPLAY_NONE)
		{
			m_displayState = DISPLAY_START;   // 表示開始
		}
	}

	/* 表示状態がNONEかチェック */
	bool CheckBranchButtonDisplayNone() const { return m_displayState == DISPLAY_NONE; }
	/* 表示状態がENDかチェック */
	bool CheckBranchButtonDisplayEnd() const { return m_displayState == DISPLAY_END; }
	/* 表示状態がEndの次にNoneにセット */
	void  SetBranchButtonDisplayNone() { m_displayState = DISPLAY_NONE; }

	bool GetBranchButtonFirstDisplayEnd() const { return m_firstDisplayEnd; }

	/* ボタン押下フラグ */
	bool GetButtonPush() const { return m_buttonPush; }
	/* ターゲットラベルのインデックス */
	unsigned int GetTargetRavelIndex() const { return m_targetRavelIndex; }
	
	/* 表示処理 */
	void BranchButtonDisplay()
	{
		/* 分岐ボタンの生成 */
		BranchButton* branchButton = Manager::GetDebugScene()->AddGameObject<BranchButton>(OBJECT_2D);

		branchButton->SetTransform(m_parameters[m_currentIndex].transform);
		branchButton->SetTexturePath(m_parameters[m_currentIndex].path);
		branchButton->SetText(m_parameters[m_currentIndex].text);
		branchButton->SetTextColor(m_parameters[m_currentIndex].color);
		branchButton->Init();

		m_parameters[m_currentIndex].branchButton = branchButton;	// 最後に分岐ボタン情報をまとめて格納
	}

	/* 押したかのフラグ（押したらtrueへ） */
	void BranchButtonPush()
	{
		m_buttonPush = false;	// 探索前に値を初期化

		for (const OutputParameter& parameter : m_parameters)
		{
			/* 既に生成されているボタンのなかで押されたボタンがあればtrue */
			if (parameter.branchButton != nullptr)
			{
				if (parameter.branchButton->GetComponent<BranchButtonComponent>()->ButtonPush() == true)
				{
					m_buttonPush = true;										// 押下フラグをtrue
					std::wstring targetRavelName = parameter.targetRavelName;	// パラメーターの取得

					SearchTargetRavelIndex(targetRavelName);	// ターゲットラベルのインデックスを取得
				}
			}
		}
	}

	/* 該当のラベルのインデックスを返す */
	void SearchTargetRavelIndex(std::wstring targetName)
	{
		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

		if (createList == nullptr) { return; }

		/* ラベルからターゲットを探す */

		unsigned int i = 0;	// クリエイトリスト内のインデックス

		for (const ObjectEntry& listObject : createList->GetListObjects())
		{
			if (listObject.objectType == ObjectType::Ravel)
			{
				CreateRavel* ravel = dynamic_cast<CreateRavel*>(listObject.listObject);

				if (ravel != nullptr)
				{
					/* 該当するラベル名が見つかったら、そのラベルのインデックスを取得する */
					if (targetName == ravel->GetRavelName())
					{
						m_targetRavelIndex = i;
					}
				}
			}

			i++;
		}
	}

	/* 今表示されているボタンを削除する */
	void DisplayButtonAllDelete()
	{
		for (OutputParameter& parameter : m_parameters)
		{
			if (parameter.branchButton != nullptr)
			{
				parameter.branchButton->SetDestroy();
				parameter.branchButton = nullptr;
			}
		}
	}

	void Init() override 
	{
		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

		if (createList == nullptr) { return; }

		/* ボタンごとにパラメーターを入れていく */

		unsigned int i = 0;	// クリエイトリストのインデックス

		for (const ObjectEntry& listObject : createList->GetListObjects())
		{
			if (listObject.objectType == ObjectType::BranchButton)
			{
				CreateBranchButton* branchButton = dynamic_cast<CreateBranchButton*>(listObject.listObject);

				if (branchButton != nullptr)
				{
					OutputParameter parameter{};
					parameter.listIndex = i;										// クリエイトリスト内のインデックス
					parameter.path = branchButton->GetTexturePath();				// 画像パス
					parameter.text = branchButton->GetButtonText();					// ボタンに表示されるテキスト
					parameter.transform = branchButton->GetTransform();				// ボタンのトランスフォーム
					parameter.color.x = branchButton->GetTextColor().x;				// 色（R）
					parameter.color.y = branchButton->GetTextColor().y;				// 色（G）
					parameter.color.z = branchButton->GetTextColor().z;				// 色（B）
					parameter.color.w = 1.0f;										// α値は固定
					parameter.targetRavelName = branchButton->GetTargetRavelName();	// 目的のラベル

					/* ここでパラメーターを格納する */
					m_parameters.push_back(parameter);
				}
			}

			i++;	// インデクス加算
		}

		if (m_parameters.empty()) { return; }

		/* クリエイトリストの中でも、先頭だったら */
		if (m_parameters[0].listIndex == 0)
		{
			m_firstDisplay = true;
			m_displayState = DISPLAY_START;	//表示状態を開始へ
		}
	}

	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_BRANCH_BUTTON_OUTPUT_COMPONENT_H