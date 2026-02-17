/*===============================================================*/
//
//	createScene.h(作業用シーン管理)
// 
// 
// ---------------------------------- 2025/09/27 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_SCENE_H
#define _CREATE_SCENE_H

#include "scene.h"

class CreateScene : public Scene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

#endif //_CREATE_SCENE_H