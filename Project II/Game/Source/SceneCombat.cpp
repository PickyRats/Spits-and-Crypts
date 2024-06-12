#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneCombat.h"
#include "SceneFloor1.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneCombat::SceneCombat(bool enabled) : Module(enabled)
{
	name.Create("sceneCombat");
}

// Destructor
SceneCombat::~SceneCombat()
{}

pugi::xml_node configNodeCombat;

// Called before render is available
bool SceneCombat::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	int i = 0;
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy[i]->parameters = itemNode;
		i++;
	}

	configNodeCombat = config;

	currentCombat = 0;
	return ret;
}

// Called before the first frame
bool SceneCombat::Start()
{
	if (currentCombat == 0)
	{
		if (configNodeCombat.child("map1")) {
			//Get the map name from the config file and assigns the value in the module
			app->map->mapName = configNodeCombat.child("map1").attribute("name").as_string();
			app->map->path = configNodeCombat.child("map1").attribute("path").as_string();
		}
	}
	else if (currentCombat == 1)
	{
		if (configNodeCombat.child("map2")) {
			//Get the map name from the config file and assigns the value in the module
			app->map->mapName = configNodeCombat.child("map2").attribute("name").as_string();
			app->map->path = configNodeCombat.child("map2").attribute("path").as_string();
		}
	}
	else if (currentCombat == 2)
	{
		if (configNodeCombat.child("map3")) {
			//Get the map name from the config file and assigns the value in the module
			app->map->mapName = configNodeCombat.child("map3").attribute("name").as_string();
			app->map->path = configNodeCombat.child("map3").attribute("path").as_string();
		}
	}

	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();

	app->map->player->isCombat = true;
	app->map->player2->isCombat = true;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	tileTexture = app->tex->Load("Assets/Textures/tile.png");
	tileEnemyTexture = app->tex->Load("Assets/Textures/tile_enemy.png");
	selectedTileTexture = app->tex->Load("Assets/Textures/selected_tile.png");
	cursorTexture = app->tex->Load("Assets/Textures/selection_cursor.png");

	//carga assets
	if (currentCombat == 0)
	{
		//floor1background = app->tex->Load("Assets/Textures/Screens/floor1background.png");

		app->map->player->DestroyBody();
		app->map->player->position = { 64, 576 };

		tilePosition = { 64, 576 };

		for (int i = 0; i < 2; i++)
		{
			enemy[i]->isActive = true;
			enemies[i] = enemy[i];
			enemies[i]->SetCombatAnimation(0);
		}

		players[0] = app->map->player;
		players[0]->SetCombatAnimation(0);
		players[1] = app->map->player2;
		players[1]->SetCombatAnimation(0);
		app->map->player2->isVisible = true;
		app->map->player2->position = { 0, 576 };
	}
	else if (currentCombat == 1)
	{
		//floor1background = app->tex->Load("Assets/Textures/Screens/floor1background.png");

		app->map->player->DestroyBody();
		app->map->player->position = { 64, 576 };

		tilePosition = { 64, 576 };

		for (int i = 0; i < 2; i++)
		{
			enemy[i]->isActive = true;
			enemy[i]->health = 100;
			enemy[i]->isDead = false;
			enemies[i] = enemy[i];
			enemies[i]->SetCombatAnimation(0);
		}

		enemies[0]->position = { 1024, 192 };
		enemies[1]->position = { 448, 192 };
		players[0] = app->map->player;
		players[0]->SetCombatAnimation(0);
		players[1] = app->map->player2;
		players[1]->SetCombatAnimation(0);
		app->map->player2->isVisible = true;
		app->map->player2->position = { 0, 576 };
	}
	else if (currentCombat == 2)
	{
		app->map->player->DestroyBody();
		app->map->player->position = { 64, 576 };

		tilePosition = { 64, 576 };

		enemy[2]->isActive = true;
		enemies[2] = enemy[2];
		enemies[2]->SetCombatAnimation(0);

		players[0] = app->map->player;
		players[0]->SetCombatAnimation(0);
	}

	app->map->pathfinding->CreatePath(app->map->WorldToMap(app->map->player->position.x, app->map->player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));

	currentEntity = players[currentPlayerIndex];

	victory = app->audio->LoadFx("Assets/Audio/Fx/victoryFx.wav");
	app->audio->PlayMusic(configNodeCombat.child("music").child("CombatMusic").attribute("path").as_string());

	atack = app->audio->LoadFx("Assets/Audio/Fx/combate/espada_Fx.wav");
	move_Tile = app->audio->LoadFx("Assets/Audio/Fx/combate/tile-Selection_Fx.wav");
	pass_Turn = app->audio->LoadFx("Assets/Audio/Fx/combate/pasar-turno_Fx.wav");
	victory = app->audio->LoadFx("Assets/Audio/Fx/combate/victory_Fx.wav");

	return true;
}

