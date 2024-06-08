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
	arrowTexture = app->tex->Load("Assets/Textures/ArrowSpriteSheet.png");

	LoadAnimations();

	currentAnim = &idleAnim;

	isFacingRight = false;

	attackDamage = 30;

	return true;
}

bool Enemy::Update(float dt)
{
	
	this->dt = dt;
	currentAnim->Update();
	if (arrow)
	{
		currentAttackAnim->Update();
		if (isFacingRight) arrowPos.x += 5;
		else arrowPos.x -= 5;
	}
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

	if (isFacingRight) app->render->DrawTexture(texture, position.x - 30, position.y - 62, &rect);
	else app->render->DrawTexture(texture, position.x - 30, position.y - 62, &rect, SDL_FLIP_HORIZONTAL);

	if (arrow)
	{
		SDL_Rect arrowRect = currentAttackAnim->GetCurrentFrame();

		if (isFacingRight) app->render->DrawTexture(arrowTexture, arrowPos.x, position.y - 25, &arrowRect);
		else app->render->DrawTexture(arrowTexture, arrowPos.x - 100, position.y - 25, &arrowRect, SDL_FLIP_HORIZONTAL);
	}

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
	else if (enemyId == 2)
	{
		idleAnim.LoadAnimations("idleAnim", "enemy2");
		walkAnim.LoadAnimations("walkAnim", "enemy2");
		climbAnim.LoadAnimations("climbAnim", "enemy2");
		attackAnim.LoadAnimations("attackAnim", "enemy2");
		abilityAnim.LoadAnimations("abilityAnim", "enemy2");
		hitAnim.LoadAnimations("hitAnim", "enemy2");
		deathAnim.LoadAnimations("deathAnim", "enemy2");
		arrowAnim.LoadAnimations("arrowAnim", "enemy2");
		useProjectile = true;
		currentAttackAnim = &arrowAnim;
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

void Enemy::ShotArrow()
{
	if (!arrow && currentAnim->GetCurrentFrameCount() >= 24)
	{
		arrow = true;
		arrowPos.x = position.x;
	}
}