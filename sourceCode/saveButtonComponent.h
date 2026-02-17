/*===============================================================*/
//
//	saveButtonComponent.h(セーブするボタン操作)
// 
// 
// ---------------------------------- 2026/02/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SAVE_BUTTON_COMPONENT_H
#define _SAVE_BUTTON_COMPONENT_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "yamlConvert.h"
#include "yaml-cpp/yaml.h"
#include "createList.h"
#include <string>
#include <memory>
#include <list>
#include <fstream>
#include <experimental/filesystem>
#include "manager.h"
#include "scene.h"
#include "imGuiManager.h"

#include "createSpeakText.h"
#include "createBgTexture.h"
#include "createCharaAppearance.h"
#include "createCharaChange.h"
#include "createCharaExit.h"
#include "createBgm.h"
#include "createSe.h"
#include "createGameStop.h"
#include "createRavel.h"
#include "createBranchButton.h"
#include "createJump.h"

namespace fs = std::experimental::filesystem;

/* クリエイトリストオブジェクト保存用構造体 */
struct ObjectEntryData
{
	YAML::Node listObjectData;	// リストオブジェクトのゲームオブジェクトデータ
	std::string objectType;		// リストオブジェクトごとのタイプデータ
};

class SaveButtonComponent : public Component
{
private:
	GameObject* m_gameObject;	// ボタン自身
	bool m_buttonHit = false;	// ボタン自身の当たり判定情報
	bool m_save = false;		// ボタンを押したらセーブ

	std::list<ObjectEntryData> m_objectEntryData;	// クリエイトリストオブジェクト保存用
	std::list <YAML::Node> m_charaNameData;			// 登録したキャラ名

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		m_save = false;

		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* 左クリック処理 */
		if (m_buttonHit && Mouse::GetLeftClickTrigger())
		{
			m_save = true;
		}

