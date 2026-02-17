/*===============================================================*/
//
//	transform.h(トランスフォーム管理)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "component.h"
#include "vector3.h"
#include "renderer.h"

class Transform : public Component
{
public:
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	void Set(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}

	void Set(const Transform& transform)
	{
		position = transform.position;
		rotation = transform.rotation;
		scale = transform.scale;
	}

	Vector3 GetRight()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);

		return right;
	}

	Vector3 GetForward()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);

		return forward;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};

#endif //_TRANSFORM_H