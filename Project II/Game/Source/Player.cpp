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
#include "Puzzle.h"
#include "SceneLight.h"
#include "SceneCombat.h"
#include "Map.h"

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
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/jump_FX.wav");
	climbFx = app->audio->LoadFx("Assets/Audio/Fx/escaleras_Fx.wav");
	doorFx = app->audio->LoadFx("Assets/Audio/Fx/trampilla.wav");
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

	if (health <= 0 && !isDead && app->map->player2->health <= 0)
	{
		isDead = true;
		isCombat = false;
		app->map->player2->isVisible = false;
		app->map->player2->isCombat = false;
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
				SoundManager();

				//player movement
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					if (!isClimbing)
					{
						LeftMovement();
					}
				}

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					if (!isClimbing)
					{
						RightMovement();
					}
				}

				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
				{

					isWalking = false;
					vel.x = 0;
				}
				
				//Climbing
				if (canClimb && !isjumping && vel.y<=0)
				{
					isWalking = false;
					if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
					{
						UpMovement();
						isClimbing = true;
						pbody->body->SetGravityScale(0.0f);
					}
					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
					{
						DownMovement();
						/*isClimbing = true;*/
						pbody->body->SetGravityScale(0.0f);
					}
					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
					{
						vel.y = 0;
					}
					
				}
				//Jump
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
		printf("\r playerX: %d playerY: %d", position.x, position.y);////////////
		currentAnim->Update();
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			
			if (doorAldea) {
				app->audio->PlayFx(doorFx);
				if (app->sceneShop->active) {
					app->sceneVillage->spawnPosition = { 481, 675 };
					app->fade->Fade((Module*)app->sceneShop, (Module*)app->sceneVillage, 60.0f);
				}
				else if (app->sceneOasisFaraon->active) {
					app->sceneVillage->spawnPosition = { 1380, 675 };
					app->fade->Fade((Module*)app->sceneOasisFaraon, (Module*)app->sceneVillage, 60.0f);
				}
				else if (app->sceneTemple->active) {
					app->sceneVillage->spawnPosition = { 2269, 675 };
					app->fade->Fade((Module*)app->sceneTemple, (Module*)app->sceneVillage, 60.0f);
				}
				else if (app->sceneFloor1->active) {
					app->sceneVillage->spawnPosition = { 2787, 675 };
					app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneVillage, 60.0f);
				}
				doorAldea = false;
			}
			else if (doorOasis)
			{
				app->audio->PlayFx(doorFx);
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneOasisFaraon, 60.0f);
				doorOasis = false;
			}
			else if (doorShop)
			{
				app->audio->PlayFx(doorFx);
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneShop, 60.0f);
				doorShop = false;
			}
			else if (doorTemple)
			{
				app->audio->PlayFx(doorFx);
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneTemple, 60.0f);
				doorTemple = false;
			}
			else if (doorFlor1)
			{
				app->audio->PlayFx(doorFx);
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneFloor1, 60.0f);
				doorFlor1 = false;
			}
			else if (enterCombat)
			{
				app->audio->PlayFx(doorFx);
				app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneCombat, 60.0f);
				enterCombat = false;
			}
			else if (doorChoza)
			{
				app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneChoza, 60.0f);
				if (app->hud->mission10Active)
				{
					app->hud->mission11Active = true;
					app->hud->mission10Active = false;
					app->sceneVillage->piedra->body->SetTransform({ 0,0 }, 0);
				}
				doorChoza = false;
			}
		}
	}
	//printf("\r cameraX: %d cameraY: %d positionX: %d positionY %d", app->render->camera.x, app->render->camera.y, position.x, position.y);
	
	return true;
}

void Player::LeftMovement()
{
	isFacingRight = false;
	if(!canClimb) isWalking = true;	
	
	vel.x = -speed * 1.5f * dt;
}

void Player::RightMovement()
{
	isFacingRight = true;
	if (!canClimb) isWalking = true;
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
void Player::SoundManager()
{
	if (isjumping)
	{
		if (!jumpingSoundPlaying)
		{
			app->audio->PlayFx(jumpFx, 0);
			jumpingSoundPlaying = true;
		}
	}
	else if (jumpingSoundPlaying) // cuando se cambia el bool a true accede al segundo else que pausa el sonido y cambia el bool a false
	{
		app->audio->PauseFx(jumpFx);
		jumpingSoundPlaying = false;
	}

	if (isWalking && !isjumping)// si el bool isWalking es true  y is walking sound playing es false se activa el sonido y se cambia el bool a true
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

	if (canClimb)
	{
		if (!climbingSoundPlaying)
		{
			app->audio->PlayFx(climbFx, -1);
			climbingSoundPlaying = true;
		}
	}
	else if (climbingSoundPlaying) // cuando se cambia el bool a true accede al segundo else que pausa el sonido y cambia el bool a false
	{
		
		app->audio->PauseFx(climbFx);
		climbingSoundPlaying = false;
	}

}
void Player::Jump()
{
	vel.y = -speed * 1.5 * dt;
	isjumping = true;
}

void Player::Respaw() {
	if (app->sceneCombat->active)
	{
		app->fade->Fade((Module*)app->sceneCombat, (Module*)app->sceneEnding, 60.0f);
		app->map->player->health = 100;
		app->map->player2->health = 100;
		CreateBody();
	}
	else
	{
		app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneEnding, 60.0f);
	}

	isDead = false;
	health = 100;
	LOG("Respawn");
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
	app->audio->UnloadFx(stepsFx);
	app->audio->UnloadFx(climbFx);
	app->audio->UnloadFx(doorFx);

	return true;
}

// Handles the collision
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		isjumping = false;
		isClimbing = false;
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
		canClimb = true;
		isWalking = false;
		break;
	case ColliderType::PUZZLE:
		app->puzzle->canInteract = true;

		break;
	case ColliderType::LIGHT1:
		app->sceneLight->interactMirror = true;
		break;
	case ColliderType::LIGHT2:
		app->sceneLight->interactTrapdoor = true;
		break;
	case ColliderType::DOOR_CHOZA:
		doorChoza = true;
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
		canClimb = false;
		isClimbing = false;
		pbody->body->SetGravityScale(1.0f);
		break;
	case ColliderType::PUZZLE:
		app->puzzle->canInteract = false;
		app->puzzle->showUI = false;
		break;
	case ColliderType::LIGHT1:
		app->sceneLight->interactMirror = false;
		app->sceneLight->isInteractingMirror = false;
		break;
	case ColliderType::LIGHT2:
		app->sceneLight->interactTrapdoor = false;
		app->sceneLight->isInteractingTrapdoor = false;
		break;
	case ColliderType::DOOR_CHOZA:
		doorChoza = false;
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
