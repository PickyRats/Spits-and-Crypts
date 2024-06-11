#ifndef __PUERTAS_H__
#define __PUERTAS_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;

class Puertas : public Entity
{
public:

	Puertas();

	virtual ~Puertas();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void DrawDoors();

	void SoundManager();

public:
	float speed = 0.2f;
	int id = 0;
	int classId = 0;
	const char* texturePath;
	int platformCollisionCount = 0;
	int scene = 0;

	bool puzle1Completed = false;
	SDL_Texture* texture = NULL;
	Animation* currentAnim;
	Animation cerrada;
	Animation abierta;

};

#endif // __PLAYER_H__