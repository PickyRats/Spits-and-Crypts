#include "Npcs.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneFloor1.h"
#include "SceneTemple.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Hud.h""

Npcs::Npcs() : Entity(EntityType::NPCS)
{
	name.Create("Npcs");

}

Npcs::~Npcs() {

}

bool Npcs::Awake() {

	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	npcId = parameters.attribute("id").as_int();

	return true;
}

bool Npcs::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);

	//LoadAnimations();

	//currentAnim = &idleAnim;

	return true;
}

bool Npcs::Update(float dt)
{
	this->dt = dt;
	int sceneVillage = app->sceneVillage->sceneNum;
	int sceneTemple = app->sceneTemple->sceneNum;
	int sceneShop = app->sceneShop->sceneNum;
	int sceneOasisFaraon = app->sceneOasisFaraon->sceneNum;
	int sceneFloor1 = app->sceneFloor1->sceneNum;
	int npcScene= parameters.attribute("scene").as_int();

	//Comprobar si la escena actual es la que tiene el npc y crearlo si es así
	if (app->sceneVillage->active && sceneVillage == npcScene)
	{
		DrawNpcs();
	}
	else if (app->sceneTemple->active && sceneTemple== npcScene)
	{
		DrawNpcs();
	}
	else if (app->sceneShop->active && sceneShop == npcScene)
	{
		DrawNpcs();
	}
	else if (app->sceneOasisFaraon->active && sceneOasisFaraon == npcScene)
	{
		DrawNpcs();
	}
	else if (app->sceneFloor1->active && sceneFloor1 == npcScene)
	{
		DrawNpcs();
	}

	return true;
}

void Npcs::Interact(int id) 
{
	switch (id)
	{
	case 1:
		GiveMission(1);
		break;
	case 2:
		GiveMission(2);
		break;
	case 3:
		GiveMission(3);
		break;
	case 4:
		GiveMission(4);
		break;
	case 5:
		GiveMission(5);
		break;
	case 6:
		GiveMission(6);
		break;
	case 7:
		GiveMission(7);
		break;
	case 8:
		GiveMission(8);
		break;
	case 9:
		GiveMission(9);
		break;
	default:
		break;
	}

}

void Npcs::GiveMission(int idMission) 
{
	switch (idMission)
	{
	case 1:
		mission1 = true;
		printf("ves a buscar a mi nieta  \n");
		break;                   
	case 2:
		printf("  la nieta  \n");
		break;
	case 3:
		printf(" Soy maat \n");
		break;
	case 4:
		printf( " toth  \n");
		break;
	case 5:
		printf(" Soy ISIS  \n");
		break;
	case 6:
		printf(" Horrus  \n");
		break;
	case 7:
		printf("  mi humilde tienda \n");
		break;
	case 8:
		printf(" Soy el tabernero\n");
		break;
	case 9:
		printf(" Que haces pidiendome wishky con cereales \n");
		break;
	default:
		break;
	}
}

void Npcs::DrawNpcs()
{

	//SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y);

}

bool Npcs::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

// Handles the collision
void Npcs::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	case ColliderType::PLAYER:
		Interact(npcId);
		LOG("Collision UNKNOWN");
		break;
	}
	
}


// Loads the animations
void Npcs::LoadAnimations()
{
	idleAnim.LoadAnimations("idleAnim", "npc");
}
