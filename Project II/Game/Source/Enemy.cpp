#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "SceneCombat.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Hud.h""

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");

}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	enemyId = parameters.attribute("id").as_int();

	return true;
}

bool Enemy::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);

	//LoadAnimations();

	//currentAnim = &idleAnim;

	return true;
}

bool Enemy::Update(float dt)
{
	
	this->dt = dt;
	
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;*/
	if (app->sceneCombat->active)
	{
		DrawEnemy();
	}
	return true;
}

void Enemy::DrawEnemy()
{

	//SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y);

}

bool Enemy::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

// Handles the collision
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
	
}


// Loads the animations
void Enemy::LoadAnimations()
{
	idleAnim.LoadAnimations("idleAnim", "enemy");
}
