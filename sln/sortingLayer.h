/*===============================================================*/
//
//	sortingLayer.h(2D Zソート管理)
// 
// 
// ---------------------------------- 2025/10/09 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SORTING_LAYER_H
#define _SORTING_LAYER_H

#include "component.h"
#include "scene.h"

class SortingLayer : public Component
{
private:
	int m_sortingLayer = 0;	// 昇順で描画（１→２→３の順）

public:
	void Set(int sortingLayer) { m_sortingLayer = sortingLayer; }
	int GetSortingLayer() const { return m_sortingLayer; }

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_SORTING_LAYER_H