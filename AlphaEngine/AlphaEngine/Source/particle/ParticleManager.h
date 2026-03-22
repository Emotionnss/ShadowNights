#pragma once
#include "AEEngine.h"
#include <vector>

struct Particle {
    AEVec2 position;
    AEVec2 velocity;
    AEVec2 scale;
    float rotation;
    float life;
    float maxLife;
    float r, g, b, a;

    AEGfxTexture* texture = nullptr;
};

class ParticleManager {
public:
    static ParticleManager* GetInstance();
    static void DestroyInstance();

    void Update(float dt);
    void Draw(AEGfxVertexList* mesh);

    void SpawnParticle(AEVec2 pos, AEVec2 vel, AEVec2 scale, float life, float r, float g, float b, float a);

    void SpawnTrail(AEVec2 pos, AEVec2 scale, float life = 0.4f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float rotation = 0.0f, AEGfxTexture* tex = nullptr);

    void ClearAll();

private:
    ParticleManager() = default;
    ~ParticleManager() = default;

    static ParticleManager* instance;
    std::vector<Particle> particles;
};