#pragma once
#include <string>
#include "Entity.h"
#include "Dialog.h"
#include "List.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class DialogTrigger : public Entity
{
public:

	DialogTrigger();
	virtual ~DialogTrigger();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void PlayDialog();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void CreateCollider();

	void CreateColliderBig();

	void Interact(int id);

	void GiveMission(int idMission);

	bool DialogMission = false;


private:

	//En caso de querer a�adirle una textura a la entidad
	SDL_Texture* texture;

	//La textura de la cara general para el dialogo
	SDL_Texture* faceTexture;

	//El path de la textura de la entidad
	const char* texturePath;

	//El path de la textura de la cara general
	const char* faceTexturePath;

	//Cuerpo de fisicas, para detectar cuando el jugador esta cerca
	//y activar el dialogo en caso de que el jugador asi lo quiera
	PhysBody* pbody;

	//Lista de los dialogos que puede decir este trigger
	List<Dialog*> dialogues;

	//Variables para saber si se tiene que repetir el dialogo y si
	//ya se a activado minimo una vez
	bool repeatDialog = false;
	bool played = false;
	bool physCreated = false;
	int dialogScene;
	int id;
	bool wasXPressed = false;
	uint dialogs[3];
	
	//Lista de dialogos a repetir
	List<Dialog*> dialoguesRepeat;

	//Lista de dialogos de mision
	List<Dialog*> dialoguesMission;

	int npcid;

};
