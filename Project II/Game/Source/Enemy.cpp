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

	attackDamage = 10;

	return true;
}

bool Enemy::Update(float dt)
{
	
	this->dt = dt;
	
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;*/
	if (health <= 0 && !isDead)
	{
		isDead = true;
	}
	if (app->sceneCombat->active && !isDead)
	{
		DrawEnemy();
	}
	return true;
}

void Enemy::DrawEnemy()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect);

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
	if (enemyId == 1)
	{
		idleAnim.LoadAnimations("idleAnim", "enemy1");
		walkAnim.LoadAnimations("walkAnim", "enemy1");
		climbAnim.LoadAnimations("climbAnim", "enemy1");
		attackAnim.LoadAnimations("attackAnim", "enemy1");
		abilityAnim.LoadAnimations("abilityAnim", "enemy1");
		hitAnim.LoadAnimations("hitAnim", "enemy1");
		deathAnim.LoadAnimations("deathAnim", "enemy1");
	}
}

void Enemy::SetCombatAnimation(int animationIndex)
{
	switch (animationIndex)
	{
	case 0:
		currentAnim = &idleAnim;
		break;
	case 1:
		currentAnim = &walkAnim;
		break;
	case 2:
		currentAnim = &climbAnim;
		break;
	case 3:
		currentAnim = &attackAnim;
		currentAnim->ResetLoopCount();
		currentAnim->Reset();
		break;
	case 4:
		currentAnim = &abilityAnim;
		currentAnim->ResetLoopCount();
		currentAnim->Reset();
		break;
	case 5:
		currentAnim = &hitAnim;
		currentAnim->ResetLoopCount();
		currentAnim->Reset();
		break;
	case 6:
		currentAnim = &deathAnim;
		break;
	}
}


bool Enemy::AnimationFinished()
{
	return currentAnim->HasFinished();
}
