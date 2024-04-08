#pragma once

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class Enemy:public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void LoadAnimations();

	void DrawEnemy();

public:

	const char* texturePath;
		
	SDL_Texture* texture = NULL;
	int enemyId;

	PhysBody* pbody;

	Animation* currentAnim;
	Animation idleAnim;

	float dt;

	SDL_Texture* tileTexture;
};

