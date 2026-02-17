/*===============================================================*/
//
//	imGuiManager.h(ImGui管理)
// 
// 
// ---------------------------------- 2025/09/29 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _IMGUI_MANAGER_H
#define _IMGUI_MANAGER_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "main.h"
#include <string>
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include <commdlg.h>
#include <shlwapi.h>
#include <experimental/filesystem>
#pragma comment(lib, "shlwapi.lib")

namespace fs = std::experimental::filesystem;

class ImGuiManager
{
private:
    struct ImeBuffer
    {
        std::wstring inputBuffer;       // 確定文字
        std::wstring compositionBuffer; // 未確定文字
    };

    static ImeBuffer m_imeName;
    static ImeBuffer m_imeText;
    static ImeBuffer m_imeCharaName;
    static ImeBuffer m_imeRavelName;
    static ImeBuffer m_imeBranchButtonText;
    static ImeBuffer* m_imeActive;

    static fs::path m_projectRoot;  // プロジェクトフォルダのパス

    static std::wstring m_selectedBgTexture;        // エディタ内の選んだ背景画像名
    static std::wstring m_selectedCharaName;        // エディタ内の選んだキャラの名前
    static std::wstring m_selectedCharaTexture;     // エディタ内の選んだキャラ画像名
    static std::wstring m_selectedBgm;              // エディタ内の選んだBGM
    static std::wstring m_selectedSe;               // エディタ内の選んだSE
    static std::wstring m_selectRavelButtonTexture; // エディタ内の選んだ分岐ボタンテクスチャー
    static std::wstring m_selectRavelName;          // エディタ内の選んだラベル名

    static std::wstring m_selectProjectPath;    // 選択されたプロジェクトフォルダパス
    static std::wstring m_selectProjectName;    // 選択されたプロジェクト名

    static XMFLOAT2 m_charaPosition;                // キャラの配置ポジション
    static float m_charaRotation;                   // キャラの回転(度表記、ラジアンではない)
    static XMFLOAT2 m_charaScale;                   // キャラの大きさ
    static std::vector<std::wstring> m_charaNames;  // キャラの名前

    static XMFLOAT2 m_branchButtonPosition; // 分岐ボタンの配置ポジション
    static float m_branchButtonRotation;    // 分岐ボタンの回転(度表記、ラジアンではない)
    static XMFLOAT2 m_branchButtonScale;    // 分岐ボタンの大きさ

    static Vector3 m_BranchButtonTextColor;  // 分岐ボタンテキストのカラーコード

    static bool m_charaEditorOpen;          // キャラエディター開くフラグ
    static bool m_branchButtonEditorOpen;   // 分岐ボタンエディター開くフラグ

    static bool m_projectSelectOpen;    // プロジェクト選択画面の開閉フラグ
    static bool m_createNewProjectOpen; // 新規プロジェクト作成画面の開閉フラグ

public:
    static void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
    static void BegineFrame();
    static void DrawCreate();
    static void DrawProjectSelect();
    static void EndFrame();

    /* プロジェクト選択画面を開く */
    static void OpenToProjectSelect() { m_projectSelectOpen = true; }
    static std::wstring GetSelectProjectName() { return m_selectProjectName; }
    static std::string GetSelectProjectNameString();

    static void TextEditor();       // テキストエディタ用関数
    static void BgEditor();         // 背景エディタ用関数
    static void CharaEditor();      // キャラエディタ用関数
    static void SoundEditor();      // サウンドエディタ用関数
    static void SystemEditor();     // システム用関数
    static void HierarchyEditor();  // ヒエラルキーエディタ用関数

    static void CharaDelete(std::wstring& selectCharaName); // キャラ削除用関数
    static void CharaAppearanceEditor();                    // キャラ登場エディタ用関数
    static void CharaChangeEditor();                        // キャラ状態変更エディタ用関数
    static void CharaExitEditor();                          // キャラ退場エディタ用関数
    static void CharaEditorOpem();                          // キャラの詳細設定画面の表示

    static void BgmEditor();                                // BGMエディタ用関数
    static void SeEditor();                                 // SEエディタ用関数

