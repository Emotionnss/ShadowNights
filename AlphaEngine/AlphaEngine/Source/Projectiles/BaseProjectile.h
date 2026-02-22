#pragma once
#include "../Player/Transform2D.h"

class BaseProjectile 
{
private:
	float timeLeft = 2.f;
	AEVec2 direction;
	float currLifespan = timeLeft;

public:
	float speed = 1.f, size = 1.f;
	Transform2D transform;
	AEGfxTexture* projTex;


	bool stopped{};
	BaseProjectile(AEVec2 pos, AEVec2 dir, bool skipTextureLoad = false);
	~BaseProjectile();

	virtual void Update(float deltaTime);
	void Draw(AEGfxVertexList* mesh);
	void Exit();

	float GetLifespan();
	void SetLifespan(float time);

	AEVec2 GetDirection();
};

