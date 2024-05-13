#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "DialogTriggerEntity.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnExitCollision(PhysBody* physA, PhysBody* physB);

	void LoadAnimations();
	
	void ToggleGodMode();

	void DrawPlayer();

	void LeftMovement();

	void RightMovement();

	void UpMovement();

	void DownMovement();

	void Jump();
	
	void WalkingSound();

	void Respaw();

	void CreateBody();

	void DestroyBody();

public:
	float speed = 0.2f;
	int id = 0;
	const char* texturePath;

	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	PhysBody* playerPbody;

	Animation* currentAnim;
	Animation idleAnim;

	bool isWalking = false;

	bool isFacingRight = true;

	bool godMode = false;

	bool isjumping = false;

	bool isClimbing = false;

	bool doorAldea = false;
	bool doorShop = false;
	bool doorOasis = false;
	bool doorTemple = false;
	bool doorFlor1 = false;
	bool enterCombat = false;

	bool step = false;
	bool walkingSoundPlaying = false;
	bool climbingSoundPlaying = false;

	b2Transform initialTransform;

	b2Vec2 vel;

	float dt;

	bool contactDialogTrigger = false;
	DialogTrigger* dialogTriggerContact = nullptr;
	bool isCombat = false;
	bool isVisible = true;

	uint stepsFx;
	uint climbFx;
	uint doorFx;


	bool isDead = false;


};

#endif // __PLAYER_H__