    static void BranchButtonEditor();       // 分岐ボタンエディタ用関数
    static void BranchButtonEditorOpen();   // 分岐ボタンの詳細設定画面の表示

    static void AddImageTransform(ImDrawList* draw, ImTextureID tex, ImVec2 pos, float rotation, ImVec2 scale);   // キャラ調整の回転処理にて使用

    static std::wstring Utf8ToUtf16(const std::string& utf8);
    static std::string Utf16ToUtf8(const std::wstring& utf16);

    static bool InputNameW(const char* label, std::wstring& wstr);
    static bool InputTextW(const char* label, std::wstring& wstr);
    static bool InputCharaNameW(const char* label, std::wstring& wstr);
    static bool InputRavelNameW(const char* label, std::wstring& wstr);
    static bool InputBranchButtonTextW(const char* label, std::wstring& wstr);



    /* exeファイルの場所からパスを取得する関数 */
    static fs::path GetProjectRoot();

    /* 画像・音声ファイル取り込み */
    static std::wstring OpenFileDialogTexture();
    static std::wstring OpenFileDialogSound();
    static bool CopyToBgTextureFolder(const std::wstring& sourcePath);
    static bool CopyToCharaTextureFolder(const std::wstring& sourcePath);
    static bool CopyToBgmFolder(const std::wstring& sourcePath);
    static bool CopyToSeFolder(const std::wstring& sourcePath);
    static bool CopyToBranchButtonFolder(const std::wstring& sourcePath);

    /* 取り込み画像・音声フォルダの参照 */
    static void OpenBuildBgTextureFolder();
    static void OpenBuildCharaTextureFolder();
    static void OpenBuildBgmFolder();
    static void OpenBuildSeFolder();
    static void OpenBuildBranchButtonFolder();

    /* 取り込み画像・音声一覧 */
    static std::vector<fs::path> GetImages(const std::wstring& folder);
    static std::vector<fs::path> GetSounds(const std::wstring& folder);
    static std::vector<fs::path> GetProjectFolders(const std::wstring& folder);

    static void ShowBgTextureSelector(std::wstring& selected);
    static void ShowCharaNameSelector(std::wstring& selected);
    static void ShowCharaTextureSelector(std::wstring& selected);
    static void ShowBgmSelector(std::wstring& selected);
    static void ShowSeSelector(std::wstring& selected);
    static void ShowRavelTextureSelector(std::wstring& selected);
    static void ShowRavelSelector(std::wstring& selected);

    /* プロジェクト一覧 */
    static void ShowProjectSelecter(std::wstring& selected);

    /* プロジェクトの新規作成 */
    static void CreateNewProject();

    /* 登録したキャラ名も保存する */
    static void SetCharaNames(std::vector<std::wstring> names) { m_charaNames = names; }
    static std::vector<std::wstring> GetCharaNames() { return m_charaNames; }

    /* WndProcから呼ぶ */
    static bool HasImeActive() { return m_imeActive != nullptr; }
    static void AddInputBuffer(const std::wstring& wstring) { 
        if (!m_imeActive) { return; }
        m_imeActive->inputBuffer += wstring;
    }
    static std::wstring& GetInputBuffer() { return m_imeActive->inputBuffer; }
    static void SetCompositionBuffer(const std::wstring& wstring) {
        if (!m_imeActive) { return; }
        m_imeActive->compositionBuffer = wstring; 
    }
    static const std::wstring& GetCompositionBuffer() { return m_imeActive->compositionBuffer; }

    /* Enter押下で確定文字を追加する */
    static void ConfirmComposition() { 
        if (!m_imeActive) { return; }
        m_imeActive->inputBuffer += m_imeActive->compositionBuffer;
        m_imeActive->compositionBuffer.clear();
    }

    /* backspaceキーを押したときの文字削除 */
    static void Backspace() {
        if (!m_imeActive) { return; }
        if (m_imeActive->inputBuffer.size() > 0) {
            m_imeActive->inputBuffer.pop_back();
        }
    }

    /* deleteキーを押したときの文字全削除 */
    static void Delete() {
        if (!m_imeActive) { return; }
        if (m_imeActive->inputBuffer.size() > 0) {
            m_imeActive->inputBuffer.erase();
        }
    }
};

#endif
