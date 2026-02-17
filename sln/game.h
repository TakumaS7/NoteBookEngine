/*===============================================================*/
//
//	game.h(ƒQ[ƒ€ˆ—)
// 
// 
// ---------------------------------- 2025/07/19 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _GAME_H
#define _GAME_H

#include "scene.h"

class Game : public Scene
{
public:
	void Init() override;
	void Update() override;
};

#endif //_GAME_H