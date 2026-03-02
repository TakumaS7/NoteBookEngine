/*===============================================================*/
//
//	projectLoadComponent.h(プロジェクトのロードコンポーネント)
// 
// 
// ---------------------------------- 2026/02/16 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PROJECT_LOAD_COMPONENT_H
#define _PROJECT_LOAD_COMPONENT_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "component.h"
#include "yamlConvert.h"
#include "yaml-cpp/yaml.h"
#include "imGuiManager.h"
#include <string>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
#include "createList.h"
#include "transform.h"
#include "createList.h"
#include "manager.h"
#include "scene.h"

class ProjectLoadComponent : public Component
{
public:
	bool m_loadStart = true;
	std::vector<std::wstring> m_charaNames;

private:
	/* string → watring変換用 */
	std::wstring Utf8ToUtf16(const std::string& utf8)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(utf8);
	}

	/* 文字列をenumに変換 */
	ObjectType ToEnum(std::string string)
	{
		if (string == "SpeakText") { return ObjectType::SpeakText; }
		if (string == "BgTexture") { return ObjectType::BgTexture; }
		if (string == "CharaAppearance") { return ObjectType::CharaAppearance; }
		if (string == "CharaChange") { return ObjectType::CharaChange; }
		if (string == "CharaExit") { return ObjectType::CharaExit; }
		if (string == "BGM") { return ObjectType::BGM; }
		if (string == "SE") { return ObjectType::SE; }
		if (string == "GameStop") { return ObjectType::GameStop; }
		if (string == "Ravel") { return ObjectType::Ravel; }
		if (string == "BranchButton") { return ObjectType::BranchButton; }
		if (string == "Jump") { return ObjectType::Jump; }
		if (string == "SpeakTextHidden") { return ObjectType::SpeakTextHidden; }
		if (string == "SpeakTextDisplay") { return ObjectType::SpeakTextDisplay; }

		return ObjectType::Type_None;
	}

	/* ファイルの移動などで、パスが変わったときのために更新 */
	std::wstring ResetPath(std::wstring setPath, ObjectType type)
	{
		fs::path path = setPath;
		fs::path fileName = path.filename();
		fs::path updatePath;
		switch (type)
		{
			case ObjectType::BgTexture:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildBgTexture" / fileName;
				break;
			case ObjectType::CharaAppearance:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildCharaTexture" / fileName;
				break;
			case ObjectType::CharaChange:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildCharaTexture" / fileName;
				break;
			case ObjectType::BGM:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildBgm" / fileName;
				break;
			case ObjectType::SE:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildSe" / fileName;
				break;
			case ObjectType::BranchButton:
				updatePath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / "nb_asset" / "BuildBranchButton" / fileName;
				break;
			default:
				return setPath;
		}

		return updatePath.wstring();
	}

	void Init() override 
	{
		m_loadStart = true;
	}

	void Uninit() override {}
	void Update() override 
	{
		if (m_loadStart == false) { return; }
		fs::path projectPath = ImGuiManager::GetProjectRoot() / "myProject" / ImGuiManager::GetSelectProjectName() / fs::path("nb_saveData");	// セーブデータの格納先

		/* 無ければ作成 */
		try
		{
			fs::create_directories(projectPath);
		}
		catch (const fs::filesystem_error& e)
		{
			MessageBox(nullptr, e.what(), "ファイルシステムエラー", MB_OK);
		}

		fs::path yamlPath = projectPath / (ImGuiManager::GetSelectProjectNameString() + ".yaml");

		/* 無ければ作成 */
		if (!fs::exists(yamlPath))
		{
			std::ofstream yaml(yamlPath.string());
			yaml << "objectEntry: []\n";
			yaml << "charaNames: []\n";
		}

		/* ロード */
		YAML::Node config;
		try
		{
			config = YAML::LoadFile(yamlPath.string());
		}
		catch (const YAML::Exception& e)
		{
			MessageBoxA(nullptr, e.what(), "YAML Load Error", MB_OK);
			return;
		}

		if (config.IsNull()) { return; }

		CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

		if (!createList) { return; }

		/* ObjectEntryデータのロード */
		YAML::Node objectEntryData = config["objectEntry"];

		if (objectEntryData && objectEntryData.IsSequence())
		{
			for (auto data : objectEntryData)
			{
				try
				{
					if (!data["objectType"]) { continue; }

					std::string objectTypeData = data["objectType"].as<std::string>();
					YAML::Node objectData = data["listObjectData"];

					ObjectType type = ToEnum(objectTypeData);

					switch (type)
					{
					case ObjectType::SpeakText:
					{
						std::wstring name = Utf8ToUtf16(objectData["name"].as<std::string>());
						std::wstring text = Utf8ToUtf16(objectData["text"].as<std::string>());
						createList->AddSpeakText(name, text);
						break;

					}
					case ObjectType::BgTexture:
					{
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						createList->AddBgTexture(path);
						break;
					}
					case ObjectType::CharaAppearance:
					{
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						XMFLOAT2 pos = objectData["position"].as<XMFLOAT2>();
						float rot = objectData["rotation"].as<float>();
						XMFLOAT2 scale = objectData["scale"].as<XMFLOAT2>();
						std::wstring name = Utf8ToUtf16(objectData["name"].as<std::string>());
						createList->AddCharaAppearance(path, pos, rot, scale, name);
						break;
					}
					case ObjectType::CharaChange:
					{
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						XMFLOAT2 pos = objectData["position"].as<XMFLOAT2>();
						float rot = objectData["rotation"].as<float>();
						XMFLOAT2 scale = objectData["scale"].as<XMFLOAT2>();
						std::wstring name = Utf8ToUtf16(objectData["name"].as<std::string>());
						createList->AddCharaChange(path, pos, rot, scale, name);
						break;
					}
					case ObjectType::CharaExit:
					{
						std::wstring name = Utf8ToUtf16(objectData["name"].as<std::string>());
						createList->AddCharaExit(name);
						break;
					}
					case ObjectType::BGM:
					{
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						createList->AddBgm(path);
						break;
					}
					case ObjectType::SE:
					{
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						createList->AddSe(path);
						break;
					}
					case ObjectType::GameStop:
					{
						createList->AddGameStop();
						break;
					}
					case ObjectType::Ravel:
					{
						std::wstring name = Utf8ToUtf16(objectData["name"].as<std::string>());
						createList->AddRavel(name);
						break;
					}
					case ObjectType::BranchButton:
					{
						std::wstring targetName = Utf8ToUtf16(objectData["targetName"].as <std::string>());
						std::wstring text = Utf8ToUtf16(objectData["text"].as<std::string>());
						std::wstring path = ResetPath(Utf8ToUtf16(objectData["path"].as<std::string>()), type);
						XMFLOAT2 pos = objectData["position"].as<XMFLOAT2>();
						XMFLOAT2 scale = objectData["scale"].as<XMFLOAT2>();
						Vector3 textColor = objectData["textColor"].as<Vector3>();
						createList->AddBranchButton(targetName, text, path, pos, scale, textColor);
						break;
					}
					case ObjectType::Jump:
					{
						std::wstring targetName = Utf8ToUtf16(objectData["targetName"].as<std::string>());
						createList->AddJump(targetName);
						break;
					}
					case ObjectType::SpeakTextHidden:
					{
						createList->AddSpeakTextHidden();
						break;
					}
					case ObjectType::SpeakTextDisplay:
					{
						createList->AddSpeakTextDisplay();
						break;
					}

					default:
						continue;
					}
				}
				catch (const YAML::Exception& e)
				{
					/* 壊れたデータはスキップ */
					continue;
				}
			}
		}

		/* CharaNameデータのロード */
		YAML::Node charaNameData = config["charaNames"];
		m_charaNames.clear();

		if (charaNameData && charaNameData.IsSequence())
		{
			for (auto charaName : charaNameData)
			{
				try
				{
					m_charaNames.push_back(Utf8ToUtf16(charaName["name"].as<std::string>()));
				}
				catch (const YAML::Exception&)
				{
					continue;
				}
			}
		}

		ImGuiManager::SetCharaNames(m_charaNames);

		m_loadStart = false;	// 一回のロードで終了
	}
	void Draw() override{}
};

#endif //_PROJECT_LOAD_COMPONENT_H