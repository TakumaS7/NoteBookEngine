/*===============================================================*/
//
// seOutputComponent.h(BGM送りコンポーネント管理)
//
// ---------------------------------- 2026/02/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SE_OUTPUT_COMPONENT_H
#define _SE_OUTPUT_COMPONENT_H

#include "component.h"
#include <string>
#include <vector>
#include "se.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "soundComponent.h"
#include "outputParameter.h"
#include "createSe.h"

class SeOutputComponent : public Component
{
private:
    /* BGMの読み込み状態 */
    enum SeLoadState
    {
        LOAD_NONE,    // BGMをロードしてないとき
        LOAD_START,   // BGMのロードが始まったとき
        LOAD_END,     // BGMのロードが終わったとき
    };

    std::vector<OutputParameter> m_parameters;   // ここにクリエイトリストからプッシュバックしていく

    unsigned int m_currentIndex = 0; // 現在のSEインデックス

    bool m_isFinished = false;  // 全SE処理が終わったか

    SeLoadState m_seLoadState = LOAD_NONE;  // ロード状態

    Se* m_se = nullptr;   // 現在読み込んでいるSE

    bool m_firstLoad = false;       // 最初のロードを開始したか
    bool m_firstLoadEnd = false;    // 最初のSEを読み込み終えたか

    std::vector<ObjectEntry> m_listObjects;   // 全体のリストオブジェクト

public:
    /* スキップ機能が来た時に使用 */
    void SeIndexSkip(unsigned int skipObjectIndex)
    {
        bool find = true;   // 一回だけ探す

        /* スキップが走った場合 */
        /* 続きのインデックスから始める必要があるため、SEパラメーター全体から次になるものを探す */
        for (unsigned int i = 0; i < m_parameters.size(); ++i)
        {
            if (m_parameters[i].listIndex >= skipObjectIndex + 1)
            {
                /* falseだったら探さない */
                if (find == false) { break; }

                m_currentIndex = i; // 次に流すSEのインデックスを代入

                /* 先頭じゃない場合は１を引く（LeftClickで1足すため） */
                if (m_firstLoadEnd == true && m_currentIndex > 0)
                {
                    m_currentIndex += -1;
                }

                /* 先頭の場合 */
                if (i == 0)
                {
                    m_firstLoad = false;
                    m_firstLoadEnd = false;
                }

                if (m_currentIndex < m_parameters.size())
                {
                    /* 読み込み */
                    m_se->GetComponent<SoundComponent>()->SetMusic(m_parameters[m_currentIndex].path, false);
                }

                find = false;			// 一回読込処理が走ったら、終了する
                m_isFinished = false;	// 終了フラグをオフにする
            }
        }
    }

    void SeAnim()
    {
        /* インデックスが最後まで行ったら停止 */
        if (m_isFinished) { return; }

        /* 一番最初のSEの再生 */
        if (m_firstLoad == true)
        {
            /* SEの再生 */
            m_se->GetComponent<SoundComponent>()->SetPlayMusic();

            m_firstLoad = false;
            m_firstLoadEnd = true;  // 最初の読み込み完了
            m_seLoadState = LOAD_END;
        }

        /* 最初の画像が表示されていなければ下へは行かない */
        if (m_firstLoad == true) { return; }
        if (m_firstLoadEnd == false) { return; }

        /* 変更フラグがtrueになったときだけ */
        if (m_seLoadState == LOAD_START)
        {
            m_currentIndex++;

            if (m_currentIndex < m_parameters.size())
            {
                /* 読み込み */
                m_se->GetComponent<SoundComponent>()->SetMusic(m_parameters[m_currentIndex].path, false);
                m_se->GetComponent<SoundComponent>()->SetPlayMusic();
            }

            /* 一回通ったら終わり */
            m_seLoadState = LOAD_END;
        }

        /* 全部画像を出し終えたら終了 */
        if (m_currentIndex >= m_parameters.size())
        {
            m_isFinished = true;
        }
    }

    void LeftClickSe()
    {
        /* 自動で次のSEへ */
        if (m_firstLoad == false && m_firstLoadEnd == false)
        {
            m_firstLoad = true;
        }
        else if (m_seLoadState == LOAD_NONE)
        {
            m_seLoadState = LOAD_START;
        }
    }

    /* SEロードがNONEかチェック */
    bool CheckSeLoadNone() const { return m_seLoadState == LOAD_NONE; }
    /* SEロードがENDかチェック */
    bool CheckSeLoadEnd() const { return m_seLoadState == LOAD_END; }
    /* SEロードをEndの次にNoneにセット */
    void  SetSeLoadNone() { m_seLoadState = LOAD_NONE; }

    bool GetSeFirstLoadEnd() const { return m_firstLoadEnd; }

    void Init() override
    {
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

        if (createList == nullptr) { return; }

        /* SEごとにクリエイトリスト内のインデックス、ファイルパスを読み込む */

        unsigned int i = 0; // クリエイトリスト内のインデックス

        for (const ObjectEntry& listObject : createList->GetListObjects())
        {
            if (listObject.objectType == ObjectType::SE)
            {
                CreateSe* se = dynamic_cast<CreateSe*>(listObject.listObject);
                
                if (se != nullptr)
                {
                    OutputParameter parameter{};
                    parameter.listIndex = i;        // クリエイトリスト内のインデックスを取得
                    parameter.path = se->GetSe();   // SEのパスを取得

                    /* ここでSEごとに格納していく */
                    m_parameters.push_back(parameter);
                }
            }

            i++;    // インデックス加算
        }

        if (m_parameters.empty()) { return; }

        /* リスト内で最初のインデックスだったらtrueにする */
        if (m_parameters[0].listIndex == 0)
        {
            m_firstLoad = true;
        }


        /* 一番最初のSEを読み込む */
        if (Manager::GetDebugScene())
        {
            /* 最初のSEを生成 */
            m_se = Manager::GetDebugScene()->AddGameObject<Se>(OBJECT_2D);
            m_se->Init();
            m_se->GetComponent<SoundComponent>()->SetMusic(m_parameters[m_currentIndex].path, false);
        }
    }

    void Uninit() override {}
    void Update() override {}
    void Draw() override {}

};

#endif //_SE_OUTPUT_COMPONENT_H