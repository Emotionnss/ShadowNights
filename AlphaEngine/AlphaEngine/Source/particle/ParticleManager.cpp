#include "ParticleManager.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {
    if (!instance) instance = new ParticleManager();
    return instance;
}

void ParticleManager::DestroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void ParticleManager::SpawnParticle(AEVec2 pos, AEVec2 vel, AEVec2 scale, float life, float r, float g, float b, float a) {
    Particle p;
    p.position = pos;
    p.velocity = vel;
    p.scale = scale;
    p.rotation = 0.0f;
    p.life = life;
    p.maxLife = life;
    p.r = r; p.g = g; p.b = b; p.a = a;

    particles.push_back(p);
}

void ParticleManager::SpawnTrail(AEVec2 pos, AEVec2 scale, float life, float r, float g, float b) {
    // Trails have no velocity, spawn and fade out
    SpawnParticle(pos, { 0.0f, 0.0f }, scale, life, r, g, b, 0.5f);
}

void ParticleManager::Update(float dt) {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->life -= dt;

        if (it->life <= 0.0f) {
            it = particles.erase(it); // Remove dead particles
        }
        else {
            // Move particle
            it->position.x += it->velocity.x * dt;
            it->position.y += it->velocity.y * dt;

            // Fade out smoothly over its lifespan
            it->a = (it->life / it->maxLife) * 0.5f; 

            ++it;
        }
    }
}

void ParticleManager::Draw(AEGfxVertexList* mesh) {
    if (!mesh || particles.empty()) return;

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    for (const auto& p : particles) {
        AEGfxSetTransparency(p.a);
        AEGfxSetColorToMultiply(p.r, p.g, p.b, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); // Reset additive color

        AEMtx33 scale, rot, trans, finalMtx;
        AEMtx33Scale(&scale, p.scale.x, p.scale.y);
        AEMtx33Rot(&rot, p.rotation);
        AEMtx33Trans(&trans, p.position.x, p.position.y);

        AEMtx33Concat(&finalMtx, &rot, &scale);
        AEMtx33Concat(&finalMtx, &trans, &finalMtx);

        AEGfxSetTransform(finalMtx.m);
        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
    }

    // Clean up graphics state after drawing all particles
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

void ParticleManager::ClearAll() {
    particles.clear();
}