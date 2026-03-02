/*===============================================================*/
//
//	createList.h(エディタリスト管理)
// 
// 
// ---------------------------------- 2025/09/28 製作者：佐藤琢磨
//
/*===============================================================*/
/* オブジェクトタイプ一覧↓ */
//
// SpeakText		:セリフテキスト
// BgTexture		:背景画像
// CharaAppearance	:キャラの登場
// CharaChange		:キャラの状態変更
// CharaExit		:キャラの退場
// BGM				:BGM
// SE				:SE
// GameStop			:ゲーム停止
// Ravel			:ラベル
// BranchButton		:分岐ボタン
// Jump				:ジャンプ
//
/*===============================================================*/

#ifndef _CREATE_LIST_H
#define _CREATE_LIST_H

#include "gameObject.h"
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "transform.h"
#include "yaml-cpp/yaml.h"

class CreateSpeakText;
class CreateBgTexture;
class CreateCharaAppearance;
class CreateCharaChange;
class CreateCharaExit;
class CreateBgm;
class CreateSe;
class CreateGameStop;
class CreateRavel;
class CreateBranchButton;
class CreateJump;
class CreateSpeakTextHidden;
class CreateSpeakTextDisplay;

enum class ObjectType
{
	Type_None,			// タイプなし
	SpeakText,			// セリフテキスト
	BgTexture,			// 背景画像
	CharaAppearance,	// キャラの登場
	CharaChange,		// キャラの状態変更
	CharaExit,			// キャラの退場
	BGM,				// BGM
	SE,					// SE
	GameStop,			// ゲーム停止
	Ravel,				// ラベル
	BranchButton,		// 分岐ボタン
	Jump,				// ジャンプ
	SpeakTextHidden,	// テキストフレーム非表示
	SpeakTextDisplay,	// テキストフレーム表示
};

struct ObjectEntry
{
	GameObject* listObject;	// リストオブジェクトのゲームオブジェクト
	ObjectType objectType;	// リストオブジェクトごとに指定 
};

class CreateList : public GameObject
{
private:
	std::list<ObjectEntry> m_listObjects;

	float m_objWidth = 0.0f;							// リスト内オブジェクトの幅
	float m_objHeight = 0.0f;							// リスト内オブジェクトの高さ
	float m_prevListPosY = 0.0f;						// リストのポジションYの保持

public:
	void Init() override;
	void Uninit() override
	{
		for (auto component : GetComponentVector())
		{
			component->Uninit();
		}
	}

	void Update() override
	{
		ChildMove();
		ResizeList();

		for (auto component : GetComponentVector())
		{
			component->Update();
		}
	}

	void Draw() override
	{
		for (auto component : GetComponentVector())
		{
			component->Draw();
		}
	}

	CreateSpeakText* AddSpeakText(const std::wstring& name, const std::wstring& text);
	CreateBgTexture* AddBgTexture(const std::wstring& textureName);
	CreateCharaAppearance* AddCharaAppearance(const std::wstring& textureName, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring charaName);
	CreateCharaChange* AddCharaChange(const std::wstring& textureName, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring charaName);
	CreateCharaExit* AddCharaExit(const std::wstring& charaName);
	CreateBgm* AddBgm(const std::wstring& bgmName);
	CreateSe* AddSe(const std::wstring& seName);
	CreateGameStop* AddGameStop();
	CreateRavel* AddRavel(const std::wstring& ravelName);
	CreateBranchButton* AddBranchButton(const std::wstring& targetRavelName, const std::wstring& buttonText, const std::wstring texturePath, XMFLOAT2& pos, XMFLOAT2& scale, const Vector3& textColor);
	CreateJump* AddJump(const std::wstring& targetRavelName);
	CreateSpeakTextHidden* AddSpeakTextHidden();
	CreateSpeakTextDisplay* AddSpeakTextDisplay();

	void ListObjectDestroy() {
		// 削除予定のオブジェクトをリストから除去
		m_listObjects.remove_if([](ObjectEntry& entry) {
			return entry.listObject && entry.listObject->GetDestroy(); // Destroy フラグが立っているものを削除
			});
	}

	/* スクロール上限の更新 */
	void SetScrollMinMax();

	/* リストとリスト内のオブジェクトのポジション連動 */
	void ChildMove();

	/* リスト内オブジェクト全情報 */
	std::list<ObjectEntry>& GetListObjects() { return m_listObjects; }

	/* リスト内オブジェクトの配置更新 */
	void SetSequence();

	/* リストのサイズ変更 */
	void ResizeList();

	/* 全セリフテキストはこれで取得する */
	std::vector<CreateSpeakText*> GetAllTexts() const;
	/* 全背景画像情報はこれで取得する */
	std::vector<std::wstring> GetAllBgTextures() const;

	/* 全BGM情報はこれで取得する */
	std::vector<std::wstring> GetAllBgm() const;
};

#endif //_CREATE_LIST_H