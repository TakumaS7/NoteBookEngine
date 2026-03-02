/*===============================================================*/
//
//	imGuiManager.cpp(ImGui管理)
// 
// 
// ---------------------------------- 2025/09/29 製作者：佐藤琢磨
//
/*===============================================================*/
#include "imGuiManager.h"
#include <iostream>
#include <codecvt>
#include <locale>
#include <algorithm>
#include "manager.h"
#include "scene.h"
#include "gameObject.h"
#include "createList.h"
#include "createRavel.h"
#include "textComponent.h"
#include <experimental/filesystem>
#include "texture.h"
#include "mouseCollision.h"
#include "fade.h"

/* static変数宣言 */
//std::wstring ImGuiManager::m_inputBuffer;
//std::wstring ImGuiManager::m_compositionBuffer;
ImGuiManager::ImeBuffer ImGuiManager::m_imeName;
ImGuiManager::ImeBuffer ImGuiManager::m_imeText;
ImGuiManager::ImeBuffer ImGuiManager::m_imeCharaName;
ImGuiManager::ImeBuffer ImGuiManager::m_imeRavelName;
ImGuiManager::ImeBuffer ImGuiManager::m_imeBranchButtonText;
ImGuiManager::ImeBuffer* ImGuiManager::m_imeActive;

fs::path ImGuiManager::m_projectRoot;                   // プロジェクトフォルダのパス
std::wstring ImGuiManager::m_selectedBgTexture;         // エディタ内の選んだ背景画像名
std::wstring ImGuiManager::m_selectedCharaName;         // エディタ内の選んだキャラの名前
std::wstring ImGuiManager::m_selectedCharaTexture;      // エディタ内の選んだキャラ画像名
std::wstring ImGuiManager::m_selectedBgm;               // エディタ内の選んだBGM
std::wstring ImGuiManager::m_selectedSe;                // エディタ内の選んだSE
std::wstring ImGuiManager::m_selectRavelButtonTexture;  // エディタ内の選んだ分岐ボタンテクスチャー
std::wstring ImGuiManager::m_selectRavelName;           // エディタ内の選んだラベル名

std::wstring ImGuiManager::m_selectProjectPath;    // 選択されたプロジェクトフォルダパス
std::wstring ImGuiManager::m_selectProjectName;    // 選択されたプロジェクト名

XMFLOAT2 ImGuiManager::m_charaPosition = { 0.0f, 0.0f };    // キャラの配置ポジション
float ImGuiManager::m_charaRotation = 0.0f;                 // キャラの回転(度表記、ラジアンではない)
XMFLOAT2 ImGuiManager::m_charaScale = { 100.0f, 100.0f };   // キャラの大きさ
std::vector<std::wstring> ImGuiManager::m_charaNames;       // キャラの名前

XMFLOAT2 ImGuiManager::m_branchButtonPosition = { 0.0f, 0.0f }; // 分岐ボタンの配置ポジション
float ImGuiManager::m_branchButtonRotation = 0.0f;              // 分岐ボタンの回転(度表記、ラジアンではない)
XMFLOAT2 ImGuiManager::m_branchButtonScale = { 40.0f, 40.0f };  // 分岐ボタンの大きさ

Vector3 ImGuiManager::m_BranchButtonTextColor = { 0.0f, 0.0f, 0.0f };  // 分岐ボタンテキストのカラーコード

bool ImGuiManager::m_charaEditorOpen = false;           // キャラエディター開くフラグ
bool ImGuiManager::m_branchButtonEditorOpen = false;    // 分岐ボタンエディター開くフラグ

bool ImGuiManager::m_projectSelectOpen = false;     // プロジェクト選択画面の開閉フラグ
bool ImGuiManager::m_createNewProjectOpen = false;  // プロジェクト新規作成画面の開閉フラグ


void ImGuiManager::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(device, deviceContext);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->Build();

    ImGui::StyleColorsDark();

    m_imeActive = nullptr;
}

void ImGuiManager::BegineFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}



void ImGuiManager::DrawCreate()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(310.0f, SCREEN_HEIGHT), ImGuiCond_Always);

    ImGui::Begin(u8"作業ボックス");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);



    /* ヘッダーの色変更（赤） */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));

    if (ImGui::CollapsingHeader(u8"テキスト"))
    {
        TextEditor();
    }
    ImGui::PopStyleColor(3);


    /* ヘッダーの色変更（緑） */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));

    if (ImGui::CollapsingHeader(u8"背景"))
    {
        BgEditor();
    }
    ImGui::PopStyleColor(3);


    /* ヘッダーの色変更（黄色） */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.7f, 0.7f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.6f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.5f, 0.5f, 0.1f, 1.0f));

    if (ImGui::CollapsingHeader(u8"キャラクター"))
    {
        CharaEditor();
    }
    ImGui::PopStyleColor(3);


    /* ヘッダーの色変更（オレンジ） */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.7f, 0.4f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.3f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.5f, 0.2f, 0.1f, 1.0f));

    if (ImGui::CollapsingHeader(u8"サウンド"))
    {
        SoundEditor();
    }
    ImGui::PopStyleColor(3);


    /* ヘッダーの色変更（青） */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1f, 0.1f, 0.7f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1f, 0.1f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1f, 0.1f, 0.5f, 1.0f));

    if (ImGui::CollapsingHeader(u8"システム"))
    {
        SystemEditor();
    }
    ImGui::PopStyleColor(3);


    /* ヘッダーの色変更 */
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    if (ImGui::CollapsingHeader(u8"ヒエラルキー"))
    {
        HierarchyEditor();
    }
    ImGui::PopStyleColor(3);



    ImGui::End();

    if (!ImGui::IsAnyItemActive())
    {
        m_imeActive = nullptr;
    }
}

void ImGuiManager::DrawProjectSelect()
{
    if (m_projectSelectOpen == true)
    {
        ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.8f), ImGuiCond_Always);

        ImGui::SetNextWindowBgAlpha(1.0f);

        ImGui::Begin(u8"プロジェクト選択", nullptr, 
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        /* 上固定エリア */
        ImGui::SetCursorPos(ImVec2(890.0f, 30.0f));
        if (ImGui::Button(u8"プロジェクト新規作成"))
        {
            m_createNewProjectOpen = true;
        }

        /* プロジェクト新規作成画面を開く */
        if (m_createNewProjectOpen == true)
        {
            CreateNewProject();
        }

        /* 少しの余白 */
        ImGui::Dummy(ImVec2(0.0f, 20.0f));



        /* スクロールエリア */
        float footerHeight = 50.0f; // 下ボタン分
        /* 残りの高さを取得 */
        float childHeight = ImGui::GetContentRegionAvail().y - footerHeight;

        ImGui::BeginChild("ProjectList", ImVec2(0, childHeight), true);

        ShowProjectSelecter(m_selectProjectPath);

        ImGui::EndChild();



        /* 下固定エリア */
        ImGui::TextWrapped(u8"選択中：%s", Utf16ToUtf8(m_selectProjectPath).c_str());

        if (ImGui::Button(u8"戻る", ImVec2(60.0f, 20.0f)))
        {
            m_projectSelectOpen = false;
        }

        ImGui::SameLine(0.0f, 885.0f);

        if (ImGui::Button(u8"開く", ImVec2(60.0f, 20.0f)))
        {
            /* プロジェクトを開く */
            if (!m_selectProjectPath.empty())
            {
                /* 開いたプロジェクト名を記録する */
                fs::path projectName(m_selectProjectPath);
                m_selectProjectName = projectName.filename().wstring();
                
                Fade::SetFadeScene(Fade::CREATE_SCENE);
            }
        }



        ImGui::End();
    }
}

