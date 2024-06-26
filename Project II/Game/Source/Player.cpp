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
#include "SceneSelection.h"
#include "CutscenePlayer.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath1 = parameters.attribute("texturepath1").as_string();
	texturePath2 = parameters.attribute("texturepath2").as_string();
	texturePath3 = parameters.attribute("texturepath3").as_string();
	texturePath4 = parameters.attribute("texturepath4").as_string();
	speed = parameters.attribute("speed").as_float();
	id = parameters.attribute("id").as_int();

	return true;
}

bool Player::Start() {

	//initilize texture
	if (id == 1)
	{
		if (app->sceneSelection->currentSelection == 0) texture = app->tex->Load(texturePath1);
		else if (app->sceneSelection->currentSelection == 1) texture = app->tex->Load(texturePath2);
		else if (app->sceneSelection->currentSelection == 2) texture = app->tex->Load(texturePath3);
		else if (app->sceneSelection->currentSelection == 3) texture = app->tex->Load(texturePath4);

		texture1 = app->tex->Load("Assets/Textures/MC_Sprites_Esclavo.png");
	}
	else
	{
		texture = app->tex->Load(texturePath1);
	}

	LoadAnimations();

	currentAnim = &walkAnim;

	stepsFx = app->audio->LoadFx("Assets/Audio/Fx/Footsteps_Fx.wav");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/jump_FX.wav");
	climbFx = app->audio->LoadFx("Assets/Audio/Fx/escaleras_Fx.wav");
	doorFx = app->audio->LoadFx("Assets/Audio/Fx/trampilla.wav");


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
	GamePad& pad = app->input->pads[0];
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

			vel = pbody->body->GetLinearVelocity();

			if (!godMode)
			{
				//Funcion para hacer sonidos
				SoundManager();

				//player movement
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !inicio)
				{
					if (!isClimbing && !isEnteringDoor)
					{
						LeftMovement();
					}
				}
				if (pad.l_x <= -0.2)
				{
					if (!isClimbing && !isEnteringDoor)
					{
						LeftMovement();
					}
				}

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					if (!isClimbing && !isEnteringDoor)
					{
						RightMovement();
					}
				}
				if (pad.l_x >= 0.2)
				{
					if (!isClimbing && !isEnteringDoor)
					{
						RightMovement();
					}
				}

				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
					&& (pad.l_x < 0.2 && pad.l_x > -0.2)&& (pad.l_y < 0.2 && pad.l_y > -0.2))
				{
					if (!isJumping && !isClimbing && !isEnteringDoor)
					{
						if (!inicio) currentAnim = &idleAnim;
						else currentAnim = &idleRockAnim;
					}
					isWalking = false;
					vel.x = 0;
				}
				
				//Climbing
				if (canClimb && !isJumping && vel.y<=0)
				{
					
					isWalking = false;
					if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !inicio)
					{
						UpMovement();
						isClimbing = true;
						pbody->body->SetGravityScale(0.0f);
						if (collisionActivated)
						{
							for (int i = 0; i < app->map->tempColliders.Count(); i++)
							{
								PhysBody* temp = app->map->tempColliders.At(i)->data;
								temp->body->SetActive(false);
							}
							collisionActivated = false;
						}
						wasUpPressed = true;
					}
					if (pad.l_y <= -0.2)
					{
						UpMovement();
						isClimbing = true;
						pbody->body->SetGravityScale(0.0f);
						if (collisionActivated)
						{
							for (int i = 0; i < app->map->tempColliders.Count(); i++)
							{
								PhysBody* temp = app->map->tempColliders.At(i)->data;
								temp->body->SetActive(false);
							}
							collisionActivated = false;
						}
						wasUpPressed = true;
					}

					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !inicio)
					{
						DownMovement();
						isClimbing = true;
						pbody->body->SetGravityScale(0.0f);
						if (collisionActivated)
						{
							for (int i = 0; i < app->map->tempColliders.Count(); i++)
							{
								PhysBody* temp = app->map->tempColliders.At(i)->data;
								temp->body->SetActive(false);
							}
							collisionActivated = false;
						}
						
					}
					if (pad.l_y >= 0.2)
					{
						DownMovement();
						isClimbing = true;
						pbody->body->SetGravityScale(0.0f);
						if (collisionActivated)
						{
							for (int i = 0; i < app->map->tempColliders.Count(); i++)
							{
								PhysBody* temp = app->map->tempColliders.At(i)->data;
								temp->body->SetActive(false);
							}
							collisionActivated = false;
						}

					}
					
				
					if (app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE 
						&& (pad.l_y < 0.2 && pad.l_y > -0.2))
					{
						vel.y = 0;
					}
					
					
				}
				//Jump
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN|| pad.a && !inicio)
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

				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !inicio)
				{
					vel.y = -speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !inicio)
				{
					vel.y = speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !inicio)
				{
					vel.x = -speed * 2 * dt;
				}
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !inicio)
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
		//printf("\r playerX: %d playerY: %d", position.x, position.y);////////////
		//printf("\r tilecount %d", platformCollisionCount);////////////
		currentAnim->Update();
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || pad.x==KEY_DOWN && !inicio)
		{
			EnterDoor();
		}
	}

	//printf("\r cameraX: %d cameraY: %d positionX: %d positionY %d", app->render->camera.x, app->render->camera.y, position.x, position.y);
	return true;
}