		/* セーブ処理 */
		if (m_save == true)
		{
			/* クリエイトリスト内のオブジェクトをセーブする */
			SaveListObject();

			/* 登録したキャラ名をセーブする */
			SaveCharaNames();

			YAML::Node root;
			YAML::Node saveData;

			/* SaveListObject関数で格納した要素を読み込む */
			for (const ObjectEntryData& objectEntry : m_objectEntryData)
			{
				YAML::Node sendData;

				sendData["objectType"] = objectEntry.objectType;
				sendData["listObjectData"] = objectEntry.listObjectData;

				saveData.push_back(sendData);	// 保存内容を送る
			}

			root["objectEntry"] = saveData;
			root["charaNames"] = m_charaNameData;

			YAML::Emitter out;
			out << root;

			/* エラーチェック */
			std::wstring errorLog = L"YAML Emit エラー: " + Utf8ToUtf16(out.GetLastError());
			if (!out.good()) {
				MessageBoxW(nullptr, errorLog.c_str(), L"YAML ERROR", MB_OK);
				return;
			}

			/* YAMLファイルへ保存（プロジェクト → myProject → nb_createProject → nb_saveData → nb_createProject.yaml） */
			fs::path nodeFolderName = fs::path("myProject");																						// 全てのプロジェクトを置くフォルダ名
			fs::path projectPath = ImGuiManager::GetProjectRoot()/ nodeFolderName / ImGuiManager::GetSelectProjectName() / fs::path("nb_saveData");	// セーブデータの格納先
			fs::path dataPath = projectPath / (ImGuiManager::GetSelectProjectName() + L".yaml");													// セーブデータのパス
			fs::create_directories(projectPath);																									// フォルダがない場合は追加

			std::ofstream writeIn(dataPath);
			if (!writeIn)
			{
				MessageBoxW(nullptr, L"ファイルを開けませんでした", L"ERROR", MB_OK);
				return;
			}

			writeIn << out.c_str();
			writeIn.close();

			MessageBoxW(nullptr, L"プロジェクトをセーブしました！", L"完了", MB_OK);
		}
	}

	void Draw() override {}



	/* リストオブジェクトの保存 */
	void SaveListObject()
	{
		/* 前回のデータをクリア */
		m_objectEntryData.clear();

		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();
		if (createList == nullptr) { return; }

		/* タイプを判別し、それぞれパラメーターを保存していく */
		for (const ObjectEntry& entry : createList->GetListObjects())
		{
			YAML::Node parameter{};

			if (entry.objectType == ObjectType::SpeakText)	// セリフテキスト
			{
				CreateSpeakText* speakText = dynamic_cast<CreateSpeakText*>(entry.listObject);
				if (speakText == nullptr) { continue; }

				parameter["name"] = Utf16ToUtf8(speakText->GetName());	// 名前
				parameter["text"] = Utf16ToUtf8(speakText->GetText());	// セリフテキスト
			}
			else if (entry.objectType == ObjectType::BgTexture)	// 背景
			{
				CreateBgTexture* bgTexture = dynamic_cast<CreateBgTexture*>(entry.listObject);
				if (bgTexture == nullptr) { continue; }

				parameter["path"] = Utf16ToUtf8(bgTexture->GetTexture());	// テクスチャパス

			}
			else if (entry.objectType == ObjectType::CharaAppearance)	// キャラ登場
			{
				CreateCharaAppearance* charaAppearance = dynamic_cast<CreateCharaAppearance*>(entry.listObject);
				if (charaAppearance == nullptr) { continue; }

				parameter["name"] = Utf16ToUtf8(charaAppearance->GetCharaName());	// キャラの名前
				parameter["path"] = Utf16ToUtf8(charaAppearance->GetTexture());		// テクスチャパス
				parameter["position"] = charaAppearance->GetPosition();				// 座標
				parameter["rotation"] = charaAppearance->GetRotation();				// 回転
				parameter["scale"] = charaAppearance->GetScale();					// 大きさ
			}
			else if (entry.objectType == ObjectType::CharaChange)	// キャラ状態変更
			{
				CreateCharaChange* charaChange = dynamic_cast<CreateCharaChange*>(entry.listObject);
				if (charaChange == nullptr) { continue; }

				parameter["name"] = Utf16ToUtf8(charaChange->GetCharaName());	// キャラの名前
				parameter["path"] = Utf16ToUtf8(charaChange->GetTexture());		// テクスチャパス
				parameter["position"] = charaChange->GetPosition();				// 座標
				parameter["rotation"] = charaChange->GetRotation();				// 回転
				parameter["scale"] = charaChange->GetScale();					// 大きさ
			}
			else if (entry.objectType == ObjectType::CharaExit)	// キャラ退場
			{
				CreateCharaExit* charaExit = dynamic_cast<CreateCharaExit*>(entry.listObject);
				if (charaExit == nullptr) { continue; }

				parameter["name"] = Utf16ToUtf8(charaExit->GetCharaName());	// キャラの名前
			}
			else if (entry.objectType == ObjectType::BGM)	// BGM
			{
				CreateBgm* bgm = dynamic_cast<CreateBgm*>(entry.listObject);
				if (bgm == nullptr) { continue; }

				parameter["path"] = Utf16ToUtf8(bgm->GetBgm());	// サウンドパス
			}
			else if (entry.objectType == ObjectType::SE)	// SE
			{
				CreateSe* se = dynamic_cast<CreateSe*>(entry.listObject);
				if (se == nullptr) { continue; }

				parameter["path"] = Utf16ToUtf8(se->GetSe());	// サウンドパス
			}
			else if (entry.objectType == ObjectType::GameStop)	// 停止
			{
				CreateGameStop* gameStop = dynamic_cast<CreateGameStop*>(entry.listObject);
				if (gameStop == nullptr) { continue; }
			}
			else if (entry.objectType == ObjectType::Ravel)	// ラベル
			{
				CreateRavel* ravel = dynamic_cast<CreateRavel*>(entry.listObject);
				if (ravel == nullptr) { continue; }

				parameter["name"] = Utf16ToUtf8(ravel->GetRavelName());	// ラベル名
			}
			else if (entry.objectType == ObjectType::BranchButton)	// 分岐ボタン
			{
				CreateBranchButton* branchButton = dynamic_cast<CreateBranchButton*>(entry.listObject);
				if (branchButton == nullptr) { continue; }

				parameter["targetName"] = Utf16ToUtf8(branchButton->GetTargetRavelName());	// 目的のラベル名
				parameter["text"] = Utf16ToUtf8(branchButton->GetButtonText());				// ボタン内のテキスト
				parameter["path"] = Utf16ToUtf8(branchButton->GetTexturePath());			// テクスチャパス
				parameter["position"] = branchButton->GetPosition();						// 座標
				parameter["scale"] = branchButton->GetScale();								// 大きさ
				parameter["textColor"] = branchButton->GetTextColor();						// テキストの色
			}
			else if (entry.objectType == ObjectType::Jump)	// ジャンプ
			{
				CreateJump* jump = dynamic_cast<CreateJump*>(entry.listObject);
				if (jump == nullptr) { continue; }

				parameter["targetName"] = Utf16ToUtf8(jump->GetTargetRavelName());	// 目的のラベル名
			}

			if (entry.objectType != ObjectType::Type_None)
			{
				m_objectEntryData.push_back({ parameter, ToString(entry.objectType) });
			}
		}
	}



	void SaveCharaNames()
	{
		/* 前回のデータをクリア */
		m_charaNameData.clear();

		for (const std::wstring& charaName : ImGuiManager::GetCharaNames())
		{
			YAML::Node charaNameData{};

			charaNameData["name"] = Utf16ToUtf8(charaName);

			m_charaNameData.push_back(charaNameData);
		}
	}



	/* string → watring変換用 */
	std::wstring Utf8ToUtf16(const std::string& utf8)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(utf8);
	}



	/* wstring → string変換用 */
	std::string Utf16ToUtf8(const std::wstring& utf16)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(utf16);
	}



	std::string ToString(ObjectType type)
	{
		switch (type)
		{
		case ObjectType::SpeakText: return "SpeakText";
		case ObjectType::BgTexture: return "BgTexture";
		case ObjectType::CharaAppearance: return "CharaAppearance";
		case ObjectType::CharaChange: return "CharaChange";
		case ObjectType::CharaExit: return "CharaExit";
		case ObjectType::BGM: return "BGM";
		case ObjectType::SE: return "SE";
		case ObjectType::GameStop: return "GameStop";
		case ObjectType::Ravel: return "Ravel";
		case ObjectType::BranchButton: return "BranchButton";
		case ObjectType::Jump: return "Jump";
		default: return"";
		}
	}
};

#endif //_SAVE_BUTTON_COMPONENT_H