void ImGuiManager::EndFrame()
{
    /* ImGuiの機能にマウスが触れていたらマウス操作停止(false) */
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
    {
        Mouse::SetClickFlag(false);
        Mouse::SetScrollFlag(false);

        /* マウスの当たり判定も消す */
        for (GameObject* gameObject : Manager::GetScene()->GetAllGameObject2D())
        {
            MouseCollision* mouseHit = gameObject->GetComponent<MouseCollision>();
            if (mouseHit == nullptr) { continue; }
            mouseHit->SetOnImGui(true);
        }
    }
    else
    {
        Mouse::SetClickFlag(true);
        Mouse::SetScrollFlag(true);

        /* マウスの当たり判定も消す */
        for (GameObject* gameObject : Manager::GetScene()->GetAllGameObject2D())
        {
            MouseCollision* mouseHit = gameObject->GetComponent<MouseCollision>();
            if (mouseHit == nullptr) { continue; }
            mouseHit->SetOnImGui(false);
        }
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}



std::string ImGuiManager::GetSelectProjectNameString()
{
    return Utf16ToUtf8(m_selectProjectName);
}

void ImGuiManager::TextEditor()
{
    /* 日本語入力 */
    std::wstring wrightText;
    std::wstring wrightName;
    InputNameW(u8"：名前入力", wrightName);
    InputTextW(u8"：セリフ入力", wrightText);

    if (ImGui::Button(u8"テキスト生成")) {
        /* 生成 */
        Manager::GetScene()->GetGameObject<CreateList>()->AddSpeakText(wrightName, wrightText);

        m_imeText.inputBuffer.erase();
    }
}

void ImGuiManager::BgEditor()
{
    if (ImGui::Button(u8"背景画像取り込み"))
    {
        /* フォルダ参照 */
        std::wstring path = ImGuiManager::OpenFileDialogTexture();

        if (!path.empty())
        {
            /* コピーを実行 */
            if (ImGuiManager::CopyToBgTextureFolder(path))
            {
                MessageBoxW(nullptr, L"画像をプロジェクトフォルダにコピーしました！", L"完了", MB_OK);
            }
        }
    }

    if (ImGui::Button(u8"取り込み背景画像フォルダ参照"))
    {
        ImGuiManager::OpenBuildBgTextureFolder();
    }

    if (ImGui::TreeNode(u8"背景画像生成"))
    {
        if (ImGui::BeginMenu(u8"取り込み背景画像一覧"))
        {
            ShowBgTextureSelector(m_selectedBgTexture);

            ImGui::EndMenu();
        }

        ImGui::TextWrapped(u8"選択中：%s", Utf16ToUtf8(m_selectedBgTexture).c_str());

        if (ImGui::Button(u8"選択背景画像生成"))
        {
            if (!m_selectedBgTexture.empty()) {
                Manager::GetScene()->GetGameObject<CreateList>()->AddBgTexture(m_selectedBgTexture);
                m_selectedBgTexture.clear();
            }
        }

        ImGui::TreePop();
    }
}



void ImGuiManager::CharaEditor()
{
    if (ImGui::Button(u8"キャラ画像取り込み"))
    {
        /* フォルダ参照 */
        std::wstring path = ImGuiManager::OpenFileDialogTexture();

        if (!path.empty())
        {
            /* コピーを実行 */
            if (ImGuiManager::CopyToCharaTextureFolder(path))
            {
                MessageBoxW(nullptr, L"画像をプロジェクトフォルダにコピーしました！", L"完了", MB_OK);
            }
        }
    }

    if (ImGui::Button(u8"取り込みキャラ画像フォルダ参照"))
    {
        ImGuiManager::OpenBuildCharaTextureFolder();
    }

    if (ImGui::BeginTabBar("キャラクターテーブル"))
    {
        if (ImGui::BeginTabItem(u8"登録"))
        {
            std::wstring charaName;
            ImGuiManager::InputCharaNameW(u8"キャラ名", charaName);

            if (ImGui::Button(u8"キャラ登録"))
            {
                /* キャラ名の登録 */
                if (!charaName.empty())
                {
                    m_charaNames.push_back(charaName);
                    m_imeCharaName.inputBuffer.erase();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"削除"))
        {
            if (ImGui::BeginMenu(u8"削除するキャラを選択"))
            {
                ShowCharaNameSelector(m_selectedCharaName);

                ImGui::EndMenu();
            }

            ImGui::TextWrapped(u8"選択中：%s", Utf16ToUtf8(m_selectedCharaName).c_str());

            if (ImGui::Button(u8"選択中のキャラ削除"))
            {
                if (!m_selectedCharaName.empty()) {
                    CharaDelete(m_selectedCharaName);
                    m_selectedCharaName.clear();
                }
            }

            ImGui::EndTabItem();
        }

        /* キャラ登場、キャラ状態変更、キャラ退場ツリーノードの開閉状態はは排他的にする */
        if (ImGui::BeginTabItem(u8"登場"))
        {
            CharaAppearanceEditor();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"状態変更"))
        {
            CharaChangeEditor();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"退場"))
        {
            CharaExitEditor();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void ImGuiManager::SoundEditor()
{
    if (ImGui::BeginTabBar("サウンドテーブル"))
    {
        if (ImGui::BeginTabItem(u8"BGM"))
        {
            BgmEditor();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(u8"SE"))
        {
            SeEditor();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void ImGuiManager::SystemEditor()
{
    if (ImGui::TreeNode(u8"分岐システム"))
    {
        if (ImGui::BeginTabBar("分岐システムテーブル"))
        {
            if (ImGui::BeginTabItem(u8"ゲーム停止"))
            {
                if (ImGui::Button(u8"生成"))
                {
                    Manager::GetScene()->GetGameObject<CreateList>()->AddGameStop();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(u8"ラベル"))
            {
                std::wstring ravelName;
                ImGuiManager::InputRavelNameW(u8"ラベル名", ravelName);

                if (ImGui::Button(u8"ラベル生成"))
                {
                    if (!ravelName.empty())
                    {
                        Manager::GetScene()->GetGameObject<CreateList>()->AddRavel(ravelName);
                        m_imeRavelName.inputBuffer.erase();
                    }
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(u8"分岐ボタン"))
            {
                BranchButtonEditor();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(u8"ジャンプ"))
            {
                if (ImGui::BeginMenu(u8"ラベル選択"))
                {
                    ShowRavelSelector(m_selectRavelName);

                    ImGui::EndMenu();
                }

                ImGui::TextWrapped(u8"ラベル選択中：%s", Utf16ToUtf8(m_selectRavelName).c_str());

                if (ImGui::Button(u8"生成"))
                {
                    if (!m_selectRavelName.empty())
                    {
                        Manager::GetScene()->GetGameObject<CreateList>()->AddJump(m_selectRavelName);
                        m_selectRavelName.erase();
                    }
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode(u8"テキストシステム"))
    {
        if (ImGui::BeginTabBar("テキストフレームシステムテーブル"))
        {
            if (ImGui::BeginTabItem(u8"テキストフレーム表示"))
            {
                if (ImGui::Button(u8"表示生成"))
                {
                    Manager::GetScene()->GetGameObject<CreateList>()->AddSpeakTextDisplay();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(u8"テキストフレーム非表示"))
            {
                if (ImGui::Button(u8"非表示生成"))
                {
                    Manager::GetScene()->GetGameObject<CreateList>()->AddSpeakTextHidden();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

		ImGui::TreePop();
    }
}



void ImGuiManager::HierarchyEditor()
{
    auto& list = Manager::GetScene()->GetGameObject<CreateList>()->GetListObjects();

    int i = 0;

    for (auto entry : list) {
        if (!entry.listObject || entry.listObject->GetDestroy()) { continue; }

        const wchar_t* type{};
        /* 文字変換 */
        switch (entry.objectType)
        {
        case ObjectType::SpeakText: type = L"SpeakText"; break;
        case ObjectType::BgTexture: type = L"BgTexture"; break;
        case ObjectType::CharaAppearance: type = L"CharaAppearance"; break;
        case ObjectType::CharaChange: type = L"CharaChange"; break;
        case ObjectType::CharaExit: type = L"CharaExit"; break;
        case ObjectType::BGM: type = L"BGM"; break;
        case ObjectType::SE: type = L"SE"; break;
        case ObjectType::GameStop: type = L"GameStop"; break;
        case ObjectType::Ravel: type = L"Ravel"; break;
        case ObjectType::BranchButton: type = L"BranchButton"; break;
        case ObjectType::Jump: type = L"Jump"; break;
        default: type = L""; break;
        }

        const wchar_t* text = entry.listObject->GetComponent<TextComponent>()->GetText().c_str();

        std::wstring wstrText(text);
        std::wstring wstrType(type);

        std::string utf8Text = Utf16ToUtf8(wstrText);
        std::string utf8Type = Utf16ToUtf8(wstrType);

        ImGui::TextWrapped(u8"[%d] %s：%s", i++, utf8Type.c_str(), utf8Text.c_str());
    }
}



void ImGuiManager::CharaDelete(std::wstring& selectCharaName)
{
    /* 登録した名前から探して削除する（一回のみ） */
    auto iterator = std::find(m_charaNames.begin(), m_charaNames.end(), selectCharaName);

    if (iterator != m_charaNames.end())
    {
        m_charaNames.erase(iterator);
    }
}



void ImGuiManager::CharaAppearanceEditor()
{
    if (ImGui::BeginMenu(u8"キャラ選択"))
    {
        ShowCharaNameSelector(m_selectedCharaName);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"キャラ選択中：%s", Utf16ToUtf8(m_selectedCharaName).c_str());

    if (ImGui::BeginMenu(u8"取り込みキャラ画像一覧"))
    {
        ShowCharaTextureSelector(m_selectedCharaTexture);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"画像選択中：%s", Utf16ToUtf8(m_selectedCharaTexture).c_str());

    ImGui::DragFloat2(u8"座標(x, y)", &m_charaPosition.x, 0.1f);
    ImGui::DragFloat(u8"回転(度)", &m_charaRotation, 0.1f, 0.0f, 360.0f);
    ImGui::DragFloat2(u8"サイズ(x, y)", &m_charaScale.x, 0.1f, 0.0f, 9999.0f);

    if (ImGui::Button(u8"選択キャラ位置調整"))
    {
        m_charaEditorOpen = true;
    }

    /* キャラのさらに細かな設定を開く */
    if (m_charaEditorOpen)
    {
        CharaEditorOpem();  // キャラ詳細設定画面を開く

        ImGui::SetNextWindowSize(ImVec2(310.0f, 210.0f), ImGuiCond_Always);
        ImGui::Begin(u8"キャラ調整");
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

        ImGui::DragFloat2(u8"座標(x, y)", &m_charaPosition.x, 0.1f);
        ImGui::DragFloat(u8"回転(度)", &m_charaRotation, 0.1f, 0.0f, 360.0f);
        ImGui::DragFloat2(u8"サイズ(x, y)", &m_charaScale.x, 0.1f, 0.0f, 9999.0f);

        if (ImGui::Button(u8"キャラ調整画面を閉じる"))
        {
            m_charaEditorOpen = false;
        }

        ImGui::End();
    }


    if (ImGui::Button(u8"キャラの登場"))
    {
        if (!m_selectedCharaTexture.empty() && !m_selectedCharaName.empty()) {
            Manager::GetScene()->GetGameObject<CreateList>()->AddCharaAppearance(m_selectedCharaTexture, m_charaPosition, m_charaRotation, m_charaScale, m_selectedCharaName);
            m_selectedCharaName.clear();
            m_selectedCharaTexture.clear();
        }
    }
}

void ImGuiManager::CharaChangeEditor()
{
    if (ImGui::BeginMenu(u8"キャラ選択"))
    {
        ShowCharaNameSelector(m_selectedCharaName);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"キャラ選択中：%s", Utf16ToUtf8(m_selectedCharaName).c_str());

    if (ImGui::BeginMenu(u8"取り込みキャラ画像一覧"))
    {
        ShowCharaTextureSelector(m_selectedCharaTexture);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"画像選択中：%s", Utf16ToUtf8(m_selectedCharaTexture).c_str());

    ImGui::DragFloat2(u8"座標(x, y)", &m_charaPosition.x, 0.1f);
    ImGui::DragFloat(u8"回転(度)", &m_charaRotation, 0.1f, 0.0f, 360.0f);
    ImGui::DragFloat2(u8"サイズ(x, y)", &m_charaScale.x, 0.1f, 0.0f, 9999.0f);

    if (ImGui::Button(u8"選択キャラ位置調整"))
    {
        m_charaEditorOpen = true;
    }

    /* キャラのさらに細かな設定を開く */
    if (m_charaEditorOpen)
    {
        CharaEditorOpem();  // キャラ詳細設定画面を開く

        ImGui::SetNextWindowSize(ImVec2(310.0f, 210.0f), ImGuiCond_Always);
        ImGui::Begin(u8"キャラ調整");
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

        ImGui::DragFloat2(u8"座標(x, y)", &m_charaPosition.x, 0.1f);
        ImGui::DragFloat(u8"回転(度)", &m_charaRotation, 0.1f, 0.0f, 360.0f);
        ImGui::DragFloat2(u8"サイズ(x, y)", &m_charaScale.x, 0.1f, 0.0f, 9999.0f);

        if (ImGui::Button(u8"キャラ調整画面を閉じる"))
        {
            m_charaEditorOpen = false;
        }

        ImGui::End();
    }


    if (ImGui::Button(u8"キャラの状態変更"))
    {
        if (!m_selectedCharaTexture.empty() && !m_selectedCharaName.empty()) {
            Manager::GetScene()->GetGameObject<CreateList>()->AddCharaChange(m_selectedCharaTexture, m_charaPosition, m_charaRotation, m_charaScale, m_selectedCharaName);
            m_selectedCharaName.clear();
            m_selectedCharaTexture.clear();
        }
    }
}

void ImGuiManager::CharaExitEditor()
{
    if (ImGui::BeginMenu(u8"キャラ選択"))
    {
        ShowCharaNameSelector(m_selectedCharaName);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"キャラ選択中：%s", Utf16ToUtf8(m_selectedCharaName).c_str());

    if (ImGui::Button(u8"キャラの退場"))
    {
        if (!m_selectedCharaName.empty())
        {
            Manager::GetScene()->GetGameObject<CreateList>()->AddCharaExit(m_selectedCharaName);
            m_selectedCharaName.clear();
        }
    }
}

void ImGuiManager::CharaEditorOpem()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT), ImGuiCond_Always);

    ImGui::Begin(u8"キャラエディタ", &m_charaEditorOpen, ImGuiWindowFlags_NoCollapse);

    ImVec2 canvasSize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* 背景 */
    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(
        { 0.0f, 0.0f },
        canvasSize,
        IM_COL32(120.0f, 120.0f, 180.0f, 255.0f)
    );

    /* オブジェクト */
    auto srv = Texture::EditorGet(m_selectedCharaTexture);

    /* テクスチャがセットされているなら */
    AddImageTransform(
        draw,
        (ImTextureID)srv,
        ImVec2(m_charaPosition.x, m_charaPosition.y),
        m_charaRotation,
        ImVec2(m_charaScale.x, m_charaScale.y)
    );

    ImVec2 objMin = ImVec2(m_charaPosition.x - m_charaScale.x, m_charaPosition.y - m_charaScale.y);
    ImVec2 objMax = ImVec2(m_charaPosition.x + m_charaScale.x, m_charaPosition.y + m_charaScale.y);
    float centerX = (objMin.x + objMax.x) * 0.5f;
    float centerY = (objMin.y + objMax.y) * 0.5f;


    /* ここから拡大縮小用の枠線の処理 */
    /* 枠線の描画 */
    draw->AddRect(
        objMin,
        objMax,
        IM_COL32(255, 255, 255, 255),
        0.0f,
        0,
        2.0f
    );

    /* ハンドル部分の大きさ */
    const float HANDLE_SIZE = 5.0f;

    /* 斜めのハンドル（全体を拡大縮小する用） */
    ImVec2 handlesDiagonal[4] =
    {
        objMin,                                  // 左上
        { objMax.x, objMin.y },                  // 右上
        objMax,                                  // 右下
        { objMin.x, objMax.y }                   // 左下
    };

    /* 上下左右のハンドル（XとYに分かれた拡大縮小用） */
    ImVec2 handlesHorizon[4] =
    {
        { centerX, objMin.y }, // 上
        { objMax.x, centerY }, // 右
        { centerX, objMax.y }, // 下
        { objMin.x, centerY }  // 左
    };

    /* 斜めハンドルの処理 */
    for (int i = 0; i < 4; i++)
    {
        /* 描画 */
        draw->AddRectFilled(
            ImVec2(handlesDiagonal[i].x - HANDLE_SIZE, handlesDiagonal[i].y - HANDLE_SIZE),
            ImVec2(handlesDiagonal[i].x + HANDLE_SIZE, handlesDiagonal[i].y + HANDLE_SIZE),
            IM_COL32(255, 255, 255, 255)
        );

        /* 入力（下のInvisibleButtonの範囲に適用される） */
        ImGui::SetCursorScreenPos(
            ImVec2(handlesDiagonal[i].x - HANDLE_SIZE, handlesDiagonal[i].y - HANDLE_SIZE)
        );

        ImGui::PushID(i);   // InvisibleButtonがi（一つの四角）ごとに区別するために使用
        ImGui::InvisibleButton("resizeDiagonal", ImVec2(HANDLE_SIZE * 2, HANDLE_SIZE * 2));

        /* 左クリック中 */
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            /* マウスポジションの差分（全体の拡大縮小なのでyの値だけで十分） */
            float delta = ImGui::GetIO().MouseDelta.y;

            /* 綺麗な拡大縮小にするために比率計算 */
			float scaleFactor = 0.005f;   // 拡大縮小の感度
            float deltaX = delta * m_charaScale.x * scaleFactor;
			float deltaY = delta * m_charaScale.y * scaleFactor;

            /* 上側はマイナス */
            float sign = (i == 0 || i == 1) ? -1.0f : 1.0f;

            /* 拡大縮小 */
            m_charaScale.x += deltaX * sign;
            m_charaScale.y += deltaY * sign;

            m_charaScale.x = std::max(m_charaScale.x, 1.0f);
            m_charaScale.y = std::max(m_charaScale.y, 1.0f);
        }
        ImGui::PopID();
    }

    /* 上下左右ハンドルの処理 */
    for (int i = 0; i < 4; i++)
    {
        /* 描画 */
        draw->AddRectFilled(
            ImVec2(handlesHorizon[i].x - HANDLE_SIZE, handlesHorizon[i].y - HANDLE_SIZE),
            ImVec2(handlesHorizon[i].x + HANDLE_SIZE, handlesHorizon[i].y + HANDLE_SIZE),
            IM_COL32(255, 255, 255, 255)
        );

        /* 入力（下のInvisibleButtonの範囲に適用される） */
        ImGui::SetCursorScreenPos(
            ImVec2(handlesHorizon[i].x - HANDLE_SIZE, handlesHorizon[i].y - HANDLE_SIZE)
        );

        ImGui::PushID(i);   // InvisibleButtonがi（一つの四角）ごとに区別するために使用
        ImGui::InvisibleButton("resizeHorizon", ImVec2(HANDLE_SIZE * 2, HANDLE_SIZE * 2));

        /* 左クリック中 */
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            /* マウスポジションの差分 */
            ImVec2 delta = ImGui::GetIO().MouseDelta;

            /* i（一つの四角）ごとに拡大縮小処理をする */
            switch (i)
            {
            case 0:
                m_charaScale.y += -delta.y;
                break;
            case 1:
                m_charaScale.x += delta.x;
                break;
            case 2:
                m_charaScale.y += delta.y;
                break;
            case 3:
                m_charaScale.x += -delta.x;
                break;
            }

            m_charaScale.x = std::max(m_charaScale.x, 1.0f);
            m_charaScale.y = std::max(m_charaScale.y, 1.0f);
        }
        ImGui::PopID();
    }


    /* ここからオブジェクト移動用 */
    ImGui::SetCursorScreenPos(objMin);  // 下のInvisibleButtonに適用される
    float w = objMax.x - objMin.x;
    float h = objMax.y - objMin.y;

    /* 下限に1をとる（InvisibleButtonが壊れるのを防ぐため） */
    w = std::max(w, 1.0f);
    h = std::max(h, 1.0f);

    ImGui::InvisibleButton("move", ImVec2(w, h));

    /* 左クリック中 */
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        /* マウスに追従して移動 */
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        m_charaPosition.x += delta.x;
        m_charaPosition.y += delta.y;
    }

    ImGui::End();
}

void ImGuiManager::BgmEditor()
{
    if (ImGui::Button(u8"BGM取り込み"))
    {
        /* フォルダ参照 */
        std::wstring path = ImGuiManager::OpenFileDialogSound();

        if (!path.empty())
        {
            /* コピーを実行 */
            if (ImGuiManager::CopyToBgmFolder(path))
            {
                MessageBoxW(nullptr, L"サウンドをプロジェクトフォルダにコピーしました！", L"完了", MB_OK);
            }
        }
    }

    if (ImGui::Button(u8"取り込みBGMフォルダ参照"))
    {
        ImGuiManager::OpenBuildBgmFolder();
    }

    if (ImGui::TreeNode(u8"BGM生成"))
    {
        if (ImGui::BeginMenu(u8"取り込みBGM一覧"))
        {
            ShowBgmSelector(m_selectedBgm);

            ImGui::EndMenu();
        }

        ImGui::TextWrapped(u8"選択中：%s", Utf16ToUtf8(m_selectedBgm).c_str());

        if (ImGui::Button(u8"選択BGM生成"))
        {
            if (!m_selectedBgm.empty()) {
                Manager::GetScene()->GetGameObject<CreateList>()->AddBgm(m_selectedBgm);
                m_selectedBgm.clear();
            }
        }

        ImGui::TreePop();
    }
}

void ImGuiManager::SeEditor()
{
    if (ImGui::Button(u8"SE取り込み"))
    {
        /* フォルダ参照 */
        std::wstring path = ImGuiManager::OpenFileDialogSound();

        if (!path.empty())
        {
            /* コピーを実行 */
            if (ImGuiManager::CopyToSeFolder(path))
            {
                MessageBoxW(nullptr, L"サウンドをプロジェクトフォルダにコピーしました！", L"完了", MB_OK);
            }
        }
    }

    if (ImGui::Button(u8"取り込みSEフォルダ参照"))
    {
        ImGuiManager::OpenBuildSeFolder();
    }

    if (ImGui::TreeNode(u8"SE生成"))
    {
        if (ImGui::BeginMenu(u8"取り込みSE一覧"))
        {
            ShowSeSelector(m_selectedSe);

            ImGui::EndMenu();
        }

        ImGui::TextWrapped(u8"選択中：%s", Utf16ToUtf8(m_selectedSe).c_str());

        if (ImGui::Button(u8"選択SE生成"))
        {
            if (!m_selectedSe.empty()) {
                Manager::GetScene()->GetGameObject<CreateList>()->AddSe(m_selectedSe);
                m_selectedSe.clear();
            }
        }

        ImGui::TreePop();
    }
}

void ImGuiManager::BranchButtonEditor()
{
    if (ImGui::Button(u8"分岐ボタン画像取り込み"))
    {
        /* フォルダ参照 */
        std::wstring path = ImGuiManager::OpenFileDialogTexture();

        if (!path.empty())
        {
            /* コピーを実行 */
            if (ImGuiManager::CopyToBranchButtonFolder(path))
            {
                MessageBoxW(nullptr, L"画像をプロジェクトフォルダにコピーしました！", L"完了", MB_OK);
            }
        }
    }

    if (ImGui::Button(u8"取り込み分岐ボタン画像フォルダ参照"))
    {
        ImGuiManager::OpenBuildBranchButtonFolder();
    }

    if (ImGui::BeginMenu(u8"分岐ボタン画像選択"))
    {
        ShowRavelTextureSelector(m_selectRavelButtonTexture);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"分岐ボタン画像選択中：%s", Utf16ToUtf8(m_selectRavelButtonTexture).c_str());

    if (ImGui::BeginMenu(u8"ラベル選択"))
    {
        ShowRavelSelector(m_selectRavelName);

        ImGui::EndMenu();
    }

    ImGui::TextWrapped(u8"ラベル選択中：%s", Utf16ToUtf8(m_selectRavelName).c_str());

    std::wstring buttonText;
    InputBranchButtonTextW(u8"テキスト", buttonText);

    if (ImGui::TreeNode(u8"ボタン位置・サイズ"))
    {
        ImGui::DragFloat2(u8"座標(x, y)", &m_branchButtonPosition.x, 0.1f);
        ImGui::DragFloat2(u8"サイズ(x, y)", &m_branchButtonScale.x, 0.1f, 0.0f, 9999.0f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode(u8"テキストカラー"))
    {
        /* テキスト色設定 */
        ImGui::DragFloat3(u8"カラーコード", &m_BranchButtonTextColor.x, 0.01f, 0.0f, 1.0f);
        ImGui::ColorEdit3(u8"テキストの色", (float*)&m_BranchButtonTextColor, ImGuiColorEditFlags_DisplayHex);

        ImGui::TreePop();
    }

    if (ImGui::Button(u8"分岐ボタン位置調整"))
    {
        m_branchButtonEditorOpen = true;
    }

    /* 分岐ボタンのさらに細かな設定を開く */
    if (m_branchButtonEditorOpen)
    {
        BranchButtonEditorOpen();  // 分岐ボタン詳細設定画面を開く

        ImGui::SetNextWindowSize(ImVec2(350.0f, 210.0f), ImGuiCond_Always);
        ImGui::Begin(u8"分岐ボタン調整");
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

        if (ImGui::TreeNode(u8"ボタン位置・サイズ"))
        {
            ImGui::DragFloat2(u8"座標(x, y)", &m_branchButtonPosition.x, 0.1f);
            ImGui::DragFloat2(u8"サイズ(x, y)", &m_branchButtonScale.x, 0.1f, 0.0f, 9999.0f);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(u8"テキストカラー"))
        {
            /* テキスト色設定 */
            ImGui::DragFloat3(u8"カラーコード", &m_BranchButtonTextColor.x, 0.01f, 0.0f, 1.0f);
            ImGui::ColorEdit3(u8"テキストの色", (float*)&m_BranchButtonTextColor, ImGuiColorEditFlags_DisplayHex);

            ImGui::TreePop();
        }

        if (ImGui::Button(u8"分岐ボタン調整画面を閉じる"))
        {
            m_branchButtonEditorOpen = false;
        }

        ImGui::End();
    }

    if (ImGui::Button(u8"分岐ボタン生成"))
    {
        if (!m_selectRavelName.empty() && !m_selectRavelButtonTexture.empty())
        {
            Manager::GetScene()->GetGameObject<CreateList>()->AddBranchButton(m_selectRavelName, buttonText, m_selectRavelButtonTexture, m_branchButtonPosition, m_branchButtonScale, m_BranchButtonTextColor);

            m_selectRavelButtonTexture.clear();
            m_selectRavelName.clear();
            m_imeBranchButtonText.inputBuffer.erase();
        }
    }
}

void ImGuiManager::BranchButtonEditorOpen()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT), ImGuiCond_Always);

    ImGui::Begin(u8"分岐ボタンエディタ", &m_branchButtonEditorOpen, ImGuiWindowFlags_NoCollapse);

    ImVec2 canvasSize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* 背景 */
    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(
        { 0.0f, 0.0f },
        canvasSize,
        IM_COL32(120.0f, 120.0f, 180.0f, 255.0f)
    );

    /* オブジェクト */
    auto srv = Texture::EditorGet(m_selectRavelButtonTexture);

    /* テクスチャがセットされているなら */
    AddImageTransform(
        draw,
        (ImTextureID)srv,
        ImVec2(m_branchButtonPosition.x, m_branchButtonPosition.y),
        m_branchButtonRotation,
        ImVec2(m_branchButtonScale.x, m_branchButtonScale.y)
    );


    ImVec2 objMin = ImVec2(m_branchButtonPosition.x - m_branchButtonScale.x, m_branchButtonPosition.y - m_branchButtonScale.y);
    ImVec2 objMax = ImVec2(m_branchButtonPosition.x + m_branchButtonScale.x, m_branchButtonPosition.y + m_branchButtonScale.y);
    float centerX = (objMin.x + objMax.x) * 0.5f;
    float centerY = (objMin.y + objMax.y) * 0.5f;



    /* 基準フォントを取得 */
    ImFont* font = ImGui::GetFont();
    float baseFontSize = font->FontSize;

    /* ボタンサイズからスケール率を作る */
    float textScale = m_branchButtonScale.y / 15.0f;

    /* 文字サイズの計算 */
    std::string textUtf8 = Utf16ToUtf8(m_imeBranchButtonText.inputBuffer);

    ImVec2 textSize = ImGui::CalcTextSize(textUtf8.c_str());
    textSize.x *= textScale;
    textSize.y *= textScale;

    ImVec2 textPos = {
        centerX - textSize.x * 0.5f,
        centerY - textSize.y * 0.5f,
    };

    /* 色を255基準に変換する */
    Vector3 textColor = {
        m_BranchButtonTextColor.x * 255.0f,
        m_BranchButtonTextColor.y * 255.0f,
        m_BranchButtonTextColor.z * 255.0f
    };

    /* テキスト表示 */
    draw->AddText(
        font,
        baseFontSize * textScale,
        textPos,                        // 表示位置（スクリーン座標）
        IM_COL32(textColor.x, textColor.y, textColor.z, 255.0f),   // 色
        textUtf8.c_str()                // 表示文字
    );



    /* ここから拡大縮小用の枠線の処理 */
    /* 枠線の描画 */
    draw->AddRect(
        objMin,
        objMax,
        IM_COL32(255, 255, 255, 255),
        0.0f,
        0,
        2.0f
    );

    /* ハンドル部分の大きさ */
    const float HANDLE_SIZE = 5.0f;

    /* 斜めのハンドル（全体を拡大縮小する用） */
    ImVec2 handlesDiagonal[4] =
    {
        objMin,                                  // 左上
        { objMax.x, objMin.y },                  // 右上
        objMax,                                  // 右下
        { objMin.x, objMax.y }                   // 左下
    };

    /* 上下左右のハンドル（XとYに分かれた拡大縮小用） */
    ImVec2 handlesHorizon[4] =
    {
        { centerX, objMin.y }, // 上
        { objMax.x, centerY }, // 右
        { centerX, objMax.y }, // 下
        { objMin.x, centerY }  // 左
    };

    /* 斜めハンドルの処理 */
    for (int i = 0; i < 4; i++)
    {
        /* 描画 */
        draw->AddRectFilled(
            ImVec2(handlesDiagonal[i].x - HANDLE_SIZE, handlesDiagonal[i].y - HANDLE_SIZE),
            ImVec2(handlesDiagonal[i].x + HANDLE_SIZE, handlesDiagonal[i].y + HANDLE_SIZE),
            IM_COL32(255, 255, 255, 255)
        );

        /* 入力（下のInvisibleButtonの範囲に適用される） */
        ImGui::SetCursorScreenPos(
            ImVec2(handlesDiagonal[i].x - HANDLE_SIZE, handlesDiagonal[i].y - HANDLE_SIZE)
        );

        ImGui::PushID(i);   // InvisibleButtonがi（一つの四角）ごとに区別するために使用
        ImGui::InvisibleButton("resizeDiagonal", ImVec2(HANDLE_SIZE * 2, HANDLE_SIZE * 2));

        /* 左クリック中 */
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            /* マウスポジションの差分（全体の拡大縮小なのでyの値だけで十分） */
            float delta = ImGui::GetIO().MouseDelta.y;

            /* 上側はマイナス */
            float sign = (i == 0 || i == 1) ? -1.0f : 1.0f;

            /* 拡大縮小 */
            m_branchButtonScale.x += delta * sign;
            m_branchButtonScale.y += delta * sign;

            m_branchButtonScale.x = std::max(m_branchButtonScale.x, 1.0f);
            m_branchButtonScale.y = std::max(m_branchButtonScale.y, 1.0f);
        }
        ImGui::PopID();
    }

    /* 上下左右ハンドルの処理 */
    for (int i = 0; i < 4; i++)
    {
        /* 描画 */
        draw->AddRectFilled(
            ImVec2(handlesHorizon[i].x - HANDLE_SIZE, handlesHorizon[i].y - HANDLE_SIZE),
            ImVec2(handlesHorizon[i].x + HANDLE_SIZE, handlesHorizon[i].y + HANDLE_SIZE),
            IM_COL32(255, 255, 255, 255)
        );

        /* 入力（下のInvisibleButtonの範囲に適用される） */
        ImGui::SetCursorScreenPos(
            ImVec2(handlesHorizon[i].x - HANDLE_SIZE, handlesHorizon[i].y - HANDLE_SIZE)
        );

        ImGui::PushID(i);   // InvisibleButtonがi（一つの四角）ごとに区別するために使用
        ImGui::InvisibleButton("resizeHorizon", ImVec2(HANDLE_SIZE * 2, HANDLE_SIZE * 2));

        /* 左クリック中 */
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            /* マウスポジションの差分 */
            ImVec2 delta = ImGui::GetIO().MouseDelta;

            /* i（一つの四角）ごとに拡大縮小処理をする */
            switch (i)
            {
            case 0:
                m_branchButtonScale.y += -delta.y;
                break;
            case 1:
                m_branchButtonScale.x += delta.x;
                break;
            case 2:
                m_branchButtonScale.y += delta.y;
                break;
            case 3:
                m_branchButtonScale.x += -delta.x;
                break;
            }

            m_branchButtonScale.x = std::max(m_branchButtonScale.x, 1.0f);
            m_branchButtonScale.y = std::max(m_branchButtonScale.y, 1.0f);
        }
        ImGui::PopID();
    }


    /* ここからオブジェクト移動用 */
    ImGui::SetCursorScreenPos(objMin);  // 下のInvisibleButtonに適用される
    float w = objMax.x - objMin.x;
    float h = objMax.y - objMin.y;

    /* 下限に1をとる（InvisibleButtonが壊れるのを防ぐため） */
    w = std::max(w, 1.0f);
    h = std::max(h, 1.0f);

    ImGui::InvisibleButton("move", ImVec2(w, h));

    /* 左クリック中 */
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        /* マウスに追従して移動 */
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        m_branchButtonPosition.x += delta.x;
        m_branchButtonPosition.y += delta.y;
    }

    ImGui::End();
}



void ImGuiManager::AddImageTransform(ImDrawList* draw, ImTextureID tex, ImVec2 pos, float rotation, ImVec2 scale)
{
    /* 中心からの広がりを算出 */
    ImVec2 half = { scale.x, scale.y };

    /* 回転のサイン・コサイン */
    float cos = cosf(XMConvertToRadians(rotation));
    float sin = sinf(XMConvertToRadians(rotation));

    /* ローカル座標 */
    ImVec2 local[4] =
    {
        {-half.x, -half.y},
        {half.x, -half.y},
        {half.x, half.y},
        {-half.x, half.y}
    };

    /* ワールド座標 */
    ImVec2 world[4];

    for (int i = 0; i < 4; ++i)
    {
        world[i] = ImVec2(
            pos.x + local[i].x * cos - local[i].y * sin,
            pos.y + local[i].x * sin + local[i].y * cos);
    }

    /* 実際の描画（テクスチャが無ければ赤い四角を描画） */
    if (tex)
    {
        draw->AddImageQuad(
            tex,
            world[0], world[1], world[2], world[3],
            ImVec2(0, 0), ImVec2(1, 0),
            ImVec2(1, 1), ImVec2(0, 1)
        );
    }
    else
    {
        draw->AddQuadFilled(world[0], world[1], world[2], world[3], IM_COL32(255.0f, 0.0f, 0.0f, 255.0f));
    }
}



std::wstring ImGuiManager::Utf8ToUtf16(const std::string& utf8)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(utf8);
}



std::string ImGuiManager::Utf16ToUtf8(const std::wstring& utf16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(utf16);
}



bool ImGuiManager::InputNameW(const char* label, std::wstring& wstr)
{
    /* 表示用文字列 = 確定文字 + 未確定文字 */
    std::wstring display = m_imeName.inputBuffer + m_imeName.compositionBuffer;
    std::string utf8 = Utf16ToUtf8(display);

    /* バッファ固定長 */
    static char buffer[512] = {};
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, utf8.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    /* 枠線だけ表示（編集不可） */
    ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::IsItemActive())
    {
        m_imeActive = &m_imeName;
    }

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);
    ImGui::Text(u8"全角は必ず最後はENTERを押下してください");

    /* 確定文字を wstr に反映 */
    wstr = m_imeName.inputBuffer;

    return false;
}

bool ImGuiManager::InputTextW(const char* label, std::wstring& wstr)
{
    /* 表示用文字列 = 確定文字 + 未確定文字 */
    std::wstring display = m_imeText.inputBuffer + m_imeText.compositionBuffer;
    std::string utf8 = Utf16ToUtf8(display);

    /* バッファ固定長 */
    static char buffer[512] = {};
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, utf8.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    /* 枠線だけ表示（編集不可） */
    ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::IsItemActive())
    {
        m_imeActive = &m_imeText;
    }

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);
    ImGui::Text(u8"全角は必ず最後はENTERを押下してください");

    /* 確定文字を wstr に反映 */
    wstr = m_imeText.inputBuffer;

    return false;
}