void Player::EnterDoor()
{
	if (doorOasis)
	{
		EnteringDoor();
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneOasisFaraon, 60.0f);
		doorOasis = false;
	}
	else if (doorShop)
	{
		EnteringDoor();
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneShop, 60.0f);
		doorShop = false;
	}
	else if (doorTemple)
	{
		EnteringDoor();
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneTemple, 60.0f);
		doorTemple = false;
		if (app->hud->mission20Active)
		{
			app->hud->mission20Active = false;
			app->hud->mission21Active = true;
		}
	}
	else if (doorFloor1)
	{
		//EnteringDoor();
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneFloor1, 60.0f);
		doorFloor1 = false;
		if (app->hud->mission30Active)
		{
			app->hud->mission31Active = true;
			app->hud->mission30Active = false;
		}
		
	}
	else if (enterCombat1)
	{
		app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneCombat, 60.0f);
		enterCombat1 = false;
	}
	else if (enterCombat2)
	{
		if (app->hud->mission3Complete) app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneCombat, 60.0f);
		enterCombat2 = false;
	}
	else if (enterCombat3)
	{
		app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneCombat, 60.0f);
		enterCombat3 = false;
	}
	else if (doorChoza)
	{
		EnteringDoor();
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneChoza, 60.0f);
		if (app->hud->mission10Active)
		{
			app->hud->mission11Active = true;
			app->hud->mission10Active = false;
			app->sceneVillage->piedra->body->SetTransform({ 0,0 }, 0);
		}
		doorChoza = false;
	}
	else if (enterPuzle3)
	{
		app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneLight, 60.0f);
		enterCombat3 = false;
	}
	
}

void Player::EnteringDoor()
{
	isEnteringDoor = true;
	currentAnim = &turnAnim;
	currentAnim->ResetLoopCount();
	currentAnim->Reset();
	app->audio->PlayFx(doorFx);
}

void Player::LeftMovement()
{
	if (!isJumping) currentAnim = &walkAnim;

	isFacingRight = false;
	if(!canClimb) isWalking = true;	
	
	vel.x = -speed * 1.5f * dt;
}

void Player::RightMovement()
{
	if (!isJumping && !inicio) currentAnim = &walkAnim;
	else if (inicio) currentAnim = &rockAnim;

	isFacingRight = true;
	if (!canClimb) isWalking = true;

	vel.x = speed * 1.5f * dt;
}

void Player::UpMovement()
{
	currentAnim = &climbAnim;
	if (currentAnim->HasFinished())
	{
		currentAnim->ResetLoopCount();
		currentAnim->Reset();
	}
	vel.y = -speed * 1 * dt;
}

