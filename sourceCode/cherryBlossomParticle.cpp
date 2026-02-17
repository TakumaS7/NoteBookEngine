/*===============================================================*/
//
//	cherryBlossomParticle.cpp(桜パーティクル管理)
// 
// 
// ---------------------------------- 2025/12/07 製作者：佐藤琢磨
//
/*===============================================================*/

#include "cherryBlossomParticle.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader3D.h"
#include "transformDrawParticle.h"
#include "textureComponent.h"
#include "particleVertex.h"
#include "cherryBlossomParticleComponent.h"

void CherryBlossomParticle::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 3.0f, -6.0f }, { 0.0f, 0.0f, 0.0f }, { 0.05f, 0.05f, 0.05f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\cherryBlossom.png", true);
	AddComponent<ParticleVertex>()->Set(this);
	AddComponent<TransformDrawParticle>()->Set(this);
	AddComponent<CherryBlossomParticleComponent>()->Set(this, 0.0f);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
