/*===============================================================*/
//
// charaAppearanceOutputComponent.h (キャラ登場コンポーネント)
//
// ---------------------------------- 2026/02/05 製作者：佐藤琢磨
//
/*===============================================================*/
//メモ
// キャラが登場していない場合は、該当するキャラ名の「キャラ変更」を
// 無視すること。
//
/*===============================================================*/

#ifndef _CHARA_APPEARANCE_OUTPUT_COMPONENT_H
#define _CHARA_APPEARANCE_OUTPUT_COMPONENT_H

#include "component.h"
#include <vector>
#include <string>
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "createCharaAppearance.h"
#include "charaTexture.h"
#include "textureCharaComponent.h"
#include "outputParameter.h"

class CharaAppearanceOutputComponent : public Component
{
private:
    /* キャラのフェード状態 */
    enum FadeState
    {
        FADE_NONE,    // フェードしてないとき
        FADE_START,   // フェードが始まったとき
        FADE_END,     // フェードが終わったとき
    };

    FadeState m_fadeState = FADE_NONE;  // NONEから始める

    float m_alpha = 0.0f;   // フェードに使用するα値

    unsigned int m_currentIndex = 0;    // 現在のキャラ登場内のインデックス
    std::wstring m_currentCharaName;    // 現在のキャラの名前

    bool m_isFinished = false;  // 登場処理が終えたらtrue

    bool m_charaDiscovery = false;  // キャラを発見した際のフラグ

    bool m_firstFade = false;       // 最初の登場が始まったか
    bool m_firstFadeEnd = false;    // 最初のキャラ登場が終えたか

    std::vector<OutputParameter> m_parameters; // キャラごと

public:

    std::vector<OutputParameter>& GetCharaAppearanceParameter()
    {
        return m_parameters;
    }

    void CharaAppearanceIndexSkip(unsigned int skipObjectIndex) 
    {
        bool find = true;   // 一回だけ探す

        /* スキップが走った場合 */
        /* 続きのインデックスから始める必要があるため、キャラ登場パラメーター全体から次になるものを探す */
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

    void CharaAppearanceAnim() 
    {
        /* 全て終了したか */
        if (m_isFinished == true) { return; }

        /* FADE_STARTでフェードイン開始 */
        if (m_fadeState == FADE_START)
        {
            /* 既に登場済みだったらここでFADE_ENDにして終了 */
            if (m_parameters[m_currentIndex].charaAppearance == true)
            {
                m_fadeState = FADE_END;
                m_currentIndex++;
                return;
            }

            m_alpha += 0.01f;
            m_parameters[m_currentIndex].charaTexture->GetComponent<TextureCharaComponent>()->SetDrawable(true);
            m_parameters[m_currentIndex].charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);

            /* 完全に顕現したら終了（インデックスを増やす） */
            if (m_alpha >= 1.0f)
            {
                /* はじめのフェードのみ */
                if (m_firstFade == true)
                {
                    m_firstFade = false;
                    m_firstFadeEnd = true;  // 最初の読み込み完了
                }

                m_alpha = 1.0f;                                                                                         // α値を1.0に固定
                m_parameters[m_currentIndex].charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);    // 再度代入

                m_parameters[m_currentIndex].charaAppearance = true;    // 登場フラグをtrueにする

                /* 登場した場合、その後に同じキャラがいた場合は全てに出現済みフラグをtrueにする */
                for (unsigned int i = m_currentIndex + 1; i < m_parameters.size(); ++i)
                {
                    if (m_parameters[i].charaName == m_parameters[m_currentIndex].charaName)
                    {
                        m_parameters[i].charaAppearance = true;
                    }
                }

                m_fadeState = FADE_END;     // フェード状態をENDへ
                m_currentIndex++;           // インデックスを次へ
                m_charaDiscovery = false;   // 再度falseへ戻す
                m_alpha = 0.0f;             // 再度0に戻す
            }
        }

        /* 全て終わったらフィニッシュ */
        if (m_currentIndex >= m_parameters.size())
        {
            m_isFinished = true;
        }
    }

    void LeftClickCharaAppearance() 
    {
        if (m_firstFade == false && m_firstFadeEnd == false && m_fadeState == FADE_NONE)
        {
            m_firstFade = true;
            m_fadeState = FADE_START;   // フェード開始
        }
        else if (m_fadeState == FADE_NONE)
        {
            m_fadeState = FADE_START;   // フェード開始
        }
    }

    /* フェードがNONEかチェック */
    bool CheckCharaAppearanceFadeNone() const { return m_fadeState == FADE_NONE; }
    /* フェードがENDかチェック */
    bool CheckCharaAppearanceFadeEnd() const { return m_fadeState == FADE_END; }
    /* フェードをEndの次にNoneにセット */
    void  SetCharaAppearanceFadeNone() { m_fadeState = FADE_NONE; }

    bool GetCharaAppearanceFirstFadeEnd() const { return m_firstFadeEnd; }

    void Init() override 
    {
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

        if (createList == nullptr) { return; }

        /* キャラごとに登録された画像パス、トランスフォーム、名前、クリエイトリスト内のインデックスを読み込む */

        unsigned int i = 0; // クリエイトリストのインデックス

        for (ObjectEntry& listObject : createList->GetListObjects())
        {
            if (listObject.objectType == ObjectType::CharaAppearance)
            {
                CreateCharaAppearance* charaAppearance = dynamic_cast<CreateCharaAppearance*>(listObject.listObject);

                if (charaAppearance != nullptr) 
                {
                    OutputParameter parameter{};
                    parameter.listIndex = i;                                    // 全体のリストのインデックスを代入
                    parameter.path = charaAppearance->GetTexture();             // キャラの画像パスを代入
                    parameter.transform = charaAppearance->GetCharaTransform(); // キャラのトランスフォーム情報を代入
                    parameter.charaName = charaAppearance->GetCharaName();      // キャラの名前を代入
                    parameter.charaAppearance = false;                          // キャラの登場フラグの初期化

                    /* ここでキャラのパラメーターを格納する */
                    m_parameters.push_back(parameter);
                }
            }

            i++;    //インデックス加算
        }


        if (m_parameters.empty()) { return; }

        /* リストの一番最初だったらtrue */
        if (m_parameters[0].listIndex == 0)
        {
            m_firstFade = true;
            m_fadeState = FADE_START;   // フェード開始
        }

        if (Manager::GetDebugScene())
        {
            /* キャラごとにテクスチャの初期化 */
            for (int i = 0; i < m_parameters.size(); ++i)
            {
                CharaTexture* charaTexture = Manager::GetDebugScene()->AddGameObject<CharaTexture>(OBJECT_2D);

                charaTexture->Init();
                charaTexture->GetComponent<TextureCharaComponent>()->SetTexture(m_parameters[i].path);  // キャラの画像パスを取得
                charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);                 // キャラのα値を取得（最初はみんな透明）
                charaTexture->GetComponent<TextureCharaComponent>()->SetDrawable(false);                // キャラの描画状態をOFF
                charaTexture->GetComponent<Transform>()->Set(m_parameters[i].transform);                // キャラのトランスフォーム情報を取得

                m_parameters[i].charaTexture = charaTexture;   // 最後にまとめて代入
            }
        }
    }

    void Uninit() override {}
    void Update() override {}
    void Draw() override {}
};

#endif //_CHARA_APPEARANCE_OUTPUT_COMPONENT_H