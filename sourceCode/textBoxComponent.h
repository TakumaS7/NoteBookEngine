/*===============================================================*/
//
// textBoxComponent.h(テキスト枠コンポーネント管理)
//
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TEXT_BOX_COMPONENT_H
#define _TEXT_BOX_COMPONENT_H

#include "component.h"
#include "gameObject.h"
#include <vector>
#include <string>
#include "manager.h"
#include "scene.h"
#include "createList.h"
#include "speakText.h"
#include "nameText.h"
#include "time.h"
#include "transform.h"
#include "createSpeakText.h"
#include "vector3.h"
#include "outputParameter.h"

class TextBoxComponent : public Component
{
private:
    std::vector<OutputParameter> m_listTextParameter; // ここにクリエイトリストからプッシュバックしていく

    int m_currentIndex = 0;             // 現在の文インデックス

    std::wstring m_currentText;         // 実際の全文
    std::wstring m_displayedText;       // 画面に表示中の文字列
    std::wstring m_currentName;         // 名前全文

    bool m_isFinished = false;          // 全文表示し終えたか
    bool m_isTextFullyShown = false;    // 現在の文が全文表示されたか

    float m_charInterval = 0.05f;       // 1文字の表示間隔(秒)
    float m_timer = 0.0f;               // 経過時間
    size_t m_charIndex = 0;             // 現在の文字インデックス

    SpeakText* m_speakText = nullptr;   // テキスト表示用オブジェクト
    NameText* m_nameText = nullptr;     // 名前テキスト表示用オブジェクト

    Transform* m_transform;             // テキストのトランスフォーム
    Transform* m_nameTransform;         // 名前のトランスフォーム

    bool m_firstLoad = false;           // 最初のロードが完了したかどうか

    Vector3 m_namePositionOffset = { 0.0f, -40.0f, 0.0f };  // 名前の位置調整

    std::vector<ObjectEntry> m_listObjects;  // 全体のリストオブジェクト

    bool m_display = true;  // テキストボックス表示状態

public:
    void Set(Transform* transform) { m_transform = transform; }

    bool GetIsTextFullyShown() const { return m_isTextFullyShown; }

    /* テキストフレームを表示へ */
    void SetDisplay() { m_display = true; }
    /* テキストフレームを非表示へ */
    void SetHidden() { m_display = false; }

    /* テキストフレーム非表示状態のゲッター */
    bool GetDisplay() const { return m_display; }

    /* スキップ機能が来た時に使用 */
    void TextIndexSkip(unsigned int skipObjectIndex)
    {
        bool find = true;   // 一回だけ探す

        /* スキップが走った場合 */
        /* 続きのインデックスから始める必要があるため、テキストパラメーター全体から次になるものを探す */
        for (unsigned int i = 0; i < m_listTextParameter.size(); ++i)
        {
            if (m_listTextParameter[i].listIndex >= skipObjectIndex + 1)
            {
                /* falseだったら探さない */
                if (find == false) { break; }

                m_currentIndex = i; // 次に表示するテキストのインデックスを代入

                /* 先頭じゃない場合は１を引く（LeftClickで1足すため） */
                if (m_firstLoad == true && m_currentIndex > 0)
                {
                    m_currentIndex += -1;
                }

                /* 先頭の場合 */
                if (i == 0)
                {
                    m_firstLoad = false;
                }

                /* ここで再度テキストの読み込みを行う */
                m_currentText = m_listTextParameter[m_currentIndex].speakText->GetText();   // 次の文を取得
                m_currentName = m_listTextParameter[m_currentIndex].speakText->GetName();   // 次の名前を取得

                find = false;			// 一回読込処理が走ったら、終了する
                m_isFinished = false;	// 終了フラグをオフにする
            }
        }
    }

    void TextAnim()
    {
        /* インデックスが最後まで行ったら停止 */
        if (!m_speakText || m_isFinished) { return; }

        /* 非表示だった場合は通らない */
        if (m_display == false) 
        { 
			m_isTextFullyShown = true;    // 非表示のまま全文表示された状態にする
            return; 
        }

        float delta = Time::m_deltaTime;
        m_timer += delta;

        /* 文字を一文字ずつ追加 */
        if (!m_isTextFullyShown && m_timer >= m_charInterval)
        {
            m_timer = 0.0f; // m_charIntervalを越えるまでまた待機する

            if (m_charIndex < m_currentText.size())
            {
                m_displayedText.push_back(m_currentText[m_charIndex]);  // １文字ずつプッシュ
                m_charIndex++;                                          // 次の１文字へ
                m_speakText->Set(m_displayedText, m_transform);         // １文字表示する
            }
            else
            {
                m_isTextFullyShown = true;  // 全文字表示完了
            }
        }

        m_nameText->Set(m_currentName, m_nameTransform);
    }

