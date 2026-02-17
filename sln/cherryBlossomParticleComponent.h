/*===============================================================*/
//
//	cherryBlossomParticleComponent.h(桜パーティクル挙動管理)
// 
// 
// ---------------------------------- 2025/12/07 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CHERRY_BLOSSOM_PARTICLE_COMPONENT_H
#define _CHERRY_BLOSSOM_PARTICLE_COMPONENT_H

#include "main.h"
#include "component.h"
#include "gameObject.h"
#include "time.h"
#include "particleVertex.h"
#include "vector3.h"
#include "transform.h"

class CherryBlossomParticleComponent : public Component
{
private:
    GameObject* m_gameObject;
	Transform* m_transform;
    ParticleVertex* m_particleVertex;
	float m_time = 0.0f;		// 秒数管理
	float m_timeLag = 0.0f;	// 出現ラグ
	static constexpr float m_airResistance = -0.0001f;	// 桜空気抵抗

public:
    void Set(GameObject* gameObject, float timeLag) {
        m_gameObject = gameObject; 
		m_timeLag = timeLag;
    }

    void Init() override {}
    void Uninit() override {}
    void Update() override
    {
		m_transform = m_gameObject->GetComponent<Transform>();
		m_particleVertex = m_gameObject->GetComponent<ParticleVertex>();

		m_time += Time::m_deltaTime;

		int count = 0;

		if (m_time >= m_timeLag) 
		{
			m_time = 0.0f;

			for (int i = 0; i < PARTICLE_MAX; ++i)
			{
				if (m_particleVertex->GetParticleStatus()[i].enable == false)
				{
					m_particleVertex->GetParticleStatus()[i].enable = true;

					m_particleVertex->GetParticleStatus()[i].life = 240;

					m_particleVertex->GetParticleStatus()[i].position = m_transform->position;
					m_particleVertex->GetParticleStatus()[i].position.x += (rand() % 20 - 10) + (0.1f * static_cast<float>(rand() % 9));

					m_particleVertex->GetParticleStatus()[i].rotation = m_transform->rotation;

					m_particleVertex->GetParticleStatus()[i].velocity.y = -(rand() % 15) * 0.002f;
					m_particleVertex->GetParticleStatus()[i].velocity.z = (15 + (rand() % 20)) * 0.002f;

					m_particleVertex->GetParticleStatus()[i].spinSpeed.x = (rand() % 5 - 2) * 0.1f;
					m_particleVertex->GetParticleStatus()[i].spinSpeed.y = (rand() % 5 - 2) * 0.1f;
					m_particleVertex->GetParticleStatus()[i].spinSpeed.z = (rand() % 5 - 2) * 0.1f;

					count++;

					if (count >= 5)
					{
						count = 0;
						break;
					}
				}
			}
		}

		for (int i = 0; i < PARTICLE_MAX; ++i)
		{
			if (m_particleVertex->GetParticleStatus()[i].enable == true)
			{
				m_particleVertex->GetParticleStatus()[i].velocity += Vector3(0.0f, 0.0f, m_airResistance);

				m_particleVertex->GetParticleStatus()[i].position += m_particleVertex->GetParticleStatus()[i].velocity;
				m_particleVertex->GetParticleStatus()[i].rotation += m_particleVertex->GetParticleStatus()[i].spinSpeed;

				m_particleVertex->GetParticleStatus()[i].life--;
				if (m_particleVertex->GetParticleStatus()[i].life == 0)
				{
					m_particleVertex->GetParticleStatus()[i].enable = false;
				}
			}
		}
    }
    void Draw() override {}
};

#endif //_CHERRY_BLOSSOM_PARTICLE_COMPONENT_H