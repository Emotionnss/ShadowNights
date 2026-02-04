#pragma once
#include "../Player/Transform2D.h"

class BaseProjectile 
{
private:
	float timeLeft = 2.f;
	AEVec2 direction;
	float currLifespan = timeLeft;

public:
	Transform2D transform;
	AEGfxTexture* projTex;

	float speed = 1.f, size = 1.f;

	bool stopped{};
	BaseProjectile(AEVec2 pos, AEVec2 dir);
	~BaseProjectile();

	void Update(float deltaTime);
	void Draw(AEGfxVertexList* mesh);
	void Exit();

	float GetLifespan();
	void SetLifespan(float time);
};

