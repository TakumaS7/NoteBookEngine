/*==========================================================*/
//
//	vector3.h (ベクトル処理)
// 
// 
// ------------------------------ 2025/05/13 製作者：佐藤琢磨
//
/*==========================================================*/
#ifndef _VECTOR3_H
#define _VECTOR3_H

#include <math.h>

class Vector3 
{
public:
	float x{}, y{}, z{};

	/* コンストラクタ */
	//デフォルトコンストラクタ
	Vector3() {}
	//コピーコンストラクタ
	Vector3(const Vector3& a) : x(a.x), y(a.y), z(a.z) {}
	//3つの値で作成する
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}



	/* 標準的なオブジェクトの保守 */
	//代入（Cの慣習に従い値への参照を返す）
	Vector3& operator = (const Vector3& a) {
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	//等しさのチェック
	bool operator == (const Vector3& a) const {
		return x == a.x && y == a.y && z == a.z;
	}

	bool operator != (const Vector3& a) const {
		return x != a.x || y != a.y || z != a.z;
	}



	/* ベクトル操作 */
	//ベクトルを０にする
	void zero() { x = y = z = 0.0f; }

	//単項式のマイナスは、反転したベクトルを返す
	Vector3 operator -() const { return Vector3(-x, -y, -z); }

	//二項式の＋とーはベクトルを加算し、減算する
	Vector3 operator + (const Vector3& a) const {
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator - (const Vector3& a) const {
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	//スカラーによる乗算と除算
	Vector3 operator * (float a) const {
		return Vector3(x * a, y * a, z * a);
	}

	Vector3 operator / (float a) const {
		float oneOverA = 1.0f / a; //ここではゼロ除算のチェックはしていない
		return Vector3(x * oneOverA, y * oneOverA, z * oneOverA);
	}

	//Cの表記に準拠するための組み合わせ代入演算
	Vector3& operator += (const Vector3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vector3& operator -= (const Vector3& a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vector3& operator *= (float a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	Vector3& operator /= (float a) {
		float oneOverA = 1.0f / a;
		x *= oneOverA;
		y *= oneOverA;
		z *= oneOverA;
		return *this;
	}

	//ベクトルを正規化する
	void normalize() {
		float magSq = x * x + y * y + z * z;

		//０除算をチェックする
		if (magSq > 0.0f) {
			float oneOverMag = static_cast<float>(1.0f / sqrt(magSq));
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
};


#endif //_VECTOR3_H