bool ImGuiManager::InputCharaNameW(const char* label, std::wstring& wstr)
{
    /* 表示用文字列 = 確定文字 + 未確定文字 */
    std::wstring display = m_imeCharaName.inputBuffer + m_imeCharaName.compositionBuffer;
    std::string utf8 = Utf16ToUtf8(display);

    /* バッファ固定長 */
    static char buffer[512] = {};
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, utf8.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    /* 枠線だけ表示（編集不可） */
    ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::IsItemActive())
    {
        m_imeActive = &m_imeCharaName;
    }

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);
    ImGui::Text(u8"全角は必ず最後はENTERを押下してください");

    /* 確定文字を wstr に反映 */
    wstr = m_imeCharaName.inputBuffer;

    return false;
}

bool ImGuiManager::InputRavelNameW(const char* label, std::wstring& wstr)
{
    /* 表示用文字列 = 確定文字 + 未確定文字 */
    std::wstring display = m_imeRavelName.inputBuffer + m_imeRavelName.compositionBuffer;
    std::string utf8 = Utf16ToUtf8(display);

    /* バッファ固定長 */
    static char buffer[512] = {};
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, utf8.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    /* 枠線だけ表示（編集不可） */
    ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::IsItemActive())
    {
        m_imeActive = &m_imeRavelName;
    }

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);
    ImGui::Text(u8"全角は必ず最後はENTERを押下してください");

    /* 確定文字を wstr に反映 */
    wstr = m_imeRavelName.inputBuffer;

    return false;
}

