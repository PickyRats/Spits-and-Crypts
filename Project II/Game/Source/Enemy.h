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

	void SetCombatAnimation(int animationIndex);

	bool AnimationFinished();

	void ShotArrow();

public:

	const char* texturePath;
		
	SDL_Texture* texture = NULL;
	SDL_Texture* arrowTexture = NULL;
	int enemyId;

	PhysBody* pbody;

	Animation* currentAnim;
	Animation	idleAnim,
				walkAnim,
				climbAnim,
				attackAnim,
				abilityAnim,
				hitAnim,
				deathAnim;

	Animation* currentAttackAnim;
	Animation arrowAnim;

	float dt;

	SDL_Texture* tileTexture;

	bool physCreated = false;
	bool isActive = false;
};

