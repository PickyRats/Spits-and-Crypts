#include "ParticleManager.h"
#include "App.h"
#include "Textures.h"
#include "Defs.h"
#include "Log.h"

#include "SceneVillage.h"
#include "SceneFloor1.h"

ParticleManager::ParticleManager(bool enabled) : Module(enabled)
{
    name.Create("particlemanager");
}

// Destructor
ParticleManager::~ParticleManager()
{
    for (auto particle : particles) {
        delete particle;
    }
}

// Called before render is available
bool ParticleManager::Awake(pugi::xml_node& config)
{
    LOG("Loading Particle Manager");
    bool ret = true;

    return ret;
}

bool ParticleManager::Start() {
    bool ret = true;

    // Load textures
    texture1 = app->tex->Load("Assets/Textures/torch1.png");
    texture2 = app->tex->Load("Assets/Textures/torch2.png");
    texture3 = app->tex->Load("Assets/Textures/torch3.png");
    texture4 = app->tex->Load("Assets/Textures/torch4.png");

    return ret;
}

// Called before quitting
bool ParticleManager::CleanUp()
{
    bool ret = true;

    for (auto particle : particles) {
        delete particle;
    }
    particles.clear();

    if (texture1) {
        SDL_DestroyTexture(texture1);
        texture1 = nullptr;
    }
    if (texture2) {
        SDL_DestroyTexture(texture2);
        texture2 = nullptr;
    }

    return ret;
}

bool ParticleManager::Update(float dt)
{
    bool ret = true;

    float time = SDL_GetTicks() / 1000.0f;

    UpdateParticles(dt, time);
    DrawParticles();

    return ret;
}

void ParticleManager::UpdateParticles(float dt, float time) {


    // Create new particle

    if (app->sceneVillage->active)
    {
        CreateParticles(dt, 4700, 530, 600.0f, 0, 1);
    }
    
	if (app->sceneFloor1->active)
    {
		CreateParticles(dt, 512, 1980, 500.0f, 1, 0);
		CreateParticles(dt, 1088, 2040, 500.0f, 2, 0);
		CreateParticles(dt, 5696, 1787, 500.0f, 3, 0);
		CreateParticles(dt, 9472, 1216, 500.0f, 3, 0);
	} 

    // Update existing particles
    for (auto it = particles.begin(); it != particles.end();) {
        Particle* particle = *it;
        particle->Update(dt);
        if (!particle->IsAlive()) {
            delete particle;
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ParticleManager::CreateParticles(float dt, int initialPositionX, int initialPositionY, float lifeTime, int index, int type)
{
    timeAccumulator[index] += dt;

    while (timeAccumulator[index] >= particleInterval) {
        // Create new particle
        SDL_Texture* chosenTexture = nullptr;
        int randomIndex = rand() % 3;

        switch (randomIndex) {
        case 0:
            chosenTexture = texture1;
            break;
        case 1:
            chosenTexture = texture2;
            break;
        case 2:
            chosenTexture = texture3;
            break;
        case 3:
			chosenTexture = texture4;
			break;
        default:
            break;
        }

        int x = rand() % (60) + initialPositionX; // X Initial Position
        int y = initialPositionY; // Y Initial position
        particles.push_back(new Particle(chosenTexture, x, y, lifeTime, initialPositionX + 30, type)); // Lifetime

        timeAccumulator[index] -= particleInterval;
    }
}

void ParticleManager::DrawParticles() {
    for (auto particle : particles) {
        particle->Draw(app->render->renderer);
    }
}