void Player::DownMovement()
{
	currentAnim = &climbAnim;
	if (currentAnim->HasFinished())
	{
		currentAnim->ResetLoopCount();
		currentAnim->Reset();
	}
	vel.y = speed * 1 * dt;
}
void Player::SoundManager()
{
	if (playJumpSound)
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

	if (isWalking && !isJumping)// si el bool isWalking es true  y is walking sound playing es false se activa el sonido y se cambia el bool a true
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
	if (!isJumping && !inicio)
	{
		isJumping = true;
		playJumpSound = true;
		currentAnim = &jumpAnim;
		currentAnim->Reset();
		vel.y = -speed * 1.5 * dt;
	}
	
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
		if (id == 1)
		{
			if (!inicio)
			{
				if (!app->sceneCombat->active) app->render->DrawTexture(texture, position.x - 30, position.y - 86, &rect);
				else app->render->DrawTexture(texture, position.x - 55, position.y - 86, &rect);
			}
			else app->render->DrawTexture(texture1, position.x, position.y - 40, &rect);
			
		}
		else if (id == 2)
		{
			if (!app->sceneCombat->active) app->render->DrawTexture(texture, position.x - 58, position.y - 64, &rect);
			else app->render->DrawTexture(texture, position.x - 70, position.y - 64, &rect);
		}
	}
	else
	{
		if (id == 1)
		{
			if (!app->sceneCombat->active) app->render->DrawTexture(texture, position.x - 30, position.y - 86, &rect, SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x - 55, position.y - 86, &rect, SDL_FLIP_HORIZONTAL);
		}
		else if (id == 2)
		{
			if (!app->sceneCombat->active) app->render->DrawTexture(texture, position.x - 58, position.y - 64, &rect, SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x - 70, position.y - 64, &rect, SDL_FLIP_HORIZONTAL);
		}
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
			pbody->body->SetLinearVelocity({ 0, - 0.165f });
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
		platformCollisionCount++;
		isJumping = false;
		playJumpSound = false;
		isClimbing = false;
		if (pbody->body->GetGravityScale() != 1.0f)
		{
			pbody->body->SetGravityScale(1.0f);
		}
		break;
	case ColliderType::DOOR_ALDEA:
		doorAldea = true;
		if (app->sceneShop->active) {
			app->sceneVillage->spawnPosition = { 1164, 675 };
			app->fade->Fade((Module*)app->sceneShop, (Module*)app->sceneVillage, 60.0f);
		}
		else if (app->sceneOasisFaraon->active) {
			app->sceneVillage->spawnPosition = { 1700, 675 };
			app->fade->Fade((Module*)app->sceneOasisFaraon, (Module*)app->sceneVillage, 60.0f);
		}
		else if (app->sceneTemple->active) {
			app->sceneVillage->spawnPosition = { 3264, 675 };
			app->fade->Fade((Module*)app->sceneTemple, (Module*)app->sceneVillage, 60.0f);
		}
		else if (app->sceneFloor1->active) {
			app->sceneVillage->spawnPosition = { 5450, 675 };
			app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneVillage, 60.0f);
		}
		doorAldea = false;
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
		doorFloor1 = true;
		app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneFloor1, 60.0f);
		doorFloor1 = false;
		if (app->hud->mission30Active)
		{
			app->hud->mission31Active = true;
			app->hud->mission30Active = false;
		}

		if (!cutsceneStarted)
		{
			cutsceneStarted = true;
			isWalking = false;
			app->audio->PauseFx(stepsFx);
			app->cutscenePlayer->file = "Assets/Video/Cinematica_04.mp4";
			app->cutscenePlayer->Enable();
			app->audio->PlayMusic("Assets/Video/Cinematica_04.ogg", 0);
			app->cutscenePlayer->ConvertPixels(0, 1);
			app->cutscenePlayer->Disable();
			app->sceneFloor1->PlayMusic();
		}
		break;
	case ColliderType::TRAP:
		isDead = true;
		break;
	case ColliderType::COMBAT1:
		if (app->sceneCombat->currentCombat == 0) enterCombat1 = true;
		break;
	case ColliderType::COMBAT2:
		if (app->sceneCombat->currentCombat == 1) enterCombat2 = true;
		break;
	case ColliderType::COMBAT3:
		if (app->sceneCombat->currentCombat == 2) enterCombat3 = true;
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

	case ColliderType::TEMP_PLATFORM:
		platformCollisionCount++;
		isJumping = false;
		playJumpSound = false;
		if (pbody->body->GetGravityScale() != 1.0f)
		{
			pbody->body->SetGravityScale(1.0f);
		}
		break;
	case ColliderType::ROCK:
		app->audio->PlayFx(app->sceneVillage->rockfx, -1);
		break;
	case ColliderType::DOOR_PUZLE_2:
		if (app->sceneFloor1->puertas[2]->puzle1Completed) enterPuzle3 = true;
		break;
	}

}
void Player::OnExitCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		platformCollisionCount--;
		if (platformCollisionCount <= 0) {
			platformCollisionCount = 0; // Asegurarse de que el contador no sea negativo
			isJumping = true; // El jugador ha salido de todas las plataformas
		}
		break;
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
		doorFloor1 = false;
		break;
	case ColliderType::COMBAT1:
		enterCombat1 = false;
		break;
	case ColliderType::STAIRS:
		canClimb = false;
		isClimbing = false;
		pbody->body->SetGravityScale(6.0f);
		if (!collisionActivated)
		{
			for (int i = 0; i < app->map->tempColliders.Count(); i++)
			{
				PhysBody* temp = app->map->tempColliders.At(i)->data;
				temp->body->SetActive(true);
			}
			collisionActivated = true;

		}
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
	case ColliderType::ROCK:
		app->audio->UnloadFx(app->sceneVillage->rockfx);
		app->audio->LoadFx("Assets/Audio/Fx/RockFx.wav");
		break;
	}

}

