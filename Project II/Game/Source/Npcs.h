#ifndef __NPCS_H__
#define __NPCS_H__

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

	void Interact(int id);
	
	// Funcion para dar misiones
	void GiveMission(int idMission);

public:

	const char* texturePath;
		
	SDL_Texture* texture = NULL;
	int npcId; // 1=ABUELA, 2=NIETA

	PhysBody* pbody;
	PhysBody* playerPbody;
	bool physCreated=false;

	Animation* currentAnim;
	Animation idleAnim;

	float dt;


};

#endif // __NPCS_H__