/******************************************************************************/
/*!
\file		ParticleManager.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the particle
            manager.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/
#include "ParticleManager.h"
#include <random>
#include <ctime>

float randomFloat(float min, float max)
{
    static std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

/******************************************************************************/
/*!
    Load Textures
*/
/******************************************************************************/
ParticleManager::ParticleManager()
{
    particleTexture = AEGfxTextureLoad("Assets/MainLevel/ml_Explosion.png");
}

/******************************************************************************/
/*!
    Destructor
*/
/******************************************************************************/
ParticleManager::~ParticleManager()
{
    Clear();
}

/******************************************************************************/
/*!
    Generate particle
*/
/******************************************************************************/
void ParticleManager::Emit(const AEVec2& position, const AEVec2& velocity, float lifespan)
{
    Particle particle{};
    particle.position = position;
    particle.velocity = velocity;
    particle.lifespan = lifespan;
    particle.currentAge = 0.0f;

    // Generate random colors for the particle
    particle.color.r = randomFloat(0.0f, 1.0f);
    particle.color.g = randomFloat(0.0f, 1.0f);
    particle.color.b = randomFloat(0.0f, 1.0f);

    particles.push_back(particle);
}

/******************************************************************************/
/*!
    Update particle
*/
/******************************************************************************/
void ParticleManager::Update(float dt)
{
    for (size_t i = 0; i < particles.size();)
    {
        particles[i].position.x += particles[i].velocity.x * dt;
        particles[i].position.y += particles[i].velocity.y * dt;
        particles[i].currentAge += dt;

        if (particles[i].currentAge >= particles[i].lifespan)
        {
            particles.erase(particles.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

/******************************************************************************/
/*!
    Draw Particle
*/
/******************************************************************************/
void ParticleManager::Draw(AEGfxVertexList* pMesh)
{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    for (const Particle& particle : particles)
    {
        AEMtx33 scale, rot, trans;

        float alpha = 1.0f - (particle.currentAge / particle.lifespan); // Fade the particle as it ages
        AEGfxSetTintColor(particle.color.r, particle.color.g, particle.color.b, alpha); // Use the particle's color with variable alpha based on age

        float size = 10.f;
        AEMtx33Scale(&scale, size, size);
        AEMtx33Rot(&rot, 0.7f);
        AEMtx33Trans(&trans, particle.position.x, particle.position.y);
        AEMtx33Concat(&particle_transform, &rot, &scale);
        AEMtx33Concat(&particle_transform, &trans, &particle_transform);

        // Drawing the particle at its current position using a texture, color, and size based on its age
        AEGfxTextureSet(particleTexture, 0, 100.f);
        AEGfxSetTransform(particle_transform.m);
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
}

/******************************************************************************/
/*!
    Clear particles
*/
/******************************************************************************/
void ParticleManager::Clear()
{
    particles.clear();
}
