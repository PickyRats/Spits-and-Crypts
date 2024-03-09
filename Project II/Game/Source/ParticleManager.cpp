#include "ParticleManager.h"
#include "App.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"


ParticleManager::ParticleManager(bool enabled) : Module(enabled)
{
	name.Create("particlemanager");
}

// Destructor
ParticleManager::~ParticleManager()
{}

// Called before render is available
bool ParticleManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Particle Manager");
	bool ret = true;

	return ret;
}

bool ParticleManager::Start() {

	bool ret = true;

	CreateParticles();

	return ret;
}

// Called before quitting
bool ParticleManager::CleanUp()
{
	bool ret = true;

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

void ParticleManager::CreateParticles() {
	
}

void ParticleManager::UpdateParticles(float dt, float time) {
	
}

void ParticleManager::DrawParticles() {
	
}