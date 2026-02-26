/*===============================================================*/
//
// bgTextureOutputComponent.h (背景画像送りコンポーネント)
//
// ---------------------------------- 2025/12/11 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BG_TEXTURE_OUTPUT_COMPONENT_H
#define _BG_TEXTURE_OUTPUT_COMPONENT_H

#include "component.h"
#include "gameObject.h"
#include <vector>
#include <string>
#include "manager.h"
#include "scene.h"
#include "createList.h"
#include "time.h"
#include "bgTexture.h"
#include "textureComponent.h"
#include "sortingLayer.h"
#include "outputParameter.h"

class BgTextureOutputComponent : public Component
{
private:
    /* 背景クロスフェード状態 */
    enum CrossFadeState
    {
        CROSS_FADE_NONE,    // クロスフェードしてないとき
        CROSS_FADE_START,   // クロスフェードが始まったとき
        CROSS_FADE_END,     // クロスフェードが終わったとき
    };

    std::vector<OutputParameter> m_listBgTextureParameter; // ここにクリエイトリストからプッシュバックしていく

    unsigned int m_currentIndex = 0; // 現在の背景画像インデックス

    bool m_isFinished = false;  // 全背景画像の表示が終わったか

    BgTexture* m_bgTexture = nullptr;           // 現在表示中の背景画像
    BgTexture* m_anotherBgTexture = nullptr;    // 現在表示中の背景画像二つ目

    CrossFadeState m_crossFadeState = CROSS_FADE_NONE;  // クロスフェード状態

    float m_alpha = 1.0f;           // 背景のα値
    float m_anotherAlpha = 0.0f;    // 背景のα値二つ目

    bool m_anotherFadeIn = false;   // 二つ目のα値を増やす際の判定フラグ

    bool m_isAnotherFront = false;  // false: bgTextureが前 / true: anotherBgTextureが前

