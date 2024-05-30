#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "Module.h"
#include "List.h"
#include "Particle.h"
#include <vector>

class ParticleManager : public Module
{
public:

    ParticleManager(bool enabled = true);

    // Destructor
    virtual ~ParticleManager();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called after Awake
    bool Start();

    // Called every frame
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Additional methods
    void CreateParticles();
    void UpdateParticles(float dt, float time);
    void TorchParticles(float dt, int initialPositionX, int initialPositionY, float lifeTime, int index);
    void DrawParticles();

private:
    std::vector<Particle*> particles;
    SDL_Texture* texture1;
    SDL_Texture* texture2;
    SDL_Texture* texture3;
    SDL_Texture* texture4;
    float timeAccumulator[3];
    const float particleInterval = 6.0f;
};

#endif // __PARTICLEMANAGER_H__
