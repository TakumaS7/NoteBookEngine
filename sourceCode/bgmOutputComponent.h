/*===============================================================*/
//
// bgmOutputComponent.h(BGM送りコンポーネント管理)
//
// ---------------------------------- 2026/01/29 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BGM_OUTPUT_COMPONENT_H
#define _BGM_OUTPUT_COMPONENT_H

#include "component.h"
#include <string>
#include <vector>
#include "bgm.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"
#include "soundComponent.h"
#include "outputParameter.h"

class BgmOutputComponent : public Component
{
private:
    /* BGMの読み込み状態 */
    enum BgmLoadState
    {
        LOAD_NONE,    // BGMをロードしてないとき
        LOAD_START,   // BGMのロードが始まったとき
        LOAD_END,     // BGMのロードが終わったとき
    };

    std::vector<OutputParameter> m_listBgmParameter;   // ここにクリエイトリストからプッシュバックしていく

    unsigned int m_currentIndex = 0; // 現在のBGMインデックス

    bool m_isFinished = false;  // 全BGM処理が終わったか

    BgmLoadState m_bgmLoadState = LOAD_NONE;  // ロード状態

    Bgm* m_bgm = nullptr;   // 現在流しているBGM

    bool m_firstLoad = false;       // 最初のロードを開始したか
    bool m_firstLoadEnd = false;    // 最初のBGMを読み込み終えたか

    std::vector<ObjectEntry> m_listObjects;   // 全体のリストオブジェクト

public:
    /* スキップ機能が来た時に使用 */
    void BgmIndexSkip(unsigned int skipObjectIndex)
    {
        bool find = true;   // 一回だけ探す

        /* スキップが走った場合 */
        /* 続きのインデックスから始める必要があるため、BGMパラメーター全体から次になるものを探す */
        for (unsigned int i = 0; i < m_listBgmParameter.size(); ++i)
        {
            if (m_listBgmParameter[i].listIndex >= skipObjectIndex + 1)
            {
                /* falseだったら探さない */
                if (find == false) { break; }

                m_currentIndex = i; // 次に流すBGMのインデックスを代入

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

                if (m_currentIndex < m_listBgmParameter.size())
                {
                    /* 読み込み */
                    m_bgm->GetComponent<SoundComponent>()->SetMusic(m_listBgmParameter[m_currentIndex].path, false);
                }

                find = false;			// 一回読込処理が走ったら、終了する
                m_isFinished = false;	// 終了フラグをオフにする
            }
        }
    }

    void BgmAnim()
    {
        /* インデックスが最後まで行ったら停止 */
        if (m_isFinished) { return; }

        /* 一番最初のBGMの再生 */
        if (m_firstLoad == true)
        {
            /* BGMの再生 */
            m_bgm->GetComponent<SoundComponent>()->SetStopMusic();
            m_bgm->GetComponent<SoundComponent>()->SetPlayMusic();

            m_firstLoad = false;
            m_firstLoadEnd = true;  // 最初の読み込み完了
            m_bgmLoadState = LOAD_END;
        }

        /* 最初の画像が表示されていなければ下へは行かない */
        if (m_firstLoad == true) { return; }
        if (m_firstLoadEnd == false) { return; }

        /* 変更フラグがtrueになったときだけ */
        if (m_bgmLoadState == LOAD_START)
        {
            m_currentIndex++;

            if (m_currentIndex < m_listBgmParameter.size())
            {
                /* 読み込み */
                m_bgm->GetComponent<SoundComponent>()->SetStopMusic();
                m_bgm->GetComponent<SoundComponent>()->SetMusic(m_listBgmParameter[m_currentIndex].path, true);
                m_bgm->GetComponent<SoundComponent>()->SetPlayMusic();
            }

            /* 一回通ったら終わり */
            m_bgmLoadState = LOAD_END;
        }

        /* 全部画像を出し終えたら終了 */
        if (m_currentIndex >= m_listBgmParameter.size())
        {
            m_isFinished = true;
        }
    }

    void LeftClickBgm()
    {
        /* 自動で次のBGMへ */
        if (m_firstLoad == false && m_firstLoadEnd == false)
        {
            m_firstLoad = true;
        }
        else if (m_bgmLoadState == LOAD_NONE)
        {
            m_bgmLoadState = LOAD_START;
        }
    }

    /* BGMロードがNONEかチェック */
    bool CheckBgmLoadNone() const { return m_bgmLoadState == LOAD_NONE; }
    /* BGMロードがENDかチェック */
    bool CheckBgmLoadEnd() const { return m_bgmLoadState == LOAD_END; }
    /* BGMロードをEndの次にNoneにセット */
    void  SetBgmLoadNone() { m_bgmLoadState = LOAD_NONE; }

    bool GetBgmFirstLoadEnd() const { return m_firstLoadEnd; }

    void Init() override 
    {
        CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

        if (createList != nullptr)
        {
            /* リスト全体のオブジェクトを取得 */
            for (ObjectEntry& listObject : createList->GetListObjects())
            {
                m_listObjects.push_back(listObject);
            }

            /* BGMだけのパスを取得 */
            std::vector<std::wstring> bgms = createList->GetAllBgm();

            if (bgms.empty()) { return; }

            /* リストからリスト内のインデックスを取得 */
            /* & */
            /* ここでm_listBgmParameterサイズの確保 */
            for (unsigned int i = 0; i < m_listObjects.size(); ++i)
            {
                if (m_listObjects[i].objectType == ObjectType::BGM)
                {
                    OutputParameter param{};
                    param.listIndex = i;

                    m_listBgmParameter.push_back(param);
                }
            }

            /* リストからキャラ画像パスを一つ一つ取得 */
            for (unsigned int i = 0; i < m_listBgmParameter.size(); ++i)
            {
                m_listBgmParameter[i].path = bgms[i];
            }

            /* リスト内で最初のインデックスだったらtrueにする */
            if (m_listBgmParameter[0].listIndex == 0)
            {
                m_firstLoad = true;
            }
        }


        /* 一番最初のBGMを読み込む */
        if (Manager::GetDebugScene())
        {
            /* 最初のBGMを生成 */
            m_bgm = Manager::GetDebugScene()->AddGameObject<Bgm>(OBJECT_2D);
            m_bgm->Init();
            m_bgm->GetComponent<SoundComponent>()->SetMusic(m_listBgmParameter[m_currentIndex].path, true);
        }
    }

    void Uninit() override {}
    void Update() override {}
    void Draw() override {}

};

#endif //_BGM_OUTPUT_COMPONENT_H