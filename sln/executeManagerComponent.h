/*===============================================================*/
//
// executeManagerComponent.h(実行コンポーネント管理)
//
// ---------------------------------- 2025/12/17 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _EXECUTE_COMPONENT_H
#define _EXECUTE_COMPONENT_H

#include "component.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include <vector>

#include "bgTextureOutput.h"
#include "charaAppearanceOutput.h"
#include "charaChangeOutput.h"
#include "charaExitOutput.h"
#include "textBox.h"
#include "bgmOutput.h"
#include "seOutput.h"
#include "gameStopOutput.h"
#include "branchButtonOutput.h"
#include "jumpOutput.h"

#include "bgTextureOutputComponent.h"
#include "charaAppearanceOutputComponent.h"
#include "charaChangeOutputComponent.h"
#include "charaExitOutputComponent.h"
#include "textBoxComponent.h"
#include "bgmOutputComponent.h"
#include "seOutputComponent.h"
#include "gameStopOutputComponent.h"
#include "branchButtonOutputComponent.h"
#include "jumpOutputComponent.h"

#include "gameObject.h"
#include "mouse.h"

class ExecuteManagerComponent : public Component
{
private:
    std::vector<ObjectEntry> m_listObjects{}; // エディタ上のオブジェクト情報格納

    bool isFinished = false;    // 終了フラグ

    BgTextureOutput* m_bgTextureOutput;             // 背景
    CharaAppearanceOutput* m_charaAppearanceOutput; // キャラ登場
    CharaChangeOutput* m_charaChangeOutput;         // キャラ状態変更
    CharaExitOutput* m_charaExitOutput;             // キャラ退場
    TextBox* m_textBox;                             // テキスト
    BgmOutput* m_bgmOutput;                         // BGM
    SeOutput* m_seOutput;                           // SE
    GameStopOutput* m_gameStopOutput;               // ゲーム停止
    BranchButtonOutput* m_brachButtonOutput;        // 分岐ボタン
    JumpOutput* m_jumpOutput;                       // ジャンプ

    BgTextureOutputComponent* m_bgTextureOutputComponent;               // 背景
    CharaAppearanceOutputComponent* m_charaAppearanceOutputComponent;   // キャラ登場
    CharaChangeOutputComponent* m_charaChangeOutputComponent;           // キャラ状態変更
    CharaExitOutputComponent* m_charaExitOutputComponent;               // キャラ退場
    TextBoxComponent* m_textBoxComponent;                               // テキスト
    BgmOutputComponent* m_bgmOutputComponent;                           // BGM
    SeOutputComponent* m_seOutputComponent;                             // SE
    GameStopOutputComponent* m_gameStopOutputComponent;                 // ゲーム停止
    BranchButtonOutputComponent* m_brachButtonOutputComponent;          // 分岐ボタン
    JumpOutputComponent* m_jumpOutputComponent;                         // ジャンプ

    unsigned int m_currentIndex = 0;    // 現在のインデックス（今何を表示しているかの管理）

    bool m_manualAddindex = true;   // 左クリックする際のリストインデックス加算フラグ
    bool m_autoAddindex = true;     // 自動アニメーションの際のリストインデックス加算フラグ
    bool m_textFullShow = true;     // テキスト全表示フラグ
    bool m_autoAnim = false;        // オートで動くフラグ（背景やキャラの自動クロスフェード）

public:
    /* インデックスが一気に飛ぶ際に使用（例：分岐する選択肢） */
    void SkipIndex(unsigned int index)
    {
        m_textBoxComponent->TextIndexSkip(index);
        m_bgTextureOutputComponent->BgTextureIndexSkip(index);
        m_charaAppearanceOutputComponent->CharaAppearanceIndexSkip(index);
        m_charaChangeOutputComponent->CharaChangeIndexSkip(index);
        m_charaExitOutputComponent->CharaExitIndexSkip(index);
        m_bgmOutputComponent->BgmIndexSkip(index);
        m_seOutputComponent->SeIndexSkip(index);
        m_brachButtonOutputComponent->BranchButtonIndexSkip(index);
        m_jumpOutputComponent->JumpIndexSkip(index);
        m_currentIndex = index + 1;

    }