bool ImGuiManager::InputBranchButtonTextW(const char* label, std::wstring& wstr)
{
    /* 表示用文字列 = 確定文字 + 未確定文字 */
    std::wstring display = m_imeBranchButtonText.inputBuffer + m_imeBranchButtonText.compositionBuffer;
    std::string utf8 = Utf16ToUtf8(display);

    /* バッファ固定長 */
    static char buffer[512] = {};
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, utf8.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    /* 枠線だけ表示（編集不可） */
    ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::IsItemActive())
    {
        m_imeActive = &m_imeBranchButtonText;
    }

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);
    ImGui::Text(u8"全角は必ず最後はENTERを押下してください");

    /* 確定文字を wstr に反映 */
    wstr = m_imeBranchButtonText.inputBuffer;

    return false;
}



fs::path ImGuiManager::GetProjectRoot()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    /* exeがあるフォルダを取得 */
    fs::path exeDir = fs::path(exePath).parent_path();

    /* プロジェクトルート（例：x64/Debug → 1階層上） */
    m_projectRoot = exeDir;

    // exeが Debug / Release / x64 / bin 等のフォルダにある場合は1階層上に戻る
    std::wstring folderName = exeDir.filename().wstring();
    if (_wcsicmp(folderName.c_str(), L"Debug") == 0 ||
        _wcsicmp(folderName.c_str(), L"Release") == 0 ||
        _wcsicmp(folderName.c_str(), L"x64") == 0 ||
        _wcsicmp(folderName.c_str(), L"bin") == 0)
    {
        m_projectRoot = exeDir.parent_path();
    }

    return m_projectRoot;
}