    bool m_firstLoad = false;               // 最初の背景画像生成処理
    bool m_firstLoadEnd = false;            // 最初の背景画像を読み込んだか
    float m_firstAlpha = 0.0f;              // 最初のテクスチャのalpha処理
    std::vector<ObjectEntry> m_listObjects;  // 全体のリストオブジェクト

public:
    /* スキップ機能が来た時に使用 */
    void BgTextureIndexSkip(unsigned int skipObjectIndex)
    {
        bool find = true;   // 一回だけ探す

        /* スキップが走った場合 */
        /* 続きのインデックスから始める必要があるため、背景画像パラメーター全体から次になるものを探す */
        for (unsigned int i = 0; i < m_listBgTextureParameter.size(); ++i)
        {
            if (m_listBgTextureParameter[i].listIndex >= skipObjectIndex + 1)
            {
                /* falseだったら探さない */
                if (find == false) { break; }

				unsigned int oldCurrentIndex = m_currentIndex - 1;  // 古いインデックスを保存（カレントを１足していたため、１引く）
				if (m_currentIndex == 0) { oldCurrentIndex = 0; }   // 最初のインデックスだったら、古いインデックスも最初のインデックスにする
                
                m_currentIndex = i;                                 // 次に表示する画像のインデックスを代入

                /* 同じインデックスの場合は読み込まずに終える */
                if (oldCurrentIndex == m_currentIndex)
                {
					m_crossFadeState = CROSS_FADE_END;  // クロスフェード状態を終わりへ
                    
                    break;
                }

                /* ここで再度見えていないほうのスプライトに画像読み込みを行う */
                if (m_alpha <= 0.0f)
                {
                    if (m_currentIndex < m_listBgTextureParameter.size())
                    {
                        /* 読み込み */
                        m_bgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                    }
                }
                else if (m_anotherAlpha <= 0.0f && m_listBgTextureParameter.size() >= 2)    //  二枚目が存在し、透明の場合
                {
                    if (m_currentIndex < m_listBgTextureParameter.size())
                    {
                        /* 読み込み */
                        m_anotherBgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                    }
                }
                
                /* まだ読み込みが一回もされてないとき */
                if (m_firstLoadEnd == false)
                {
                    /* 読み込み */
                    m_bgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);

                    m_currentIndex++;   // 次のインデックスへ
                    m_anotherBgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                }


                find = false;			// 一回読込処理が走ったら、終了する
                m_isFinished = false;	// 終了フラグをオフにする
            }
        }
    }

    void BgTextureAnim()
    {
        /* インデックスが最後まで行ったら停止 */
        if (m_isFinished) { return; }

        /* 一番最初のテクスチャのフェードイン */
        if (m_firstLoad == true)
        {
            m_firstAlpha += 0.01f;  // だんだんフェードイン

            /* α値の適用 */
            m_bgTexture->GetComponent<TextureComponent>()->SetAlpha(m_firstAlpha);

            /* 最初の画像が表示されたなら */
            if (m_firstAlpha >= 1.0f) {
                m_firstAlpha = 1.0f;
                m_firstLoad = false;
                m_firstLoadEnd = true;  // 最初の読み込み完了
                m_crossFadeState = CROSS_FADE_END;
            }
        }

        /* ここでも確認（1枚だけの場合） */
        if (m_isFinished) { return; }

        /* 最初の画像が表示されていなければ下へは行かない */
        if (m_firstLoad == true) { return; }
        if (m_firstLoadEnd == false) { return; }


        /* ここから二枚目以降のクロスフェード処理 */
        if (m_crossFadeState == CROSS_FADE_START && m_anotherBgTexture != nullptr)
        {
            if (m_anotherFadeIn == false) // 一つ目がフェードイン
            {
                /* フェードイン */
                m_alpha += 0.01f;
                m_bgTexture->GetComponent<TextureComponent>()->SetAlpha(m_alpha);

                /* アナザーがフェードアウト */
                m_anotherAlpha += -0.01f;
                m_anotherBgTexture->GetComponent<TextureComponent>()->SetAlpha(m_anotherAlpha);
            }
            else    // 二つ目がフェードイン
            {
                /* フェードアウト */
                m_alpha += -0.01f;
                m_bgTexture->GetComponent<TextureComponent>()->SetAlpha(m_alpha);

                /* アナザーがフェードイン */
                m_anotherAlpha += 0.01f;
                m_anotherBgTexture->GetComponent<TextureComponent>()->SetAlpha(m_anotherAlpha);
            }


            /* フェード終了 */
            /* ここでは、フェードアウトしたほうに新たなテクスチャをロードしておく */
            /* m_anotherFadeInの状態が */
            /* false：一つ目がα値＝１、二つ目がα値＝０ */
            /* true：一つ目がα値＝０、二つ目がα値＝１ */
            if ((m_anotherFadeIn == false && m_alpha >= 1.0f && m_anotherAlpha <= 0.0f) ||
                (m_anotherFadeIn == true && m_alpha <= 0.0f && m_anotherAlpha >= 1.0f))
            {
                m_currentIndex++;   // 次のインデックスへ

                /* フェードアウト側にテクスチャを読み込む */
                if (m_anotherFadeIn == true)
                {
                    if (m_currentIndex < m_listBgTextureParameter.size())
                    {
                        /* 読み込み */
                        m_bgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                    }
                }
                else
                {
                    if (m_currentIndex < m_listBgTextureParameter.size())
                    {
                        /* 読み込み */
                        m_anotherBgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                    }
                }

                if (m_anotherFadeIn)
                {
                    /* another が前面になった */
                    m_anotherAlpha = 1.0f;
                    m_alpha = 0.0f;
                }
                else
                {
                    /* bg が前面になった */
                    m_alpha = 1.0f;
                    m_anotherAlpha = 0.0f;
                }

                m_bgTexture->GetComponent<TextureComponent>()->SetAlpha(m_alpha);
                m_anotherBgTexture->GetComponent<TextureComponent>()->SetAlpha(m_anotherAlpha);

                /* フェード終了処理 */
                m_isAnotherFront = !m_isAnotherFront;
                m_crossFadeState = CROSS_FADE_END;  // クロスフェード状態を終わりへ
            }

            /* α制御 */
            if (m_alpha >= 1.0f) {
                m_alpha = 1.0f;
            }
            else if (m_alpha <= 0.0f) {
                m_alpha = 0.0f;
            }

            if (m_anotherAlpha >= 1.0f) {
                m_anotherAlpha = 1.0f;
            }
            else if (m_anotherAlpha <= 0.0f) {
                m_anotherAlpha = 0.0f;
            }
        }

        /* 全部画像を出し終えたら終了 */
        if (m_currentIndex >= m_listBgTextureParameter.size())
        {
            m_isFinished = true;
        }
    }

    void LeftClickBgTexture()
    {
        /* 自動で次の画像へ */
        if (m_firstLoad == false && m_firstLoadEnd == false)
        {
            m_firstLoad = true;
        }
        else if (m_crossFadeState == CROSS_FADE_NONE)
        {
            m_crossFadeState = CROSS_FADE_START;    // クロスフェード開始

            /* フェード処理 */
            if (m_isAnotherFront) {
                m_anotherFadeIn = false;  // 一つ目のα値を増加
            }
            else {
                m_anotherFadeIn = true;   // 二つ目のα値を増加
            }
        }
    }

    /* クロスフェードがNONEかチェック */
    bool CheckBgCrossfFadeNone() const { return m_crossFadeState == CROSS_FADE_NONE; }
    /* クロスフェードがENDかチェック */
    bool CheckBgCrossfFadeEnd() const { return m_crossFadeState == CROSS_FADE_END; }
    /* クロスフェードをEndの次にNoneにセット */
    void  SetBgCrossFadeNone() { m_crossFadeState = CROSS_FADE_NONE; }

    bool GetBgFirstLoadEnd() const { return m_firstLoadEnd; }



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

            /* 背景だけのテクスチャパスを取得 */
            std::vector<std::wstring> bgTextures = createList->GetAllBgTextures();

            if (bgTextures.empty()) { return; }

            /* リストからリスト内のインデックスを取得 */
            /* & */
            /* ここでm_listBgTextureParameterサイズの確保 */
            for (unsigned int i = 0; i < m_listObjects.size(); ++i)
            {
                if (m_listObjects[i].objectType == ObjectType::BgTexture)
                {
                    OutputParameter param{};
                    param.listIndex = i;

                    m_listBgTextureParameter.push_back(param);
                }
            }

            /* リストからキャラ画像パスを一つ一つ取得 */
            for (unsigned int i = 0; i < m_listBgTextureParameter.size(); ++i)
            {
                m_listBgTextureParameter[i].path = bgTextures[i];
            }

            /* リスト内で最初のインデックスだったらtrueにする */
            if (m_listBgTextureParameter[0].listIndex == 0)
            {
                m_firstLoad = true;
            }
        }



        /* 一番最初の画像を読み込む */
        if (Manager::GetDebugScene())
        {
            /* 最初の背景画像を生成 */
            m_bgTexture = Manager::GetDebugScene()->AddGameObject<BgTexture>(OBJECT_2D);
            m_bgTexture->Init();
            m_bgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
            m_bgTexture->GetComponent<TextureComponent>()->SetAlpha(m_firstAlpha);

            m_currentIndex = 1; // 二枚目のインデックスへ
            /* 複数枚の場合に管理 */
            if (m_listBgTextureParameter.size() >= 2)
            {
                /* 最初の背景画像を生成 */
                m_anotherBgTexture = Manager::GetDebugScene()->AddGameObject<BgTexture>(OBJECT_2D);
                m_anotherBgTexture->Init();
                m_anotherBgTexture->GetComponent<TextureComponent>()->SetTexture(m_listBgTextureParameter[m_currentIndex].path);
                m_anotherBgTexture->GetComponent<TextureComponent>()->SetAlpha(m_anotherAlpha);
            }
        }
    }

    void Uninit() override {}

    void Update() override {}

    void Draw() override {}
};

#endif //_BG_TEXTURE_OUTPUT_COMPONENT_H
