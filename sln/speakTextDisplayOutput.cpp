/*===============================================================*/
//
//	speakTextDisplayOutput.cpp(テキストフレーム表示管理)
// 
// 
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#include "speakTextDisplayOutput.h"

#include "sortingLayer.h"
#include "speakTextDislayOutputComponent.h"

void SpeakTextDisplayOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<SpeakTextDisplayOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