std::wstring ImGuiManager::OpenFileDialogTexture()
{
    OPENFILENAMEW ofn = {};
    wchar_t filePath[MAX_PATH] = {};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetWindow();
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"画像ファイル (*.png;*.jpg;*bmp)\0*.png;*.jpg;*.bmp\0全てのファイル (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn))
    {
        return filePath;    //選択されたパスを返す
    }

    return L""; // キャンセル時は空文字
}



std::wstring ImGuiManager::OpenFileDialogSound()
{
    OPENFILENAMEW ofn = {};
    wchar_t filePath[MAX_PATH] = {};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetWindow();
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"画像ファイル (*.wav;*.mp3)\0*.wav;*mp3\0全てのファイル (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn))
    {
        return filePath;    //選択されたパスを返す
    }

    return L""; // キャンセル時は空文字
}



bool ImGuiManager::CopyToBgTextureFolder(const std::wstring& sourcePath)
{
    try 
    {
        /* コピー先フォルダ */
        fs::path destFolder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgTexture";

        if (!fs::exists(sourcePath))
        {
            MessageBoxW(nullptr, L"コピー元フォルダが存在しません。", L"Error", MB_OK);
            return false;
        }

        /* フォルダがなければ作成 */
        fs::create_directories(destFolder);

        /* ファイル名抽出 */
        fs::path fileName = fs::path(sourcePath).filename();
        fs::path destPath = destFolder / fileName;

        /* コピー作成 */
        fs::copy_file(sourcePath, destPath ,fs::copy_options::overwrite_existing);

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());   // e.what()は環境でUTF-8のことがあるため変換して表示する
        MessageBoxW(nullptr, msg.c_str(), L"コピー失敗", MB_OK);
        return false;
    }
}

