#pragma once
#include "AEEngine.h"
#include <vector>

/*!
@file       ParticleManager.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the ParticleManager class, which handles the spawning,
             updating, and rendering of particle effects globally.
*//*_______________________________________________________________________*/

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
    /*!
    @brief    Retrieves the singleton instance of the ParticleManager.
    @return   Pointer to the active ParticleManager.
    *//*______________________________________________________________*/
    static ParticleManager* GetInstance();

    /*!
    @brief    Destroys the singleton instance to free memory.
    *//*______________________________________________________________*/
    static void DestroyInstance();

    /*!
    @brief    Updates the lifespan and physical properties of all active particles.
    @param    dt The time elapsed since the last frame.
    *//*______________________________________________________________*/
    void Update(float dt);

    /*!
    @brief    Renders all active particles to the screen using the provided mesh.
    @param    mesh Pointer to the vertex list used to draw the particles.
    *//*______________________________________________________________*/
    void Draw(AEGfxVertexList* mesh);

    /*!
    @brief    Spawns a standard particle with specific physical and color properties.
    @param    pos The starting coordinates.
    @param    vel The velocity vector.
    @param    scale The size vector.
    @param    life How long the particle exists in seconds.
    @param    r,g,b,a Color values.
    *//*______________________________________________________________*/
    void SpawnParticle(AEVec2 pos, AEVec2 vel, AEVec2 scale, float life, float r, float g, float b, float a);

    /*!
    @brief    Spawns a trailing particle, optionally with a specific texture.
    @param    pos The starting coordinates.
    @param    scale The size vector.
    @param    life Lifespan (defaults to 0.4s).
    @param    r,g,b Color values (defaults to 1.0f).
    @param    rotation The visual rotation of the particle.
    @param    tex Pointer to an optional texture to overlay on the trail.
    *//*______________________________________________________________*/
    void SpawnTrail(AEVec2 pos, AEVec2 scale, float life = 0.4f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float rotation = 0.0f, AEGfxTexture* tex = nullptr);

    /*!
    @brief    Instantly deletes all active particles from the world.
    *//*______________________________________________________________*/
    void ClearAll();

private:
    ParticleManager() = default;
    ~ParticleManager() = default;

    static ParticleManager* instance;
    std::vector<Particle> particles;
};