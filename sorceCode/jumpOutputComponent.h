/*===============================================================*/
//
// jumpOutputComponent.h (ジャンプコンポーネント)
//
// ---------------------------------- 2026/02/14 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _JUMP_OUTPUT_COMPONENT_H
#define _JUMP_OUTPUT_COMPONENT_H

#include "component.h"
#include <vector>
#include "outputParameter.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "createRavel.h"
#include "createJump.h"

class JumpOutputComponent : public Component
{
private:
	enum JumpState
	{
		JUMP_NONE,	// ジャンプしていないとき
		JUMP_START,	// ジャンプが始まったとき
		JUMP_END,	// ジャンプが終わったとき
	};

	JumpState m_jumpState = JUMP_NONE;

	bool m_isFinished = false;	// 全部終わったらtrue
	bool m_jumpFrag = false;	// そのフレームでジャンプがあるか

	unsigned int m_targetRavelIndex = 0;	// 目的のラベル名
	unsigned int m_currentIndex = 0;		// 現在のジャンプ内だけのインデックス

	std::vector<OutputParameter> m_parameters{};	// ジャンプオブジェクトごと

public:
	void JumpIndexSkip(unsigned int skipObjectIndex) 
	{
		bool find = true;   // 一回だけ探す

		/* スキップが走った場合 */
		/* 続きのインデックスから始める必要があるため、ジャンプパラメーター全体から次になるものを探す */
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

	void JumpAnim() 
	{
		/* 全て終了したか */
		if (m_isFinished == true) { return; }

		if (m_jumpState == JUMP_START)
		{			
			m_jumpState = JUMP_END;

			m_currentIndex++;
		}

		/* インデックスが最後まで行ったらfinish */
		if (m_currentIndex >= m_parameters.size())
		{
			m_isFinished = true;
		}
	}

	void LeftClickJump() 
	{
		if (m_jumpState == JUMP_NONE)
		{
			m_jumpState = JUMP_START;
		}
	}

	/* ジャンプ状態がNONEかチェック */
	bool CheckJumpStateNone() const { return m_jumpState == JUMP_NONE; }
	/* ジャンプ状態がENDかチェック */
	bool CheckJumpStateEnd() const { return m_jumpState == JUMP_END; }
	/* ジャンプ状態がEndの次にNoneにセット */
	void  SetJumpStateNone() { m_jumpState = JUMP_NONE; }

	/* ジャンプフラグ */
	bool JumpFrag() { return m_jumpFrag; }
	/* ジャンプフラグをOFFにする */
	void JumpFragOff() { m_jumpFrag = false; }

	/* ターゲットラベルのインデックス */
	unsigned int GetTargetRavelIndex() const { return m_targetRavelIndex; }

	/* 該当のラベルのインデックスを返す */
	void SearchTargetRavelIndex()
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
					if (m_parameters[m_currentIndex].targetRavelName == ravel->GetRavelName())
					{
						m_targetRavelIndex = i;
						m_jumpFrag = true;	// ジャンプする
					}
				}
			}

			i++;
		}
	}

	void Init() override 
	{
		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

		if (createList == nullptr) { return; }

		/* ジャンプごとにパラメーターを入れていく */

		unsigned int i = 0;	// クリエイトリストのインデックス

		for (const ObjectEntry& listObject : createList->GetListObjects())
		{
			if (listObject.objectType == ObjectType::Jump)
			{
				CreateJump* jump = dynamic_cast<CreateJump*>(listObject.listObject);

				if (jump != nullptr)
				{
					OutputParameter parameter{};
					parameter.listIndex = i;								// クリエイトリスト内のインデックス
					parameter.targetRavelName = jump->GetTargetRavelName();	// 目的のラベル名

					/* ここで格納する */
					m_parameters.push_back(parameter);
				}
			}

			i++;	// インデックス加算
		}

		if (m_parameters.empty()) { return; }

		/* クリエイトリスト内の先頭にいた場合 */
		if (m_parameters[0].listIndex == 0)
		{
			m_jumpState = JUMP_START;
		}
	}

	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_JUMP_OUTPUT_COMPONENT_H