/*===============================================================*/
//
//	component.h(コンポーネント管理)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/
#ifndef _COMPONENT_H
#define _COMPONENT_H

class Component
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif //_COMPONENT_H