// Called each loop iteration
bool SceneCombat::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneCombat::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	//printf("\r Player 1 life: %d Player 2 life: %d Enemy 1 life: %d Enemy 2 life: %d", players[0]->health, players[1]->health, enemies[0]->health, enemies[1]->health);
	//app->render->DrawTexture(floor1background, 0, 0, NULL, SDL_FLIP_NONE, 1);

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(0, 0);

	ClampCamera();

	//if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	//{

	//	playerCanAttack = false;
	//	ChangeTurn();
	//	app->audio->PlayFx(pass_Turn);

	//}

	VerifyAnimation();

	if (isPlayerTurn)
	{
		PlayerTurn();

	}
	else
	{
		maxTiles = 50;

		EnemyTurn();
	}

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();


	if (isPlayerTurn && isMoving) MovePlayer(players[currentPlayerIndex]);
	else if (!isPlayerTurn && isMoving) MovePlayer(enemies[currentEnemyIndex]);

	// end combat
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || (enemies[0]->isDead && enemies[1]->isDead))
	{
		SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
		EndCombat();
	}

	if (combatCompleted && app->hud->mission31Active)
	{
		app->hud->mission32Active = true;
		app->hud->mission31Active = false;
	}

	return true;
}

void SceneCombat::VerifyAnimation()
{
	if (isPlayerAttacking)
	{
		if (currentEntity->AnimationFinished())
		{
			isPlayerAttacking = false;
			playerCanAttack = false;
			ChangeTurn();
		}
	}

	if (isEnemyAttacking)
	{
		if (currentEntity->AnimationFinished())
		{
			isEnemyAttacking = false;
			enemyCanAttack = false;
			if (enemies[currentEnemyIndex]->useProjectile) enemies[currentEnemyIndex]->arrow = false;
			ChangeTurn();
		}
		if (enemies[currentEnemyIndex]->useProjectile)
		{
			currentEntity->ShotArrow();
		}
	}

	if (isPlayerHitted)
	{
		if (players[nearestPlayer]->AnimationFinished())
		{
			isPlayerHitted = false;
			players[nearestPlayer]->SetCombatAnimation(0);
		}
	}

	if (isEnemyHitted)
	{
		if (enemies[nearestPlayer]->AnimationFinished())
		{
			isEnemyHitted = false;
			enemies[nearestPlayer]->SetCombatAnimation(0);
		}
	}
}

