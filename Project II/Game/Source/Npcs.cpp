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
#include "SceneChoza.h"
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

	LoadAnimations();

	currentAnim = &idleAnim;

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
	int sceneChoza = app->sceneChoza->sceneNum;
	int npcScene= parameters.attribute("scene").as_int();

	//Comprobar si la escena actual es la que tiene el npc y crearlo si es así
	if (app->sceneVillage->active && sceneVillage == npcScene)
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
	else if (app->sceneChoza->active && sceneChoza == npcScene)
	{
		DrawNpcs();
	}
	
	currentAnim->Update();
	if (currentAnim == &interactAnim && currentAnim->HasFinished())
	{
		currentAnim->Reset();
		currentAnim->loopCount = 0;
		currentAnim = &idleAnim;
	} 
	return true;
}

void Npcs::DrawNpcs()
{
	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect);
	

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
		//Interact(npcId);
		LOG("Collision UNKNOWN");
		break;
	}
	
}


// Loads the animations
void Npcs::LoadAnimations()
{
	if (npcId == 1)
	{
		idleAnim.LoadAnimations("idleAnim", "abuela");
	}
	else if (npcId == 2)
	{
		idleAnim.LoadAnimations("idleAnim", "nieta");
	}
	else if (npcId == 3)
	{
		
	}
	else if (npcId == 4)
	{
		
	}
	else if (npcId == 5)
	{
		
	}
	else if (npcId == 6)
	{
		
	}
	else if (npcId == 7)
	{
		idleAnim.LoadAnimations("idleAnim", "Mercante");
		interactAnim.LoadAnimations("interactAnim", "Mercante");
	}
	else if (npcId == 8)
	{
		idleAnim.LoadAnimations("idleAnim", "tabernero");
		interactAnim.LoadAnimations("interactAnim", "tabernero");
	}
	else if (npcId == 9)
	{
		idleAnim.LoadAnimations("idleAnim", "Paalaya");
		interactAnim.LoadAnimations("interactAnim", "Paalaya");
	}

	
}