bool ImGuiManager::CopyToCharaTextureFolder(const std::wstring& sourcePath)
{
    try
    {
        /* コピー先フォルダ */
        fs::path destFolder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildCharaTexture";

        if (!fs::exists(sourcePath))
        {
            MessageBoxW(nullptr, L"コピー元フォルダが存在しません。", L"Error", MB_OK);
            return false;
        }

        /* フォルダがなければ作成 */
        fs::create_directories(destFolder);

        /* ファイル名抽出 */
        fs::path fileName = fs::path(sourcePath).filename();
        fs::path destPath = destFolder / fileName;

        /* コピー作成 */
        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());   // e.what()は環境でUTF-8のことがあるため変換して表示する
        MessageBoxW(nullptr, msg.c_str(), L"コピー失敗", MB_OK);
        return false;
    }
}

bool ImGuiManager::CopyToBgmFolder(const std::wstring& sourcePath)
{
    try
    {
        /* コピー先フォルダ */
        fs::path destFolder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgm";

        if (!fs::exists(sourcePath))
        {
            MessageBoxW(nullptr, L"コピー元フォルダが存在しません。", L"Error", MB_OK);
            return false;
        }

        /* フォルダがなければ作成 */
        fs::create_directories(destFolder);

        /* ファイル名抽出 */
        fs::path fileName = fs::path(sourcePath).filename();
        fs::path destPath = destFolder / fileName;

        /* コピー作成 */
        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());   // e.what()は環境でUTF-8のことがあるため変換して表示する
        MessageBoxW(nullptr, msg.c_str(), L"コピー失敗", MB_OK);
        return false;
    }
}

