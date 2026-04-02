/*!
@file       BaseProjectile.cpp
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains definitions of functions used by BaseProjectile,
            a base class that all other projectiles will inherit from
*//*_______________________________________________________________________*/

#include "BaseProjectile.h"
#include <iostream>
#include "../System/MeshManager.h"
#include "../particle/ParticleManager.h"

// Static pointer prevents memory leaks from repeated loading
AEGfxTexture* sharedKnifeTex = nullptr;

BaseProjectile::BaseProjectile(AEVec2 pos, AEVec2 dir, bool skipTextureLoad)
    : direction{ dir }
    , oriSpeed{ 500.f }
    , speed{ oriSpeed }
    , size{ 25.f }
    , projTex{ nullptr }
    , collidesWithTiles{true}
{
    transform.SetScale(size, size);
    transform.SetPosition(pos.x, pos.y);

    if (!skipTextureLoad) {
        if (sharedKnifeTex == nullptr) {
            sharedKnifeTex = AEGfxTextureLoad("Assets/Weapons/knife.png");
        }
        projTex = sharedKnifeTex;
    }
}

BaseProjectile::~BaseProjectile()
{
    Exit();
}

void BaseProjectile::Update(float deltaTime)
{
    if (currLifespan > 0.f && !stopped)
    {
        currLifespan -= 1.f * deltaTime;
    }
    speed = slowed ? oriSpeed / 2.f : oriSpeed;
    if (!stopped)
    {
        AEVec2 position = transform.GetPosition();
        position.x += speed * direction.x * deltaTime;
        position.y += speed * direction.y * deltaTime;

        transform.SetRotation(atan2f(direction.y, direction.x) + (PI * 1.25f));
        transform.SetPosition(position.x, position.y);

        if (projTex == sharedKnifeTex) // Only spawn trail if it's the knife
        {
            AEVec2 trailScale = { transform.GetScale().x * 0.5f, transform.GetScale().y * 0.5f };
            ParticleManager::GetInstance()->SpawnTrail(
                transform.GetPosition(),
                trailScale,
                0.1f,               
                0.8f, 0.8f, 0.8f  
            );
        }
    }
}

void BaseProjectile::Draw(AEGfxVertexList* mesh)
{
    if (projTex != nullptr && (unsigned long long)projTex != 0xFFFFFFFFFFFFFFFFULL) {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        AEGfxTextureSet(projTex, 0, 0);
        AEGfxSetTransform(transform.GetMatrix().m);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    }
    else
    {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxTextureSet(nullptr, 0, 0);

        AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);

        // Add this line to force the sword wave to ignore the purple paint
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);
        AEGfxSetTransform(transform.GetMatrix().m);
        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
    }

    // THE UNIVERSAL CLEANUP BRUSH
    // Put this at the end of ALL drawing functions
    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f); 
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);     

    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);    
}

void BaseProjectile::Exit()
{
    // Burst of particles when the knife hits something
    AEVec2 pos = transform.GetPosition();
    for (int i = 0; i < 3; i++) {
        AEVec2 randomVel = { (rand() % 200) - 100.0f, (rand() % 200) - 100.0f };
        ParticleManager::GetInstance()->SpawnParticle(
            pos, randomVel, { 10.0f, 10.0f }, 0.2f, 0.8f, 0.8f, 0.8f, 1.0f
        );
    }
}

float BaseProjectile::GetLifespan() { return currLifespan; }
void BaseProjectile::SetLifespan(float time) { currLifespan = time; }
AEVec2 BaseProjectile::GetDirection() { return direction; }
void BaseProjectile::SetDirection(AEVec2 newDir) { direction = newDir; }
void BaseProjectile::SetSpeed(float newSpeed) { this->speed = newSpeed; }

void ShutdownKnifeResources() {
    if (sharedKnifeTex != nullptr) {
        AEGfxTextureUnload(sharedKnifeTex);
        sharedKnifeTex = nullptr;
    }
}