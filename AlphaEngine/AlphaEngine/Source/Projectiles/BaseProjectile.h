#pragma once
#include "../Player/Transform2D.h"

class BaseProjectile
{
private:
	float timeLeft = 2.f;
	float currLifespan = timeLeft;
	AEVec2 direction;

public:
	Transform2D transform;
	AEGfxTexture* projTex;
	AEGfxVertexList* Texmesh;

	float speed = 1.f, size = 1.f;

	BaseProjectile();
	BaseProjectile(AEVec2 pos, AEVec2 dir);
	~BaseProjectile();

	void Init();
	void Update(float deltaTime);
	void Draw(AEGfxVertexList* mesh);
	void Exit();

	float GetLifespan();
};

