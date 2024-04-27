#include "PiezasPuzle.h"
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
#include "Hud.h"
#include "Puzzle.h"

PiezasPuzle::PiezasPuzle() : Entity(EntityType::PIEZAS)
{
	name.Create("Piece");

}

PiezasPuzle::~PiezasPuzle() {

}

bool PiezasPuzle::Awake() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	pieceId = parameters.attribute("id").as_int();

	return true;
}

bool PiezasPuzle::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x-30, position.y-16, 22, 22, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;
	physCreated = true;

	return true;
}

bool PiezasPuzle::Update(float dt)
{
		
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;
		DrawPieces();
	
		return true;
}

void PiezasPuzle::Interact()
{
	app->puzzle->pieceCollected[pieceId] = true;
}


void PiezasPuzle::DrawPieces()
{

	if (!app->puzzle->pieceCollected[pieceId]) app->render->DrawTexture(texture, position.x , position.y );

}

bool PiezasPuzle::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

// Handles the collision
void PiezasPuzle::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	case ColliderType::PLAYER:
		Interact();
		LOG("Collision UNKNOWN");
		break;
	}

}
