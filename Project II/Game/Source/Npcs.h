#pragma once

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class Npcs:public Entity
{
public:
	Npcs();
	virtual ~Npcs();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void LoadAnimations();

	void DrawNpcs();


		




};