void SceneCombat::PlayerTurn()
{
	GamePad& pad = app->input->pads[0];
	static bool autoSelect = true;

	if (!players[currentPlayerIndex]->isDead)
	{
		if (!playerCanAttack)
		{
			maxTiles = 12;
			UpdatePath();

			SelectTiles();

			if ((app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN ||(pad.a==KEY_DOWN && !wasAPressed)) && !isMoving)
			{
				if (tilesCount <= players[currentPlayerIndex]->currentPoints)
					players[currentPlayerIndex]->currentPoints -= (tilesCount - 1);
				else
					players[currentPlayerIndex]->currentPoints = 0;

				if (tilesCount > 1)
					MovePlayer(players[currentPlayerIndex]);
				else
					playerCanAttack = true;
				wasAPressed = true;
			}
			else if (pad.a != KEY_DOWN)
			{
				wasAPressed = false;
			}
		}
		else
		{
			if (autoSelect)
			{
				float minDistance = FLT_MAX;
				int closestEnemyIndex = -1;
				for (int i = 0; i < 2; i++)
				{
					if (!enemies[i]->isDead)
					{
						float distance = CalculateDistance(players[currentPlayerIndex]->position, enemies[i]->position);
						if (distance < minDistance)
						{
							minDistance = distance;
							closestEnemyIndex = i;
						}
					}
				}

				if (closestEnemyIndex != -1)
				{
					enemyAttackIndex = closestEnemyIndex;
				}
				autoSelect = false;
			}

			app->hud->DrawTile(selectedTileTexture, enemies[enemyAttackIndex]->position);

			if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || (pad.a == KEY_DOWN && !wasAPressed))
			{
				maxTiles = 50;
				int enemyDistance = app->map->pathfinding->CreatePath(
					app->map->WorldToMap(currentEntity->position.x, currentEntity->position.y),
					app->map->WorldToMap(enemies[enemyAttackIndex]->position.x, enemies[enemyAttackIndex]->position.y)
				);

				if (enemyDistance <= currentEntity->attackRange)
				{
					if (!useAbility)
					{
						currentEntity->SetCombatAnimation(3);
						enemies[enemyAttackIndex]->health -= currentEntity->attackDamage;
					}
					else
					{
						currentEntity->SetCombatAnimation(4);
						enemies[enemyAttackIndex]->health -= currentEntity->attackDamage + currentEntity->abilityDamage;
					}
					currentEntity->currentPoints = 0;
					enemies[enemyAttackIndex]->SetCombatAnimation(5);
					isPlayerAttacking = true;
					isEnemyHitted = true;
				}
				else
				{
					playerCanAttack = false;
					ChangeTurn();
				}
				wasAPressed = true;
			}
			else if (pad.a != KEY_DOWN)
			{
				wasAPressed = false;
			}

			
				
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN||(pad.left==KEY_DOWN && !wasLeftPressed)) && enemyAttackIndex > 0 && !enemies[enemyAttackIndex - 1]->isDead)
			{
				wasLeftPressed = true;
				enemyAttackIndex--;
			}
			else if (pad.left != KEY_DOWN)
			{
				wasLeftPressed = false;
			}

			if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || (pad.right == KEY_DOWN && !wasRightPressed)) && enemyAttackIndex < 1 && !enemies[enemyAttackIndex + 1]->isDead)
			{
				wasRightPressed = true;

				enemyAttackIndex++;
			}
			else if (pad.right != KEY_DOWN)
			{
				wasRightPressed = false;
			}
		
			if (currentEntity->currentPoints >= 2 && (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN||(pad.x == KEY_DOWN && !wasXPressed)))
			{
				useAbility = !useAbility;
			}
			else if (pad.x != KEY_DOWN)
			{
				wasXPressed = false;
			}
		}
	}
	else if (players[currentPlayerIndex + 1] != nullptr && !players[currentPlayerIndex + 1]->isDead)
		currentPlayerIndex++;
	else if (!players[0]->isDead)
		currentPlayerIndex = 0;
	else
		EndCombat();
}

