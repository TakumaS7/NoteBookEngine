/*===============================================================*/
//
//	speakTextHiddenOutput.cpp(テキストフレーム非表示管理)
// 
// 
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#include "speakTextHiddenOutput.h"

#include "sortingLayer.h"
#include "speakTextHiddenOutputComponent.h"

void SpeakTextHiddenOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<SpeakTextHiddenOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