bool ImGuiManager::CopyToSeFolder(const std::wstring& sourcePath)
{
    try
    {
        /* コピー先フォルダ */
        fs::path destFolder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildSe";

        if (!fs::exists(sourcePath))
        {
            MessageBoxW(nullptr, L"コピー元フォルダが存在しません。", L"Error", MB_OK);
            return false;
        }

        /* フォルダがなければ作成 */
        fs::create_directories(destFolder);

        /* ファイル名抽出 */
        fs::path fileName = fs::path(sourcePath).filename();
        fs::path destPath = destFolder / fileName;

        /* コピー作成 */
        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());   // e.what()は環境でUTF-8のことがあるため変換して表示する
        MessageBoxW(nullptr, msg.c_str(), L"コピー失敗", MB_OK);
        return false;
    }
}

bool ImGuiManager::CopyToBranchButtonFolder(const std::wstring& sourcePath)
{
    try
    {
        /* コピー先フォルダ */
        fs::path destFolder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBranchButton";

        if (!fs::exists(sourcePath))
        {
            MessageBoxW(nullptr, L"コピー元フォルダが存在しません。", L"Error", MB_OK);
            return false;
        }

        /* フォルダがなければ作成 */
        fs::create_directories(destFolder);

        /* ファイル名抽出 */
        fs::path fileName = fs::path(sourcePath).filename();
        fs::path destPath = destFolder / fileName;

        /* コピー作成 */
        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());   // e.what()は環境でUTF-8のことがあるため変換して表示する
        MessageBoxW(nullptr, msg.c_str(), L"コピー失敗", MB_OK);
        return false;
    }
}



void ImGuiManager::OpenBuildBgTextureFolder()
{
    try
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        /* exe があるディレクトリ */
        fs::path exeDir = fs::path(exePath).parent_path();

        std::wstring folderName = exeDir.filename().wstring();

        /* BuildBgTexture のパス */
        fs::path targetDir = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgTexture";

        /* フォルダが無い場合は作成 */
        if (!fs::exists(targetDir))
        {
            fs::create_directories(targetDir);
        }

        /* エクスプローラーで開く */
        ShellExecuteW(nullptr, L"open", targetDir.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());
        MessageBoxW(nullptr, msg.c_str(), L"フォルダを開けませんでした", MB_OK);
    }
}

void ImGuiManager::OpenBuildCharaTextureFolder()
{
    try
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        /* exe があるディレクトリ */
        fs::path exeDir = fs::path(exePath).parent_path();

        std::wstring folderName = exeDir.filename().wstring();

        /* BuildCharaTexture のパス */
        fs::path targetDir = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildCharaTexture";

        /* フォルダが無い場合は作成 */
        if (!fs::exists(targetDir))
        {
            fs::create_directories(targetDir);
        }

        /* エクスプローラーで開く */
        ShellExecuteW(nullptr, L"open", targetDir.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());
        MessageBoxW(nullptr, msg.c_str(), L"フォルダを開けませんでした", MB_OK);
    }
}

void ImGuiManager::OpenBuildBgmFolder()
{
    try
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        /* exe があるディレクトリ */
        fs::path exeDir = fs::path(exePath).parent_path();

        std::wstring folderName = exeDir.filename().wstring();

        /* BuildBgm のパス */
        fs::path targetDir = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgm";

        /* フォルダが無い場合は作成 */
        if (!fs::exists(targetDir))
        {
            fs::create_directories(targetDir);
        }

        /* エクスプローラーで開く */
        ShellExecuteW(nullptr, L"open", targetDir.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());
        MessageBoxW(nullptr, msg.c_str(), L"フォルダを開けませんでした", MB_OK);
    }
}

void ImGuiManager::OpenBuildSeFolder()
{
    try
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        /* exe があるディレクトリ */
        fs::path exeDir = fs::path(exePath).parent_path();

        std::wstring folderName = exeDir.filename().wstring();

        /* BuildSe のパス */
        fs::path targetDir = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildSe";

        /* フォルダが無い場合は作成 */
        if (!fs::exists(targetDir))
        {
            fs::create_directories(targetDir);
        }

        /* エクスプローラーで開く */
        ShellExecuteW(nullptr, L"open", targetDir.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());
        MessageBoxW(nullptr, msg.c_str(), L"フォルダを開けませんでした", MB_OK);
    }
}

void ImGuiManager::OpenBuildBranchButtonFolder()
{
    try
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        /* exe があるディレクトリ */
        fs::path exeDir = fs::path(exePath).parent_path();

        std::wstring folderName = exeDir.filename().wstring();

        /* BuildTexture のパス */
        fs::path targetDir = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBranchButton";

        /* フォルダが無い場合は作成 */
        if (!fs::exists(targetDir))
        {
            fs::create_directories(targetDir);
        }

        /* エクスプローラーで開く */
        ShellExecuteW(nullptr, L"open", targetDir.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const fs::filesystem_error& e)
    {
        std::wstring msg = Utf8ToUtf16(e.what());
        MessageBoxW(nullptr, msg.c_str(), L"フォルダを開けませんでした", MB_OK);
    }
}



