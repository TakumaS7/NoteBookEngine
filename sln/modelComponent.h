/*===============================================================*/
//
//	modelComponent.h(ÉÇÉfÉãì«çûèàóù)
// 
// 
// ---------------------------------- 2025/09/25 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _MODEL_COMPONENT_H
#define _MODEL_COMPONENT_H

class ModelComponent
{

protected:

	class  GameObject* m_GameObject = nullptr;

public:
	//Component() = delete;
	ModelComponent() {}
	ModelComponent(GameObject* Object) { m_GameObject = Object; }
	virtual ~ModelComponent() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};



};

#endif //_MODEL_COMPONENT_H