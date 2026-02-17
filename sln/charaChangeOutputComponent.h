/*===============================================================*/
//
// charaChangeOutputComponent.h (キャラ状態変更コンポーネント)
//
// ---------------------------------- 2026/02/08 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CHARA_CHANGE_OUTPUT_COMPONENT_H
#define _CHARA_CHANGE_OUTPUT_COMPONENT_H

#include "component.h"
#include <string>
#include <vector>
#include "transform.h"
#include "charaTexture.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "createCharaChange.h"
#include "charaAppearanceOutput.h"
#include "charaAppearanceOutputComponent.h"
#include "textureCharaComponent.h"
#include "outputParameter.h"

class CharaChangeOutputComponent : public Component
{
private:
    /* キャラのフェード状態 */
    enum FadeState
    {
        FADE_NONE,      // フェードしてないとき
        FADE_START,     // フェードが始まったとき
        FADE_OUT,       // フェードアウト
        FADE_CHANGE,    // 画像を切り替えるとき
        FADE_IN,        // フェードイン
        FADE_END,       // フェードが終わったとき
    };

    FadeState m_fadeState = FADE_NONE;

    std::vector<OutputParameter> m_parameters;          // クリエイトリスト内の情報を実行順に入れていく
    OutputParameter* m_appearanceParameter = nullptr;   // 該当の名前のパラメーターを参照するためのポインタ

    float m_alpha = 0.0f;   // 画像のα値

    bool m_isFinished = false;   // 全ての状態変更が終えたらtrue

    unsigned int m_currentIndex = 0;    // 現在のパラメーターのインデックス

public:
    void CharaChangeIndexSkip(unsigned int skipObjectIndex) 
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

    void CharaChangeAnim() 
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

        if (m_fadeState == FADE_START)
        {
            m_alpha = 1.0f; // α値の初期化

            /* どのキャラに向けてか */
            for (OutputParameter& appearanceParameter : charaAppearanceOutputComponent->GetCharaAppearanceParameter())
            {
                /* 該当のキャラ名を探す */
                if (m_parameters[m_currentIndex].charaName == appearanceParameter.charaName)
                {
                    /* キャラが登場していれば状態変更へ進む */
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
                /* 該当の名前が見つからなかったか、まだ登場していなかった場合 */
                m_fadeState = FADE_END;
                m_currentIndex++;
                return;
            }
        }
        else if (m_fadeState == FADE_OUT)
        {
            /* 画像フェードアウト開始 */
            if (m_alpha > 0.0f)
            {
                m_alpha -= 0.03f;
                m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);
            }
            else
            {
                /* 透明になったらテクスチャを変える */
                m_alpha = 0.0f;
                m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);
                m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetDrawable(false);
                m_fadeState = FADE_CHANGE;
            }
        }
        else if (m_fadeState == FADE_CHANGE)
        {
            /* テクスチャ変更 */
            m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetTexture(m_parameters[m_currentIndex].path);
            m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetDrawable(true);
            m_fadeState = FADE_IN;

        }
        else if (m_fadeState == FADE_IN)
        {
            /*画像フェードイン開始 */
            m_alpha += 0.03f;
            m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);

            if (m_alpha >= 1.0f)
            {
                /* フェードを終える */
                m_alpha = 1.0f; // α値の固定
                m_appearanceParameter->charaTexture->GetComponent<TextureCharaComponent>()->SetAlpha(m_alpha);

                m_appearanceParameter = nullptr;    // 参照するパラメーターのリセット
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

    void LeftClickCharaChange() 
    {
        /* フェード開始 */
        if (m_fadeState == FADE_NONE)
        {
            m_fadeState = FADE_START;
        }
    }

    /* フェードがNONEかチェック */
    bool CheckCharaChangeFadeNone() const { return m_fadeState == FADE_NONE; }
    /* フェードがENDかチェック */
    bool CheckCharaChangeFadeEnd() const { return m_fadeState == FADE_END; }
    /* フェードをEndの次にNoneにセット */
    void  SetCharaChangeFadeNone() { m_fadeState = FADE_NONE; }

    //bool GetCharaChangeFirstFadeEnd() const { return m_firstFadeEnd; }

	void Init() override 
    {
        /* リストの取得 */
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

        if (createList == nullptr) { return; }

        unsigned int i = 0; // クリエイトリストのインデックス

        /* 各パラメーターの取得 */
        for (ObjectEntry listObject : createList->GetListObjects())
        {
            if (listObject.objectType == ObjectType::CharaChange)
            {
                CreateCharaChange* charaChange = dynamic_cast<CreateCharaChange*>(listObject.listObject);

                if (charaChange != nullptr)
                {
                    OutputParameter parameter{};
                    parameter.listIndex = i;                                // 全体のリストのインデックスを代入
                    parameter.path = charaChange->GetTexture();             // キャラの画像パスを代入
                    parameter.transform = charaChange->GetCharaTransform(); // キャラのトランスフォーム情報を代入
                    parameter.charaName = charaChange->GetCharaName();      // キャラの名前を代入

                    /* ここでキャラ状態変更のパラメーターを格納する */
                    m_parameters.push_back(parameter);
                }
            }

            i++;    // インデックス加算
        }
    }

	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_CHARA_CHANGE_OUTPUT_COMPONENT_H