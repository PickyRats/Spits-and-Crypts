#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Hud.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Player::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);

	LoadAnimations();

	currentAnim = &idleAnim;

	pbody = app->physics->CreateCircle(position.x + 50, position.y, 22, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	playerPbody = app->physics->CreateRectangleSensor(position.x + 50, position.y, 24, 60, bodyType::DYNAMIC);
	playerPbody->ctype = ColliderType::PLAYER_BODY;

	initialTransform = pbody->body->GetTransform();

	ToggleGodMode();

	return true;
}

bool Player::Update(float dt)
{
	this->dt = dt;

	//godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) ToggleGodMode();

	if (health <= 0 && !isDead)
	{
		isDead = true;
	}

	if (!isDead && !isCombat)
	{
		currentAnim = &idleAnim;

		vel = pbody->body->GetLinearVelocity();

		if (!godMode)
		{

			//player movement
			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				LeftMovement();
			}

			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				RightMovement();
			}

			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
			{
				isWalking = false;
				vel.x = 0;
			}

			//jump
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isjumping)
			{	
				Jump();
			}
		
			/*vel.y = -GRAVITY_Y;*/
			pbody->body->SetLinearVelocity(vel);
			playerPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);
		}
		else
		{
			//god mode
			vel.SetZero();

			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				vel.y = -speed * 2 * dt;
			}
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				vel.y = speed * 2 * dt;
			}
			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				vel.x = -speed * 2 * dt;
			}
			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				vel.x = speed * 2 * dt;
			}

			pbody->body->SetLinearVelocity(vel);
			playerPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;
	}
	else
	{
		// death
		pbody->body->SetLinearVelocity({ 0, 0 });
	}

	DrawPlayer();

	currentAnim->Update();

	//printf("\r cameraX: %d cameraY: %d positionX: %d positionY %d", app->render->camera.x, app->render->camera.y, position.x, position.y);
	return true;
}

void Player::LeftMovement()
{
	isFacingRight = false;
	vel.x = -speed * 1 * dt;
}

void Player::RightMovement()
{
	isFacingRight = true;
	vel.x = speed * 1 * dt;
}

void Player::Jump()
{
	vel.y = -speed * 2 * dt;
	isjumping = true;
}

void Player::DrawPlayer()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();

	if (isFacingRight)
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect, SDL_FLIP_HORIZONTAL);
	}
}

// Toggles the god mode
void Player::ToggleGodMode()
{
	godMode = !godMode;

	if (godMode)
	{
		pbody->body->GetFixtureList()->SetSensor(true);
		pbody->body->SetGravityScale(0.0f);
		pbody->body->SetLinearVelocity({ 0, 0 });
		currentAnim = &idleAnim;
	}
	else
	{
		pbody->body->GetFixtureList()->SetSensor(false);
		pbody->body->SetGravityScale(1.0f);
	}

}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

// Handles the collision
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		isjumping = false;
		break;
	}

}

// Loads the animations
void Player::LoadAnimations()
{
	idleAnim.LoadAnimations("idleAnim", "player");
}