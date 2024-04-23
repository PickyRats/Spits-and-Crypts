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
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneTemple.h"
#include "SceneFloor1.h"

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
	id = parameters.attribute("id").as_int();

	return true;
}

bool Player::Start() {

	//initilize texture
	texture = app->tex->Load(texturePath);

	LoadAnimations();

	currentAnim = &idleAnim;

	stepsFx = app->audio->LoadFx("Assets/Audio/Fx/Footsteps_Fx.wav");
	//ToggleGodMode();

	if (id == 1)
	{
		CreateBody();
		isVisible = true;
	}
	else isVisible = false;

	return true;
}

bool Player::Update(float dt)
{
	this->dt = dt;

	//godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !isCombat) ToggleGodMode();

	if (health <= 0 && !isDead)
	{
		isDead = true;
	}
	if (isDead && !isCombat) Respaw();

	if (isVisible)
	{
		if (!isDead && !isCombat)
		{
			currentAnim = &idleAnim;

			vel = pbody->body->GetLinearVelocity();

			if (!godMode)
			{
				//Funcion para hacer sonidos
				WalkingSound();
				//player movement
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{

					LeftMovement();
				}

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{

					RightMovement();
				}

				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
				{

					isWalking = false;
					vel.x = 0;
				}
				//Climbing
				if (isClimbing)
				{
					if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
					{
						UpMovement();
					}
					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
					{
						DownMovement();
					}
					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
					{
						vel.y = 0;
					}
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

				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				{
					vel.y = -speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				{
					vel.y = speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					vel.x = -speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					vel.x = speed * 2 * dt;
				}

				pbody->body->SetLinearVelocity(vel);
				playerPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);
			}

			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 42;
		}
		else if (id == 1)
		{
			// death
			pbody->body->SetLinearVelocity({ 0, 0 });
		}

		DrawPlayer();

		currentAnim->Update();
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (doorAldea) {
				if (app->sceneShop->active) app->fade->Fade((Module*)app->sceneShop, (Module*)app->sceneVillage, 60.0f);
				else if (app->sceneOasisFaraon->active) app->fade->Fade((Module*)app->sceneOasisFaraon, (Module*)app->sceneVillage, 60.0f);
				else if (app->sceneTemple->active) app->fade->Fade((Module*)app->sceneTemple, (Module*)app->sceneVillage, 60.0f);
				else if (app->sceneFloor1->active) app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneVillage, 60.0f);
				doorAldea = false;
			}
			else if (doorOasis)
			{
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneOasisFaraon, 60.0f);
				doorOasis = false;
			}
			else if (doorShop)
			{
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneShop, 60.0f);
				doorShop = false;
			}
			else if (doorTemple)
			{
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneTemple, 60.0f);
				doorTemple = false;
			}
			else if (doorFlor1)
			{
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneFloor1, 60.0f);
				doorFlor1 = false;
			}
			else if (enterCombat)
			{
				app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneCombat, 60.0f);
				enterCombat = false;
			}
		}
	}
	//printf("\r cameraX: %d cameraY: %d positionX: %d positionY %d", app->render->camera.x, app->render->camera.y, position.x, position.y);
	
	return true;
}

void Player::LeftMovement()
{
	isFacingRight = false;
	isWalking = true;	
	vel.x = -speed * 1.5f * dt;
}

void Player::RightMovement()
{
	isFacingRight = true;
	isWalking = true;
	vel.x = speed * 1.5f * dt;
}

void Player::UpMovement()
{
	vel.y = -speed * 1 * dt;
}

void Player::DownMovement()
{
	vel.y = speed * 1 * dt;
}
void Player::WalkingSound()
{
	if (isWalking)// si el bool isWalking es true  y is walking sound playing es false se activa el sonido y se cambia el bool a true
	{
		if (!walkingSoundPlaying)
		{
			app->audio->PlayFx(stepsFx, -1);
			walkingSoundPlaying = true;
		}
		
	
	}
	else if(walkingSoundPlaying) // cuando se cambia el bool a true accede al segundo else que pausa el sonido y cambia el bool a false
	{
		app->audio->PauseFx(stepsFx);
		walkingSoundPlaying = false;
	}

}
void Player::Jump()
{
	vel.y = -speed * 1.5 * dt;
	isjumping = true;
}

void Player::Respaw() {
	app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneVillage, 60.0f);
	health = 100;
	isDead = false;
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
	if (id == 1)
	{
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
	case ColliderType::DOOR_ALDEA:
		doorAldea = true;
		break;
	case ColliderType::DOOR_SHOP:
		doorShop = true;
		break;
	case ColliderType::DOOR_OASIS:
		doorOasis = true;
		break;
	case ColliderType::DOOR_TEMPLE:
		doorTemple = true;
		break;
	case ColliderType::DOOR_FLOOR_1:
		doorFlor1 = true;
		break;
	case ColliderType::TRAP:
		isDead = true;
		break;
	case ColliderType::COMBAT:
		enterCombat = true;
		break;
	case ColliderType::STAIRS:
		isClimbing = true;
		break;
	}

}
void Player::OnExitCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::DOOR_ALDEA:
		doorAldea = false;
		break;
	case ColliderType::DOOR_SHOP:
		doorShop = false;
		break;
	case ColliderType::DOOR_OASIS:
		doorOasis = false;
		break;
	case ColliderType::DOOR_TEMPLE:
		doorTemple = false;
		break;
	case ColliderType::DOOR_FLOOR_1:
		doorFlor1 = false;
		break;
	case ColliderType::COMBAT:
		enterCombat = false;
		break;
	case ColliderType::STAIRS:
		isClimbing = false;
		break;
	}

}

// Loads the animations
void Player::LoadAnimations()
{
	idleAnim.LoadAnimations("idleAnim", "player");
}

void Player::CreateBody()
{
	pbody = app->physics->CreateCircle(position.x + 50, position.y, 22, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	playerPbody = app->physics->CreateRectangleSensor(position.x + 50, position.y, 24, 60, bodyType::DYNAMIC);
	playerPbody->ctype = ColliderType::PLAYER_BODY;

	initialTransform = pbody->body->GetTransform();
}

void Player::DestroyBody()
{
	app->physics->world->DestroyBody(pbody->body);
	app->physics->world->DestroyBody(playerPbody->body);
}
