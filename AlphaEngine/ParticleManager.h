/******************************************************************************/
/*!
\file		ParticleManager.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the particle
            manager.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/
#pragma once
#include <vector>
#include "AEEngine.h"

struct Color {
    float r, g, b;
};

class ParticleManager {
public:
    ParticleManager();
    ~ParticleManager();

    void Emit(const AEVec2& position, const AEVec2& velocity, float lifespan);
    void Update(float dt);
    void Draw(AEGfxVertexList* pMesh);
    void Clear();

private:
    AEGfxTexture* particleTexture;
    AEMtx33 particle_transform;

    struct Particle {
        AEVec2 position;
        AEVec2 velocity;
        float lifespan;
        float currentAge;
        Color color;

    };

    std::vector<Particle> particles;
};