void SceneCombat::EnemyTurn()
{
	GamePad& pad = app->input->pads[0];
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		// Draw the path
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->hud->DrawTile(tileEnemyTexture, pos);

		// Set the direction of the tiles
		if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
		if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
		if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
		if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || startEnemyTurn)
	{
		if (!enemies[currentEnemyIndex]->isDead)
		{
			startEnemyTurn = false;
			tilesCount = path->Count();
			app->map->pathfinding->ClearLastPath();
			int playerDistance[2];
			for (int i = 0; i < 2; i++)
			{
				if (!players[i]->isDead)
				{
					playerDistance[i] = app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(players[i]->position.x, players[i]->position.y));
				}
				else playerDistance[i] = 1000;
			}
			if (playerDistance[0] < playerDistance[1]) nearestPlayer = 0;
			else nearestPlayer = 1;
			int destinationTiles = app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(players[nearestPlayer]->position.x, players[nearestPlayer]->position.y));
			int movementTiles = destinationTiles - enemies[currentEnemyIndex]->attackRange - 1;

			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				// Draw the path
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->hud->DrawTile(tileEnemyTexture, pos);

				// Set the direction of the tiles
				if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
				if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
				if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
				if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
			}

			tilesCount = path->Count();

			// Add movement tiles if the enemy can't reach the player
			for (int i = enemies[currentEnemyIndex]->attackRange + 1; i > 0; --i)
			{
				if (tiles[tilesCount - i].position.y != players[currentPlayerIndex]->position.y) movementTiles++;
			}

			if (enemies[currentEnemyIndex]->attackRange >= destinationTiles - 1) EnemyAttack();
			else if (enemies[currentEnemyIndex]->currentPoints > 0 && movementTiles <= enemies[currentEnemyIndex]->currentPoints)
			{
				for (int i = enemies[currentEnemyIndex]->currentPoints; i < tilesCount; ++i)
				{
					tiles[i + 1] = { iPoint(0,0), 0 };
				}
				tilesCount = movementTiles;
				enemyCanAttack = true;
				MovePlayer(enemies[currentEnemyIndex]);
				//printf("Enemy is moving %d tiles parcial\n", movementTiles);
				enemies[currentEnemyIndex]->currentPoints -= movementTiles;
			}
			else if (enemies[currentEnemyIndex]->currentPoints > 0)
			{
				for (int i = enemies[currentEnemyIndex]->currentPoints; i < tilesCount; ++i)
				{
					tiles[i + 1] = { iPoint(0,0), 0 };
				}
				tilesCount = enemies[currentEnemyIndex]->currentPoints;
				MovePlayer(enemies[currentEnemyIndex]);
				//printf("Enemy is moving %d tiles total\n", enemies[currentEnemyIndex]->currentPoints);
				enemies[currentEnemyIndex]->currentPoints = 0;
			}
			//else printf("No points\n");
		}
		else if (enemies[currentEnemyIndex + 1] != nullptr && !enemies[currentEnemyIndex + 1]->isDead) currentEnemyIndex++;
		else if (!enemies[0]->isDead) currentEnemyIndex = 0;
		else EndCombat();
	}
	
}

float SceneCombat::CalculateDistance(iPoint pos1, iPoint pos2)
{
	int dx = pos1.x - pos2.x;
	int dy = pos1.y - pos2.y;
	return sqrt(dx * dx + dy * dy);
}


// Called each loop iteration
bool SceneCombat::PostUpdate()
{
	GamePad& pad = app->input->pads[0];
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN|| (pad.start == KEY_DOWN && !wasStartPressed)) {
		pause = !pause;
		app->hud->onSettings = false;
		if (!pause)
		{
			Mix_VolumeMusic(app->sceneMenu->percentageMusic);
		};
		wasStartPressed = true;
	}
	else if (pad.start != KEY_DOWN)
	{
		wasStartPressed = false;
	}
	return ret;
}

// Called before quitting
bool SceneCombat::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	app->tex->UnLoad(floor1background);
	app->tex->UnLoad(tileTexture);
	app->tex->UnLoad(tileEnemyTexture);
	app->tex->UnLoad(selectedTileTexture);
	app->tex->UnLoad(cursorTexture);

	return true;
}

void SceneCombat::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneCombat::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;

}

bool SceneCombat::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneCombat::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneCombat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}

