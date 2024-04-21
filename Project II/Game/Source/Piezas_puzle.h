#pragma once

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class Piezas_puzle :public Entity
{
public:

	Piezas_puzle();
	virtual ~Piezas_puzle();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void LoadAnimations();

	void DrawPieces();

	void Interact(int id);

	int sceneNum = 5;
	

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	int pieceId; // 1=ABUELA, 2=NIETA

	PhysBody* pbody;
	PhysBody* playerPbody;
	bool physCreated = false;

	Animation* currentAnim;
	Animation idleAnim;

	float dt;


};
