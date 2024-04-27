#ifndef __PIEZASPUZLE_H__
#define __PIEZASPUZLE_H__

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class PiezasPuzle :public Entity
{
public:

	PiezasPuzle();
	virtual ~PiezasPuzle();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void DrawPieces();

	void Interact();
	

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	int pieceId; 

	PhysBody* pbody;

	bool physCreated = false;

	float dt;

};


#endif // __PIEZASPUZLE_H__