void SceneCombat::MovePlayer(Entity* entity)
{
	iPoint* currentPosition = &entity->position;

	// Set the destination position
	if (!isMoving)
	{
		if (tiles[currentTile].direction == 1)
		{
			entity->SetCombatAnimation(1);
			entity->isFacingRight = true;
			destinationPosition = { currentPosition->x + 64, currentPosition->y };
			movingDirection = 1;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 2)
		{
			entity->SetCombatAnimation(1);
			entity->isFacingRight = false;
			destinationPosition = { currentPosition->x - 64, currentPosition->y };
			movingDirection = 2;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 3)
		{
			entity->SetCombatAnimation(2);
			entity->isFacingRight = true;
			destinationPosition = { currentPosition->x, currentPosition->y - 64 - 64 - 64 };
			movingDirection = 3;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 4)
		{
			entity->SetCombatAnimation(2);
			entity->isFacingRight = true;
			destinationPosition = { currentPosition->x, currentPosition->y + 64 + 64 + 64 };
			movingDirection = 4;
			isMoving = true;
		}
	}

	// Check if the player has reached the destination
	if (currentTile <= tilesCount)
	{
		if (currentPosition->x == destinationPosition.x && currentPosition->y == destinationPosition.y)
		{
			movingDirection = 0;
			currentTile++;
			isMoving = false;
			MovePlayer(entity);
		}
	}
	else // Reset the path
	{
		isMoving = false;
		entity->SetCombatAnimation(0);
		if (!isPlayerTurn) ChangeTurn();
		else if (currentEntity->currentPoints > 0) playerCanAttack = true;
		else ChangeTurn();
	}

	// Move the player
	if (movingDirection == 1) currentPosition->x += 2;
	if (movingDirection == 2) currentPosition->x -= 2;
	if (movingDirection == 3) currentPosition->y -= 2;
	if (movingDirection == 4) currentPosition->y += 2;
}

void SceneCombat::UpdatePath()
{
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		// Draw the path
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if (i == path->Count() - 1) app->hud->DrawTile(selectedTileTexture, pos);
		else if (!isMoving || currentTile < i + 2) app->hud->DrawTile(tileTexture, pos);

		// Set the direction of the tiles
		if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
		if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
		if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
		if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
	}

	tilesCount = path->Count();

	//Draw the cursor
	app->hud->DrawTile(cursorTexture, tilePosition);
}

void SceneCombat::SelectTiles()
{
	GamePad& pad = app->input->pads[0];
	maxTiles = players[0]->currentPoints;
	Entity* player = players[currentPlayerIndex];
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || (pad.right == KEY_DOWN && !wasRightPressed)) && !isMoving)
	{
		app->audio->PlayFx(move_Tile);
		tilePosition.x += 64;
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
		wasRightPressed = true;
	}
	else if (pad.right != KEY_DOWN)
	{
		wasRightPressed = false;
	}

	if ((app->input->GetKey(SDL_SCANCODE_A)== KEY_DOWN || (pad.left == KEY_DOWN && !wasLeftPressed)) && !isMoving)
	{
		app->audio->PlayFx(move_Tile);
		tilePosition.x -= 64;
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
		wasLeftPressed = true;
	}
	else if (pad.left != KEY_DOWN)
	{
		wasLeftPressed = false;
	}
	
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || (pad.up == KEY_DOWN && !wasUpPressed)) && !isMoving)
	{
		tilePosition.y -= (64 * 3);
		if (!IsTileOccupied())
		{
			app->audio->PlayFx(move_Tile);
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));

		}
		wasUpPressed = true;
	}
	else if (pad.up != KEY_DOWN)
	{
		wasUpPressed = false;
	}

	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || (pad.down == KEY_DOWN && !wasDownPressed)) && !isMoving)
	{
		tilePosition.y += (64 * 3);
		if (!IsTileOccupied())
		{
			app->audio->PlayFx(move_Tile);
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
		wasDownPressed = true;
	}
	else if (pad.down != KEY_DOWN)
	{
		wasDownPressed = false;
	}
}