    void ExecuteLeftClick()
    {
        /* オブジェクトごとにタイプを判別して処理分岐を行う */
        if (m_listObjects[m_currentIndex].objectType == ObjectType::SpeakText)
        {
            m_textBoxComponent->LeftClickText();
            m_autoAnim = false;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::BgTexture)
        {
            m_bgTextureOutputComponent->LeftClickBgTexture();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::CharaAppearance)
        {
            m_charaAppearanceOutputComponent->LeftClickCharaAppearance();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::CharaChange)
        {
            m_charaChangeOutputComponent->LeftClickCharaChange();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::CharaExit)
        {
            m_charaExitOutputComponent->LeftClickCharaExit();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::BGM)
        {
            m_bgmOutputComponent->LeftClickBgm();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::SE)
        {
            m_seOutputComponent->LeftClickSe();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::GameStop)
        {
            m_gameStopOutputComponent->LeftClickGameStop();
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::Ravel)
        {
            /* ラベルは何もしない */
            m_currentIndex++;   // インデックスを増加
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::BranchButton)
        {
            m_brachButtonOutputComponent->LeftClickBranchButton();
            m_autoAnim = true;
        }
        else if (m_listObjects[m_currentIndex].objectType == ObjectType::Jump)
        {
            m_jumpOutputComponent->LeftClickJump();
            m_jumpOutputComponent->SearchTargetRavelIndex();
            m_autoAnim = true;
        }
    }

    void Init() override
    {
        /* エディタ上のオブジェクトを取得 */
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

        /* デバッグに必要なオブジェクト */
        m_textBox = Manager::GetDebugScene()->AddGameObject<TextBox>(OBJECT_2D);
        m_bgTextureOutput = Manager::GetDebugScene()->AddGameObject<BgTextureOutput>(OBJECT_2D);
        m_charaAppearanceOutput = Manager::GetDebugScene()->AddGameObject<CharaAppearanceOutput>(OBJECT_2D);
        m_charaChangeOutput = Manager::GetDebugScene()->AddGameObject<CharaChangeOutput>(OBJECT_2D);
        m_charaExitOutput = Manager::GetDebugScene()->AddGameObject<CharaExitOutput>(OBJECT_2D);
        m_bgmOutput = Manager::GetDebugScene()->AddGameObject<BgmOutput>(OBJECT_2D);
        m_seOutput = Manager::GetDebugScene()->AddGameObject<SeOutput>(OBJECT_2D);
        m_gameStopOutput = Manager::GetDebugScene()->AddGameObject<GameStopOutput>(OBJECT_2D);
        m_brachButtonOutput = Manager::GetDebugScene()->AddGameObject<BranchButtonOutput>(OBJECT_2D);
        m_jumpOutput = Manager::GetDebugScene()->AddGameObject<JumpOutput>(OBJECT_2D);

        if (createList) {

            /* リスト型からベクター型への移行 */
            for (ObjectEntry& listObject : createList->GetListObjects())
            {
                m_listObjects.push_back(listObject);
            }
        }


        m_manualAddindex = true;
        m_textFullShow = true;
    }

    void Uninit() override {}

