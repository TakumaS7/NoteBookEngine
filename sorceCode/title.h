/*===============================================================*/
//
//	title.h(タイトル処理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TITLE_H
#define _TITLE_H

#include "scene.h"

class Title : public Scene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

#endif //_TITLE_H