void SceneCombat::ResetTilesArray(int max)
{
	for (int i = 1; i < max; i++)
	{
		tiles[max - i] = { iPoint(0,0), 0 };
	}
}

void SceneCombat::ChangeTurn()
{
	app->audio->PlayFx(pass_Turn);
	currentEntity->currentPoints = currentEntity->totalPoints;
	useAbility = false;
	currentEntity->SetCombatAnimation(0);

	if (isPlayerTurn) ResetPlayerTurn();
	else
	{
		if (enemyCanAttack) EnemyAttack();
		else ResetEnemyTurn();
	}
}

void SceneCombat::EnemyAttack()
{
	app->audio->PlayFx(atack);
	enemies[currentEnemyIndex]->SetCombatAnimation(3);
	players[nearestPlayer]->health -= enemies[nearestPlayer]->attackDamage;
	players[nearestPlayer]->SetCombatAnimation(5);
	isPlayerHitted = true;
	//printf("Enemy is attacking player life: %d \n", players[nearestPlayer]->health);
	movingDirection = 0;
	isEnemyAttacking = true;
}

void SceneCombat::ResetPlayerTurn()
{
	SDL_JoystickSetLED(app->input->joy, 255, 0, 0);
	tiles[0] = { enemies[currentEnemyIndex]->position, 0 };
	ResetTilesArray(currentTile);
	tilesCount = 0;
	movingDirection = 0;
	currentTile = 1;
	tilePosition = tiles[0].position;
	app->map->pathfinding->ClearLastPath();
	app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	if (players[currentPlayerIndex + 1] != nullptr && !players[currentPlayerIndex + 1]->isDead) currentPlayerIndex++;
	else if (!players[0]->isDead) currentPlayerIndex = 0;
	else EndCombat();
	currentEntity = enemies[currentEnemyIndex];
	startEnemyTurn = true;
	isPlayerTurn = false;
}

void SceneCombat::ResetEnemyTurn()
{
	SDL_JoystickSetLED(app->input->joy, 0, 0, 255);
	tiles[0] = { players[currentPlayerIndex]->position, 0 };
	ResetTilesArray(currentTile);
	tilesCount = 0;
	movingDirection = 0;
	currentTile = 1;
	tilePosition = tiles[0].position;
	app->map->pathfinding->ClearLastPath();
	app->map->pathfinding->CreatePath(app->map->WorldToMap(players[currentPlayerIndex]->position.x, players[currentPlayerIndex]->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	if (enemies[currentEnemyIndex + 1] != nullptr && !enemies[currentEnemyIndex + 1]->isDead) currentEnemyIndex++;
	else currentEnemyIndex = 0;
	currentEntity = players[currentPlayerIndex];
	isPlayerTurn = true;
}

bool SceneCombat::IsTileOccupied()
{
	for (int i = 0; i < 2; i++)
	{
		if (players[i]->position.x == tilePosition.x && players[i]->position.y == tilePosition.y && currentEntity != players[i]) return true;
	}
	for (int i = 0; i < 2; i++)
	{
		if (enemies[i]->position.x == tilePosition.x && enemies[i]->position.y == tilePosition.y) return true;
	}
	return false;
}

void SceneCombat::EndCombat()
{
	if (currentCombat == 0)
	{
		app->audio->PlayFx(victory);
		app->map->player->isCombat = false;
		app->map->player2->isCombat = false;
		app->map->player2->isVisible = false;
		app->sceneFloor1->playerStartPosition = { 67 * 64, 28 * 64 };
		app->map->player->CreateBody();
		app->sceneFloor1->levelWidth = 110 * 64;
		app->fade->Fade((Module*)app->sceneCombat, (Module*)app->sceneFloor1, 60.0f);
		combatCompleted = true;
		currentCombat++;
	}
}