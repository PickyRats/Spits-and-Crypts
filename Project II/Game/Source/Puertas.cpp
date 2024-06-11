#include "Puertas.h"
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

Puertas::Puertas() : Entity(EntityType::PUERTAS)
{
	name.Create("puerta");
}

Puertas::~Puertas() {

}

bool Puertas::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();
	cerrada.LoadAnimations("cerrada", "puerta");
	abierta.LoadAnimations("abierta", "puerta");
	return true;
}

bool Puertas::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);
	currentAnim = &abierta;
	
	return true;
}

bool Puertas::Update(float dt)
{
	DrawDoors();
	if (id == 1)
	{
		currentAnim = &abierta;
	}
	else if (id == 2)
	{
	
		currentAnim = &abierta;
	}
	else if (id == 3)
	{
		if(puzle1Completed)
		{
			currentAnim = &abierta;
		}
		else
		{
			currentAnim = &cerrada;
		}
		
	}
	else if (id == 4)
	{
		currentAnim = &cerrada;
	}
	currentAnim->Update();
	return true;
}


void Puertas::SoundManager()
{

}

void Puertas::DrawDoors()
{
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x- 30, position.y-64, &rect);
}



bool Puertas::CleanUp()
{
	return true;
}






