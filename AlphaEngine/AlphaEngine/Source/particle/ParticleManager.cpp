#include "ParticleManager.h"
#include "../System/MeshManager.h"

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
    Particle p = {};
    p.position = pos;
    p.velocity = vel;
    p.scale = scale;
    p.rotation = 0.0f;
    p.life = life;
    p.maxLife = life;
    p.r = r; p.g = g; p.b = b; p.a = a;
    p.texture = nullptr;

    particles.push_back(p);
}

void ParticleManager::SpawnTrail(AEVec2 pos, AEVec2 scale, float life, float r, float g, float b, float rotation, AEGfxTexture* tex) {
    Particle p = {};
    p.position = pos;
    p.velocity = { 0.0f, 0.0f };
    p.scale = scale;
    p.life = life;
    p.maxLife = life;
    p.r = r; p.g = g; p.b = b; p.a = 0.5f;

    p.rotation = rotation;
    p.texture = tex;

    particles.push_back(p);
}

void ParticleManager::Update(float dt) {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->life -= dt;

        if (it->life <= 0.0f) {
            it = particles.erase(it);
        }
        else {
            it->position.x += it->velocity.x * dt;
            it->position.y += it->velocity.y * dt;

            // --- THE TAPER EFFECT ---
            it->a = (it->life / it->maxLife) * 0.8f;
            it->scale.x -= it->scale.x * 5.0f * dt;
            it->scale.y -= it->scale.y * 5.0f * dt;

            ++it;
        }
    }
}

void ParticleManager::Draw(AEGfxVertexList* mesh) {
    if (!mesh || particles.empty()) return;

    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    for (const auto& p : particles) {
        if (p.texture != nullptr) continue; // Skip textures for now

        AEMtx33 scale, rot, trans, finalMtx;
        AEMtx33Scale(&scale, p.scale.x, p.scale.y);
        AEMtx33Rot(&rot, p.rotation);
        AEMtx33Trans(&trans, p.position.x, p.position.y);
        AEMtx33Concat(&finalMtx, &rot, &scale);
        AEMtx33Concat(&finalMtx, &trans, &finalMtx);
        AEGfxSetTransform(finalMtx.m);

        AEGfxSetTransparency(p.a);
        AEGfxSetColorToMultiply(p.r, p.g, p.b, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(CIRCLE), AE_GFX_MDM_TRIANGLES);
    }

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    for (const auto& p : particles) {
        if (p.texture == nullptr) continue; // Skip shape

        AEMtx33 scale, rot, trans, finalMtx;
        AEMtx33Scale(&scale, p.scale.x, p.scale.y);
        AEMtx33Rot(&rot, p.rotation);
        AEMtx33Trans(&trans, p.position.x, p.position.y);
        AEMtx33Concat(&finalMtx, &rot, &scale);
        AEMtx33Concat(&finalMtx, &trans, &finalMtx);
        AEGfxSetTransform(finalMtx.m);

        AEGfxSetTransparency(p.a);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEGfxTextureSet(p.texture, 0, 0);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    }

	//universal cleanup brush
    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);
}

void ParticleManager::ClearAll() {
    particles.clear();
}