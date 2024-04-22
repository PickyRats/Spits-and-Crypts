#include "Trap.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneTemple.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Trap::Trap() : Entity(EntityType::TRAP)
{
	name.Create("trap");
}

Trap::~Trap() {}

bool Trap::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Trap::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x, position.y, 32, 10, bodyType::STATIC);
	pbody->ctype = ColliderType::TRAP;

	return true;
}

bool Trap::Update(float dt)
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Trap::CleanUp()
{
	return true;
}