    void LeftClickText()
    {
        /* 非表示なら無視する */
        if (m_display == false) 
        {
			m_currentIndex++;           // 次の文のIndexへ（スキップと同様に次の文へ行く）
			m_isTextFullyShown = true;  // 非表示のまま全文表示された状態にする
            return; 
        }

        /* リストの途中からなら */
        if (m_listTextParameter[0].listIndex > 0 && m_firstLoad == false)
        {
            m_currentText = m_listTextParameter[m_currentIndex].speakText->GetText();  // 最初のセリフをセット
            m_currentName = m_listTextParameter[m_currentIndex].speakText->GetName();  // 最初の名前をセット
            m_displayedText.clear();    // 文字表示状態をリセット
            m_charIndex = 0;            // ０文字目から始める
            m_isTextFullyShown = false;
            m_firstLoad = true;
        }

        if (m_charIndex == 0) { return; }

        /* 左クリック処理 */
        if (!m_isTextFullyShown)
        {
            /* まだ全文表示されていなければスキップ */
            m_displayedText = m_currentText;
            m_charIndex = m_currentText.size();
            m_speakText->Set(m_displayedText, m_transform);
            m_isTextFullyShown = true;
        }
        else
        {
            /* 次の文へ */
            m_currentIndex++;   // 次の文のIndexへ

            /* 次の文が残っていたら準備 */
            if (m_currentIndex < static_cast<int>(m_listTextParameter.size()))
            {
                m_currentText = m_listTextParameter[m_currentIndex].speakText->GetText();    // 次の文を取得
                m_currentName = m_listTextParameter[m_currentIndex].speakText->GetName();    // 次の名前を取得
                m_displayedText.clear();                    // 表示中の文字列を空にする
                m_charIndex = 0;                            // 文字送りカウンタを先頭に戻す
                m_isTextFullyShown = false;                 // 全文表示済みフラグをfalse

                /* 前のテキストを削除して新しく生成 */
                if (m_speakText)
                {
                    m_speakText->SetDestroy();
                    m_speakText = nullptr;
                }
                if (m_nameText)
                {
                    m_nameText->SetDestroy();
                    m_nameText = nullptr;
                }

                m_speakText = Manager::GetDebugScene()->AddGameObject<SpeakText>(OBJECT_2D);
                m_speakText->Set(L"", m_transform);
                m_speakText->Init();

                m_nameText = Manager::GetDebugScene()->AddGameObject<NameText>(OBJECT_2D);
                m_nameText->Set(m_currentName, m_nameTransform);
                m_nameText->Init();
            }
            else
            {
                m_isFinished = true;    // 全文終了ならtrue
            }
        }
    }

    void Init() override
    {
        /* エディタ上のリストを取得 */
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();
        if (createList != nullptr) 
        {
            /* リスト全体のオブジェクトを取得 */
            for (ObjectEntry& listObject : createList->GetListObjects())
            {
                m_listObjects.push_back(listObject);
            }

            /* テキストだけの全文を取得 */
            std::vector<CreateSpeakText*> texts = createList->GetAllTexts();

            if (texts.empty()) { return; }

            /* リストから全リスト内のインデックスを取得 */
            /* & */
            /* ここでm_listBgTextureParameterサイズの確保 */
            for (unsigned int i = 0; i < m_listObjects.size(); ++i)
            {
                if (m_listObjects[i].objectType == ObjectType::SpeakText)
                {
                    OutputParameter param{};
                    param.listIndex = i;

                    m_listTextParameter.push_back(param);
                }
            }

            /* リストからテキスト全文を一つ一つ取得 */
            for (unsigned int i = 0; i < m_listTextParameter.size(); ++i)
            {
                m_listTextParameter[i].speakText = texts[i];
            }

            /* リストの一番先頭だったら */
            if (m_listTextParameter[0].listIndex == 0)
            {
                m_currentText = m_listTextParameter[0].speakText->GetText(); // 最初のセリフをセット
                m_currentName = m_listTextParameter[0].speakText->GetName(); // 最初の名前をセット
                m_displayedText.clear();    // 文字表示状態をリセット
                m_charIndex = 0;            // ０文字目から始める
                m_isTextFullyShown = false;
            }
        }



        m_nameTransform = new Transform();
        m_nameTransform->position = m_transform->position;
        m_nameTransform->position += m_namePositionOffset;
        m_nameTransform->rotation = m_transform->rotation;
        m_nameTransform->scale = m_transform->scale;

        /* SpeakObjectを生成（描画用） */
        if (Manager::GetDebugScene())
        {
            m_speakText = Manager::GetDebugScene()->AddGameObject<SpeakText>(OBJECT_2D);
            m_speakText->Set(L"", m_transform);
            m_speakText->Init();
            m_nameText = Manager::GetDebugScene()->AddGameObject<NameText>(OBJECT_2D);
            m_nameText->Set(L"", m_nameTransform);
            m_nameText->Init();
        }
    }

    void Uninit() override 
    {
        if (m_nameTransform)
        {
            m_nameTransform->Uninit();
            delete m_nameTransform;
        }
    }

    void Update() override {}

    void Draw() override {}
};

#endif //_TEXT_BOX_COMPONENT_H
