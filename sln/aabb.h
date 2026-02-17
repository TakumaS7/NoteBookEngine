/*===============================================================*/
//
//	aabb.h(当たり判定処理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _AABB_H
#define _AABB_H

#include "component.h"
#include "gameObject.h"
#include "vector3.h"
#include "transform.h"
#include "manager.h"
#include <list>
#include "rigidbody.h"

class AABB : public Component
{
private:
    GameObject* m_gameObject = nullptr;     // 自分自身
    Transform* m_transform = nullptr;       // Transform
    Rigidbody* m_rigidbody = nullptr;       // Rigidbody

    Vector3 m_min, m_max;       // AABBの範囲（ワールド座標）
    Vector3 m_oldPosition;      // 前フレームの位置
    Vector3 m_aabbSize;         // AABBの大きさ
    bool m_onGround = false;    // 地面着地フラグ
    bool m_collision = false;   // 当たったかのフラグ
    bool m_useGravity = false;  // もともと重力を適用しているかの確認フラグ

public:
    /* このオブジェクトにAABBを持たせる */
    void Set(GameObject* gameObject, Vector3 aabbSize)
    {
        m_gameObject = gameObject;
        m_aabbSize = aabbSize;
    }

    void Init() override
    {
        /* GameObjectからTransform、Rigidbodyを取得する */
        if (!m_gameObject) { return; }

        m_transform = m_gameObject->GetComponent<Transform>();
        m_rigidbody = m_gameObject->GetComponent<Rigidbody>();

        if (!m_transform || !m_rigidbody) { return; }

        /* このオブジェクトの重力適用フラグ状態を保持 */
        m_useGravity = m_rigidbody->GetUseGravity();

        /* 初期AABB計算 */
        m_min = m_transform->position - m_aabbSize;
        m_max = m_transform->position + m_aabbSize;

        m_oldPosition = m_transform->position;
    }

    void Uninit() override {}

    void Update() override
    {
        if (!m_transform || !m_rigidbody) { return; }

        /* 毎フレームAABBを更新 */
        Vector3 diff = m_transform->position - m_oldPosition;   // 1フレームでの移動差分
        m_min += diff;
        m_max += diff;
        m_oldPosition = m_transform->position;  // oldPositionに現在の座標を代入

        /* シーン内オブジェクトとの衝突判定 */
        auto objects = Manager::GetScene()->GetAllGameObject(); // シーン内の全オブジェクト情報の取得

        /* もし重力適用フラグが元々trueだったのであれば、毎フレームtrueにする */
        if (m_useGravity) { m_rigidbody->SetUseGravity(true); }

        /* 地面に着地フラグと、当たっているかのフラグを、毎フレームfalseにする */
        m_onGround = false;
        m_collision = false;

        for (auto other : objects)
        {
            if (other == m_gameObject) { continue; } // 自分は無視

            AABB* box2 = other->GetComponent<AABB>();
            if (!box2) { continue; }    //相手がAABBを持っていなければ無視

            /* 相手のAABBを取得 */
            Vector3 minB = box2->m_min;
            Vector3 maxB = box2->m_max;

            /* AABB交差チェック */
            if (!(m_min.x > maxB.x || m_max.x < minB.x ||
                m_min.y > maxB.y || m_max.y < minB.y ||
                m_min.z > maxB.z || m_max.z < minB.z))
            {
                /* penetration計算 */
                float px = std::min(maxB.x - m_min.x, m_max.x - minB.x);
                float py = std::min(maxB.y - m_min.y, m_max.y - minB.y);
                float pz = std::min(maxB.z - m_min.z, m_max.z - minB.z);

                float minPen = std::min({ px, py, pz });
                Vector3 dir = m_transform->position - other->GetComponent<Transform>()->position;

                /* 押し出し処理 */
                if (minPen == px) // X軸方向が最小
                {
                    float correction = (dir.x > 0) ? px : -px;  // X方向の距離が正ならpx、０か負なら-pxを代入
                    m_transform->position.x += correction;
                    Vector3 v = m_rigidbody->GetVelocity();

                    /* 衝突方向が相手の右方向か左方向かを判定 */
                    if (dir.x > 0 || dir.x < 0) {
                        v.x = 0.0f;                   // X速度のみクリア
                        m_rigidbody->SetVelocity(v);  // 明示的に速度を設定
                    }
                }
                else if (minPen == py) // Y軸方向が最小
                {
                    float correction = (dir.y > 0) ? py : -py;  // Y方向の距離が正ならpy、０か負なら-pyを代入
                    m_transform->position.y += correction;

                    /* 上から乗ったときは着地フラグをtrueにして、重力をOFFにする */
                    if (dir.y > 0 && m_rigidbody->GetVelocity().y <= 0.0f)
                    {
                        m_onGround = true;

                        if (m_useGravity) { m_rigidbody->SetUseGravity(false); }
                    }
                    else
                    {
                        if (m_useGravity) { m_rigidbody->SetUseGravity(true); }
                    }

                    Vector3 v = m_rigidbody->GetVelocity();
                    /* 衝突方向が相手の上方向（地面）か下方向（天井）かを判定 */
                    if (dir.y > 0 || dir.y < 0) {
                        v.y = 0.0f;                   // Y速度のみクリア
                        m_rigidbody->SetVelocity(v);  // 明示的に速度を設定
                    }
                }
                else // Z軸方向が最小
                {
                    float correction = (dir.z > 0) ? pz : -pz;  // Z方向の距離が正ならpz、０か負なら-pzを代入
                    m_transform->position.z += correction;
                    Vector3 v = m_rigidbody->GetVelocity();

                    // 衝突方向が奥方向（Zの正）か手前方向（Zの負）かを判定
                    if (dir.z > 0 || dir.z < 0) {
                        v.z = 0.0f;                   // Z速度のみクリア
                        m_rigidbody->SetVelocity(v);  // 明示的に速度を設定
                    }
                }

                m_collision = true;
            }
        }

        m_rigidbody->SetOnGround(m_onGround);
    }

    void Draw() override {}
};

#endif //_AABB_H