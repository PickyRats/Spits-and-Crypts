#include "Npcs.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
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
	int npcScene= parameters.attribute("scene").as_int();

	//Comprobar si la escena actual es la que tiene el npc y crearlo si es así
	if (app->sceneVillage->active && sceneVillage== npcScene)
	{
		//Crear el cuerpo físico una sola vez
		if (physCreated==false)
		{
			pbody = app->physics->CreateCircle(position.x + 50, position.y, 22, bodyType::DYNAMIC);
			pbody->listener = this;
			pbody->ctype = ColliderType::NPC;
			physCreated = true;
		}
		

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;
		DrawNpcs();
	}
	else if (app->sceneTemple->active && sceneTemple== npcScene)
	{
		if (physCreated==false)
		{
			pbody = app->physics->CreateCircle(position.x + 50, position.y, 22, bodyType::DYNAMIC);
			pbody->listener = this;
			pbody->ctype = ColliderType::NPC;
			physCreated = true;
		}
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
		DrawNpcs();
	}
	else
	{
		return true;
	}
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
	default:
		break;
	}

}

void Npcs::GiveMission(int idMission) 
{
	switch (idMission)
	{
	case 1:
		printf(" La abuela dijo: Hola soy la abuela \n");
		break;
	case 2:
		printf(" y la nieta respondio: JUEPUTA \n");
		break;
	case 3:
		printf(" Soy maat imbecil \n");
		break;
	case 4:
		printf( "comeme toth la polla \n");
		break;
	case 5:
		printf(" Soy ISIS (Estado Islamico) \n");
		break;
	case 6:
		printf("Eres Horruroso xd \n");
		break;
	default:
		break;
	}
}

void Npcs::DrawNpcs()
{

	//SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x-20, position.y-300);

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