std::vector<fs::path> ImGuiManager::GetImages(const std::wstring& folder)
{
    std::vector<fs::path> files;

    for (const auto& entry : fs::directory_iterator(folder))
    {
        /* フォルダははじく */
        if (!fs::is_regular_file(entry.status())) { continue; }

        auto ext = entry.path().extension().wstring();

        if (_wcsicmp(ext.c_str(), L".png") == 0 ||
            _wcsicmp(ext.c_str(), L".jpg") == 0 ||
            _wcsicmp(ext.c_str(), L".bmp") == 0)
        {
            files.push_back(entry.path());
        }
    }
    return files;
}



std::vector<fs::path> ImGuiManager::GetSounds(const std::wstring& folder)
{
    std::vector<fs::path> files;

    for (const auto& entry : fs::directory_iterator(folder))
    {
        /* フォルダははじく */
        if (!fs::is_regular_file(entry.status())) { continue; }

        auto ext = entry.path().extension().wstring();

        if (_wcsicmp(ext.c_str(), L".wav") == 0 ||
            _wcsicmp(ext.c_str(), L".mp3") == 0)
        {
            files.push_back(entry.path());
        }
    }
    return files;
}

std::vector<fs::path> ImGuiManager::GetProjectFolders(const std::wstring& folder)
{
    std::vector<fs::path> folders;

    for (const auto& entry : fs::directory_iterator(folder))
    {
        /* フォルダ以外は弾く */
        if (!fs::is_directory(entry.status())) { continue; }

        folders.push_back(entry.path());
    }
    return folders;
}



void ImGuiManager::ShowBgTextureSelector(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgTexture";
    auto files = ImGuiManager::GetImages(folder);

    for (auto& path : files)
    {
        auto srv = Texture::EditorGet(path.wstring());
        if (!srv)
        {
            Texture::EditorLoad(path.wstring());
            srv = Texture::EditorGet(path.wstring());
        }

        /* テクスチャサイズの取得 */
        ID3D11Resource* resource = nullptr;
        srv->GetResource(&resource);

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
        resource->Release();

        UINT width = 64;
        UINT height = 64;

        if (SUCCEEDED(hr))
        {
            D3D11_TEXTURE2D_DESC desc = {};
            texture->GetDesc(&desc);
            texture->Release();

            width = desc.Width;
            height = desc.Height;
        }

        /* サムネイルサイズ計算 */
        float displayW = 250.0f;
        float aspect = (float)height / (float)width;
        float displayH = displayW * aspect;

        /* サムネイル表示 */
        ImGui::Image((ImTextureID)srv, ImVec2(displayW, displayH));

        ImGui::SameLine();

        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str()))
        {
            selected = path.wstring();
        }
    }
}



void ImGuiManager::ShowCharaNameSelector(std::wstring& selected)
{
    for (std::wstring& name : m_charaNames)
    {
        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(name).c_str()))
        {
            selected = name;
        }
    }
}



void ImGuiManager::ShowCharaTextureSelector(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildCharaTexture";
    auto files = ImGuiManager::GetImages(folder);

    for (auto& path : files)
    {
        auto srv = Texture::EditorGet(path.wstring());
        if (!srv)
        {
            Texture::EditorLoad(path.wstring());
            srv = Texture::EditorGet(path.wstring());
        }

        /* テクスチャサイズの取得 */
        ID3D11Resource* resource = nullptr;
        srv->GetResource(&resource);

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
        resource->Release();

        UINT width = 64;
        UINT height = 64;

        if (SUCCEEDED(hr))
        {
            D3D11_TEXTURE2D_DESC desc = {};
            texture->GetDesc(&desc);
            texture->Release();

            width = desc.Width;
            height = desc.Height;
        }

        /* サムネイルサイズ計算 */
        float displayW = 250.0f;
        float aspect = (float)height / (float)width;
        float displayH = displayW * aspect;

        /* サムネイル表示 */
        ImGui::Image((ImTextureID)srv, ImVec2(displayW, displayH));

        ImGui::SameLine();

        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str()))
        {
            selected = path.wstring();
        }
    }
}



void ImGuiManager::ShowBgmSelector(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBgm";
    auto files = ImGuiManager::GetSounds(folder);

    for (auto& path : files)
    {
        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str()))
        {
            selected = path.wstring();
        }
    }
}

void ImGuiManager::ShowSeSelector(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildSe";
    auto files = ImGuiManager::GetSounds(folder);

    for (auto& path : files)
    {
        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str()))
        {
            selected = path.wstring();
        }
    }
}

void ImGuiManager::ShowRavelTextureSelector(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject" / m_selectProjectName / L"nb_asset" / L"BuildBranchButton";
    auto files = ImGuiManager::GetImages(folder);

    for (auto& path : files)
    {
        auto srv = Texture::EditorGet(path.wstring());
        if (!srv)
        {
            Texture::EditorLoad(path.wstring());
            srv = Texture::EditorGet(path.wstring());
        }

        /* テクスチャサイズの取得 */
        ID3D11Resource* resource = nullptr;
        srv->GetResource(&resource);

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
        resource->Release();

        UINT width = 64;
        UINT height = 64;

        if (SUCCEEDED(hr))
        {
            D3D11_TEXTURE2D_DESC desc = {};
            texture->GetDesc(&desc);
            texture->Release();

            width = desc.Width;
            height = desc.Height;
        }

        /* サムネイルサイズ計算 */
        float displayW = 250.0f;
        float aspect = (float)height / (float)width;
        float displayH = displayW * aspect;

        /* サムネイル表示 */
        ImGui::Image((ImTextureID)srv, ImVec2(displayW, displayH));

        ImGui::SameLine();

        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str()))
        {
            selected = path.wstring();
        }
    }
}

void ImGuiManager::ShowRavelSelector(std::wstring& selected)
{
    /* 現在のリストの中身を取得 */
    CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();

    if (createList == nullptr) { return; }

    for (const ObjectEntry& object : createList->GetListObjects())
    {
        /* リスト内に入っているラベルを探索 */
        if (object.objectType == ObjectType::Ravel)
        {
            CreateRavel* ravel = dynamic_cast<CreateRavel*>(object.listObject);

            if (ravel == nullptr) { continue; }

            if (ImGui::Button(Utf16ToUtf8(ravel->GetRavelName()).c_str()))
            {
                selected = ravel->GetRavelName();
            }
        }
    }
}

void ImGuiManager::ShowProjectSelecter(std::wstring& selected)
{
    fs::path folder = ImGuiManager::GetProjectRoot() / L"myProject";

    /* フォルダが無い場合は作成 */
    if (!fs::exists(folder))
    {
        fs::create_directories(folder);
    }

    auto files = ImGuiManager::GetProjectFolders(folder);

    for (auto& path : files)
    {
        /* クリックで選択 */
        if (ImGui::Button(Utf16ToUtf8(path.filename().wstring()).c_str(), ImVec2(-1.0f, 50.0f)))
        {
            selected = path.wstring();
        }
    }
}

void ImGuiManager::CreateNewProject()
{
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.15f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.7f), ImGuiCond_Always);

    ImGui::Begin(u8"プロジェクトの新規作成", &m_createNewProjectOpen, ImGuiWindowFlags_NoCollapse);
    ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

    static char buffer[512] = {};

    ImGui::InputText(u8"新規プロジェクト名：半角入力", buffer, sizeof(buffer));

    ImGui::TextWrapped(u8"プレビュー：%s", buffer);

    ImGui::Dummy(ImVec2(0.0f, 30.0f));

    if (ImGui::Button(u8"戻る"))
    {
        m_createNewProjectOpen = false;
    }

    ImGui::SameLine(0.0f, 790.0f);

    if (ImGui::Button(u8"新規作成"))
    {
        fs::path folderPath = ImGuiManager::GetProjectRoot() / L"myProject" / buffer;

        try
        {
            fs::create_directories(folderPath);

            int result = MessageBoxW(nullptr, L"プロジェクトが新規作成されました！", L"成功", MB_OK);

            if (result == IDOK)
            {
                m_createNewProjectOpen = false;
            }
        }
        catch (const fs::filesystem_error& e)
        {
            MessageBox(nullptr, e.what(), "ファイルシステムエラー", MB_OK);
        }
    }

    ImGui::End();
}

