/*===============================================================*/
//
// gameStopOutputComponent.h(ゲーム停止コンポーネント)
//
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _GAME_STOP_OUTPUT_COMPONENT
#define _GAME_STOP_OUTPUT_COMPONENT

#include "component.h"

class GameStopOutputComponent : public Component
{
private:
	bool m_gameStop = false;	// ゲーム停止状態

public:
	bool GetGameStop() const { return m_gameStop; }

	/* 停止を解除したい際に使用する */
	void SetGameStopOff() { m_gameStop = false; }

	void LeftClickGameStop() 
	{
		/* 停止フラグをONにする（ゲームが止まる） */
		m_gameStop = true;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_GAME_STOP_OUTPUT_COMPONENT