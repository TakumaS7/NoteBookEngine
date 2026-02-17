/*===============================================================*/
//
// outputParameter.h (アウトプットコンポーネントのパラメータ管理)
//
// ---------------------------------- 2026/02/09 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _OUTPUT_PARAMETER_H
#define _OUTPUT_PARAMETER_H

#include <string>
#include "transform.h"
#include "charaTexture.h"
#include "createSpeakText.h"
#include "branchButton.h"

struct OutputParameter
{
	unsigned int listIndex = 0;						// クリエイトリスト内のインデックス
	std::wstring path{};							// ファイルのパス情報
	std::wstring text{};							// テキスト情報
	Transform transform{};							// トランスフォーム情報
	XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 0.0f };	// カラーコード情報

	std::wstring charaName{};		// キャラの名前
	bool charaAppearance = false;	// キャラが登場したかどうか
	CharaTexture* charaTexture{};	// キャラのテクスチャ情報

	CreateSpeakText* speakText{};	// セリフテキスト情報

	std::wstring targetRavelName{};	// 目的にするラベル名
	BranchButton* branchButton{};	// 分岐ボタン情報

};

#endif //_OUTPUT_PARAMETER_H