// Loads the animations
void Player::LoadAnimations()
{
	if (id == 1) // Main character
	{
		idleAnim.LoadAnimations("idleAnim", "player");
		turnAnim.LoadAnimations("turnAnim", "player");
		talkAnim.LoadAnimations("talkAnim", "player");
		walkAnim.LoadAnimations("walkAnim", "player");
		climbAnim.LoadAnimations("climbAnim", "player");
		hitAnim.LoadAnimations("hitAnim", "player");
	
		jumpAnim.LoadAnimations("jumpAnim", "player");
		deathAnim.LoadAnimations("deathAnim", "player");
		idleBattleAnim.LoadAnimations("idleBattleAnim", "player");
	
		walkBattleAnim.LoadAnimations("walkBattleAnim", "player");

		idleRockAnim.LoadAnimations("idleRockAnim", "player");
		rockAnim.LoadAnimations("rockAnim", "player");

		if (app->sceneSelection->currentSelection == 0)
		{
			attackAnim.LoadAnimations("attackAnim", "player");
			abilityAnim.LoadAnimations("abilityAnim", "player");
		}
		else if (app->sceneSelection->currentSelection == 1)
		{
			attackAnim.LoadAnimations("attack1Anim", "player");
			abilityAnim.LoadAnimations("ability1Anim", "player");
		}
		else if (app->sceneSelection->currentSelection == 2)
		{
			attackAnim.LoadAnimations("attack2Anim", "player");
			abilityAnim.LoadAnimations("ability2Anim", "player");
		}
		else if (app->sceneSelection->currentSelection == 3)
		{
			attackAnim.LoadAnimations("attack3Anim", "player");
			abilityAnim.LoadAnimations("ability3Anim", "player");
		}
	}
	else if (id == 2) // Taila
	{
		idleAnim.LoadAnimations("idleAnim", "player2");
		turnAnim.LoadAnimations("turnAnim", "player2");
		talkAnim.LoadAnimations("talkAnim", "player2");
		walkAnim.LoadAnimations("walkAnim", "player2");
		climbAnim.LoadAnimations("climbAnim", "player2");
		hitAnim.LoadAnimations("hitAnim", "player2");
		jumpAnim.LoadAnimations("jumpAnim", "player2");
		deathAnim.LoadAnimations("deathAnim", "player2");
		idleBattleAnim.LoadAnimations("idleBattleAnim", "player2");
		walkBattleAnim.LoadAnimations("walkBattleAnim", "player2");
		attackAnim.LoadAnimations("attackAnim", "player2");
		abilityAnim.LoadAnimations("abilityAnim", "player2");
	}
	
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

void Player::SetCombatAnimation(int animationIndex)
{
	switch (animationIndex)
	{
	case 0:
		currentAnim = &idleBattleAnim;
		break;
	case 1:
		currentAnim = &walkBattleAnim;
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

bool Player::AnimationFinished()
{
	return currentAnim->HasFinished();
}
