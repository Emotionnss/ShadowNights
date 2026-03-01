#pragma once
#include "../Player/Transform2D.h"

class BaseProjectile
{
protected:
    float timeLeft = 2.f;
    AEVec2 direction;
    float currLifespan = timeLeft;

public:
    float oriSpeed, speed, size;
    Transform2D transform;
    AEGfxTexture* projTex;

    bool stopped{};
    bool slowed{};

    BaseProjectile(AEVec2 pos, AEVec2 dir, bool skipTextureLoad = false);

    ~BaseProjectile();

    virtual void Update(float deltaTime);
    void Draw(AEGfxVertexList* mesh);
    void Exit();

    float GetLifespan();
    void SetLifespan(float time);
    void SetDirection(AEVec2 newDir);

    void SetSpeed(float speed);
    AEVec2 GetDirection();
};
