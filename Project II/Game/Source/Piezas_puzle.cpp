#include "Piezas_Puzle.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneTemple.h"
#include "SceneFloor1.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Hud.h""

Piezas_puzle::Piezas_puzle() : Entity(EntityType::PIEZAS)
{
	name.Create("Piece");

}

Piezas_puzle::~Piezas_puzle() {

}

bool Piezas_puzle::Awake() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	pieceId = parameters.attribute("id").as_int();

	return true;
}

bool Piezas_puzle::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x-30, position.y-16, 22, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;
	physCreated = true;
	//LoadAnimations();

	//currentAnim = &idleAnim;

	return true;
}

bool Piezas_puzle::Update(float dt)
{
		
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;
		DrawPieces();
	
		return true;
}

void Piezas_puzle::Interact(int id)
{
	
}



void Piezas_puzle::DrawPieces()
{

	//SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x , position.y );

}

bool Piezas_puzle::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

// Handles the collision
void Piezas_puzle::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	case ColliderType::PLAYER:
		Interact(pieceId);
		LOG("Collision UNKNOWN");
		break;
	}

}