    void Update() override
    {
        /* 全ての処理が終わったら停止 */
        if (isFinished) { 
            return; 
        }

        /* 全て終わったら停止 */
        if (m_currentIndex >= static_cast<int>(m_listObjects.size()))
        {
            isFinished = true;
            return;
        }

        /* 必要なコンポーネントの取得 */
        m_bgTextureOutputComponent = m_bgTextureOutput->GetComponent<BgTextureOutputComponent>();
        m_charaAppearanceOutputComponent = m_charaAppearanceOutput->GetComponent<CharaAppearanceOutputComponent>();
        m_charaChangeOutputComponent = m_charaChangeOutput->GetComponent<CharaChangeOutputComponent>();
        m_charaExitOutputComponent = m_charaExitOutput->GetComponent<CharaExitOutputComponent>();
        m_textBoxComponent = m_textBox->GetComponent<TextBoxComponent>();
        m_bgmOutputComponent = m_bgmOutput->GetComponent<BgmOutputComponent>();
        m_seOutputComponent = m_seOutput->GetComponent<SeOutputComponent>();
        m_gameStopOutputComponent = m_gameStopOutput->GetComponent<GameStopOutputComponent>();
        m_brachButtonOutputComponent = m_brachButtonOutput->GetComponent<BranchButtonOutputComponent>();
        m_jumpOutputComponent = m_jumpOutput->GetComponent<JumpOutputComponent>();


        /* テキストがある場合だけ全文表示チェック */
        if (m_listObjects[m_currentIndex].objectType == ObjectType::SpeakText)
        {
            m_textFullShow = m_textBoxComponent->GetIsTextFullyShown();
        }

        /* 常にチェック */
        m_manualAddindex = m_textFullShow &&
            m_bgTextureOutputComponent->CheckBgCrossfFadeNone() == true &&              // クロスフェードしていない状態に戻ったか
            m_charaAppearanceOutputComponent->CheckCharaAppearanceFadeNone() == true && // フェードしていない状態に戻ったか
            m_charaChangeOutputComponent->CheckCharaChangeFadeNone() == true &&         // フェードしていない状態に戻ったか
            m_charaExitOutputComponent->CheckCharaExitFadeNone() == true &&             // フェードしていない状態に戻ったか
            m_bgmOutputComponent->CheckBgmLoadNone() == true &&                         // ロードしていない状態に戻ったか
            m_seOutputComponent->CheckSeLoadNone() == true &&                           // ロードしていない状態に戻ったか
            m_gameStopOutputComponent->GetGameStop() == false &&                        // ゲーム停止していない状態に戻ったか
            m_brachButtonOutputComponent->CheckBranchButtonDisplayNone() == true &&     // なにもしていない状態に戻ったか
            m_jumpOutputComponent->CheckJumpStateNone() == true;                        // なにもしていない状態に戻ったか

        m_autoAddindex = m_textFullShow &&
            m_gameStopOutputComponent->GetGameStop() == false &&                            // ゲーム停止していない状態に戻ったか
            (m_bgTextureOutputComponent->CheckBgCrossfFadeEnd() == true ||                  // クロスフェードが終わった状態になったか
                m_charaAppearanceOutputComponent->CheckCharaAppearanceFadeEnd() == true ||  // フェードが終わった状態になったか
                m_charaChangeOutputComponent->CheckCharaChangeFadeEnd() == true ||          // フェードが終わった状態になったか
                m_charaExitOutputComponent->CheckCharaExitFadeEnd() == true ||              // フェードが終わった状態になったか
                m_bgmOutputComponent->CheckBgmLoadEnd() == true ||                          // ロードが終わった状態になったか
                m_seOutputComponent->CheckSeLoadEnd() == true ||                            // ロードが終わった状態になったか
                m_brachButtonOutputComponent->CheckBranchButtonDisplayEnd() == true ||      // 表示が終わった状態になったか
                m_jumpOutputComponent->CheckJumpStateEnd() == true);                        // ジャンプが終わった状態になったか


        /* 最初の背景・キャラの登場する際、BGMを流す際はオートフラグtrue */
        if (m_listObjects[m_currentIndex].objectType == ObjectType::BgTexture &&
            m_bgTextureOutputComponent->GetBgFirstLoadEnd() == true ||
            m_listObjects[m_currentIndex].objectType == ObjectType::CharaAppearance &&
            m_charaAppearanceOutputComponent->GetCharaAppearanceFirstFadeEnd() == true ||
            m_listObjects[m_currentIndex].objectType == ObjectType::BGM &&
            m_bgmOutputComponent->GetBgmFirstLoadEnd() == true ||
            m_listObjects[m_currentIndex].objectType == ObjectType::SE &&
            m_seOutputComponent->GetSeFirstLoadEnd() == true ||
            m_listObjects[m_currentIndex].objectType == ObjectType::Ravel ||
            m_listObjects[m_currentIndex].objectType == ObjectType::BranchButton &&
            m_brachButtonOutputComponent->GetBranchButtonFirstDisplayEnd() == true ||
            m_listObjects[m_currentIndex].objectType == ObjectType::Jump)
            {
            m_autoAnim = true;
        }

        /* オートフラグが立っていなかったら左クリック処理 */
        if (Mouse::GetLeftClickTrigger() && m_autoAnim == false)
        {
            if (m_manualAddindex)
            {
                /* テクスチャがリストの先頭の場合、最初のフェード処理が終わっているなら */
                if (m_listObjects[m_currentIndex].objectType == ObjectType::BgTexture &&
                    m_bgTextureOutputComponent->GetBgFirstLoadEnd() == false)
                {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::CharaAppearance &&
                    m_charaAppearanceOutputComponent->GetCharaAppearanceFirstFadeEnd() == false)
                {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::BGM &&
                    m_bgmOutputComponent->GetBgmFirstLoadEnd() == false)
                {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::SE &&
                    m_seOutputComponent->GetSeFirstLoadEnd() == false)
                {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::BranchButton &&
                    m_brachButtonOutputComponent->GetBranchButtonFirstDisplayEnd() == false)
                {
                    return;
                }

                m_currentIndex++;   // インデックスを増加
            }

            /* ラベルへ飛ぶ処理 */
            if (m_brachButtonOutputComponent->GetButtonPush() == true)
            {
                SkipIndex(m_brachButtonOutputComponent->GetTargetRavelIndex()); // インデックスを飛ばす
                m_gameStopOutputComponent->SetGameStopOff();                    // ストップを解除する
                m_brachButtonOutputComponent->DisplayButtonAllDelete();         // ボタンを消す
            }
            else if (m_jumpOutputComponent->JumpFrag() == true)
            {
                SkipIndex(m_jumpOutputComponent->GetTargetRavelIndex()); // インデックスを飛ばす
                m_jumpOutputComponent->JumpFragOff();
            }

            /* 全て終わったら停止 */
            if (m_currentIndex >= static_cast<int>(m_listObjects.size()))
            {
                isFinished = true;
                return;
            }

            /* オブジェクトごとにタイプを判別して処理分岐を行う */
            ExecuteLeftClick();
        }
        else if (m_autoAnim == true)
        {
            /* 画像のクロスフェードが終わったら　かつ　背景・キャラフェードが終わっているなら */
            if (m_autoAddindex)
            {
                /* テクスチャがリストの先頭の場合、最初のフェード処理が終わっているなら */
                if (m_listObjects[m_currentIndex].objectType == ObjectType::BgTexture &&
                    m_bgTextureOutputComponent->GetBgFirstLoadEnd() == false) {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::CharaAppearance &&
                    m_charaAppearanceOutputComponent->GetCharaAppearanceFirstFadeEnd() == false) {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::BGM &&
                    m_bgmOutputComponent->GetBgmFirstLoadEnd() == false) {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::SE &&
                    m_seOutputComponent->GetSeFirstLoadEnd() == false) {
                    return;
                }
                else if (m_listObjects[m_currentIndex].objectType == ObjectType::BranchButton &&
                    m_brachButtonOutputComponent->GetBranchButtonFirstDisplayEnd() == false)
                {
                    return;
                }

                m_bgTextureOutputComponent->SetBgCrossFadeNone();               // フェード状態をNONEへリセット
                m_charaAppearanceOutputComponent->SetCharaAppearanceFadeNone(); // フェード状態をNONEへリセット
                m_charaChangeOutputComponent->SetCharaChangeFadeNone();         // フェード状態をNONEへリセット
                m_charaExitOutputComponent->SetCharaExitFadeNone();             // フェード状態をNONEへリセット
                m_bgmOutputComponent->SetBgmLoadNone();                         // ロード状態をNONEへリセット
                m_seOutputComponent->SetSeLoadNone();                           // ロード状態をNONEへリセット
                m_brachButtonOutputComponent->SetBranchButtonDisplayNone();     // 表示状態をNONEへリセット
                m_jumpOutputComponent->SetJumpStateNone();                      // ジャンプ状態をNONEへリセット
                m_currentIndex++;                                               // インデックスを増加
            }

            /* ラベルへ飛ぶ処理 */
            if (m_brachButtonOutputComponent->GetButtonPush() == true)
            {
                SkipIndex(m_brachButtonOutputComponent->GetTargetRavelIndex()); // インデックスを飛ばす
                m_gameStopOutputComponent->SetGameStopOff();                    // ストップを解除する
                m_brachButtonOutputComponent->DisplayButtonAllDelete();         // ボタンを消す
            }
            else if (m_jumpOutputComponent->JumpFrag() == true)
            {
                SkipIndex(m_jumpOutputComponent->GetTargetRavelIndex()); // インデックスを飛ばす
                m_jumpOutputComponent->JumpFragOff();
            }

            /* 全て終わったら停止 */
            if (m_currentIndex >= static_cast<int>(m_listObjects.size()))
            {
                isFinished = true;
                return;
            }

            /* オブジェクトごとにタイプを判別して処理分岐を行う */
            ExecuteLeftClick();
        }

        /* 動作処理 */
        m_bgTextureOutputComponent->BgTextureAnim();
        m_charaAppearanceOutputComponent->CharaAppearanceAnim();
        m_charaChangeOutputComponent->CharaChangeAnim();
        m_charaExitOutputComponent->CharaExitAnim();
        m_textBoxComponent->TextAnim();
        m_bgmOutputComponent->BgmAnim();
        m_seOutputComponent->SeAnim();
        m_brachButtonOutputComponent->BranchButtonAnim();
        m_jumpOutputComponent->JumpAnim();
    }

    void Draw() override {}
};

#endif //_EXECUTE_COMPONENT_H
