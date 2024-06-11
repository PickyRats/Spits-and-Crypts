#include "Puzzle2.h"
#include "App.h"
#include "Map"
#include "Player.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Puzzle.h"
#include "FadeToBlack.h"

Puzzle2::Puzzle2(bool enabled) : Module(enabled)
{
	name.Create("Puzzle2");

}

Puzzle2::~Puzzle2() {

}

bool Puzzle2::Awake() {

	return true;
}

bool Puzzle2::Start() {

	//initilize texture
	texture[0] = app->tex->Load("Assets/Textures/Puzzle/piece_big_2.png");
	texture[1] = app->tex->Load("Assets/Textures/Puzzle/piece_medium_2.png");
	texture[2] = app->tex->Load("Assets/Textures/Puzzle/piece_small_2.png");
	texture[3] = app->tex->Load("Assets/Textures/Puzzle/piece_big_1.png");
	texture[4] = app->tex->Load("Assets/Textures/Puzzle/piece_medium_1.png");
	texture[5] = app->tex->Load("Assets/Textures/Puzzle/piece_small_1.png");
	texture[6] = app->tex->Load("Assets/Textures/Puzzle/piece_big_selected.png");
	texture[7] = app->tex->Load("Assets/Textures/Puzzle/piece_medium_selected.png");
	texture[8] = app->tex->Load("Assets/Textures/Puzzle/piece_small_selected.png");
	rotateRockFx = app->audio->LoadFx("Assets/Audio/Fx/Rotar_Piezas.wav");
	placeRockFx = app->audio->LoadFx("Assets/Audio/Fx/Colocar_Piezas.wav");
	for (int i = 0; i < 3; i++)
	{
		piecePosition[i] = leftPosition[i];
	}
	
	return true;
}

bool Puzzle2::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	DrawPieces();
	PlaySounds();

	if (isPieceInserted[0] == 0 && isPieceInserted[1] == 0 && isPieceInserted[2] == 0
		&& rotation[0] == 90 && rotation[1] == 90 && rotation[2] == 90)
	{
		if(!puzzleCompleted) LOG("PUZZLE 2 COMPLETE");
		//app->sceneFloor1->levelWidth = 190 * 64;
		puzzleCompleted = true;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || (pad.l1 == KEY_DOWN && !wasL1Pressed) && (app->puzzle->isPuzzleCompleted=true))
	{
		for (int i = 0; i < 3; i++)
		{
			if (selection == i)
			{
				rotated = true;
				if (rotation[i + 3] >= 90) rotation[i + 3] -= 90;
				else rotation[i + 3] = 270;
				if (isPieceInserted[i] == 0)
				{
					if (rotation[i] >= 90) rotation[i] -= 90;
					else rotation[i] = 270;
				}
			}
		}
		wasL1Pressed = true;
	}
	else if (pad.l1 != KEY_DOWN)
	{
		wasL1Pressed = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || (pad.r1 == KEY_DOWN && !wasR1Pressed) && (app->puzzle->isPuzzleCompleted = true))
	{
		for (int i = 0; i < 3; i++)
		{
			if (selection == i)
			{
				rotated = true;
				if (rotation[i + 3] < 270) rotation[i + 3] += 90;

				else rotation[i + 3] = 0;
				if (isPieceInserted[i] == 0)
				{
					if (rotation[i] < 270) rotation[i] += 90;
					else rotation[i] = 0;
				}
			}
		}
		wasR1Pressed = true;
	}
	else if (pad.r1 != KEY_DOWN)
	{
		wasR1Pressed = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN || (pad.l2 == KEY_DOWN && !wasL2Pressed) && (app->puzzle->isPuzzleCompleted = true))
	{
		if (selection < 2) selection++;
		else selection = 0;
		wasL2Pressed = true;
	}
	else if (pad.l2 != KEY_DOWN)
	{
		wasL2Pressed = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN|| (pad.r2 == KEY_DOWN && !wasR2Pressed) && (app->puzzle->isPuzzleCompleted = true))
	{
		if (selection == 0)
		{
			placed = true;
			if ((isPieceInserted[0] == 1 && rotation[3] == 0) 
				|| (isPieceInserted[0] == 2 && rotation[3] == 180))
			{
				isPieceInserted[0] = 0;
				piecePosition[0] = centerPosition;
			}
			else if (isPieceInserted[0] == 0 && rotation[0] == 0)
			{
				isPieceInserted[0] = 1;
				piecePosition[0] = leftPosition[0];
			}
			else if (isPieceInserted[0] == 0 && rotation[0] == 180)
			{
				isPieceInserted[0] = 2;
				piecePosition[0] = rightPosition[0];
			}
		}
		else if (selection == 1)
		{
			placed = true;
			if (isPieceInserted[0] != 0)
			{
				if ((isPieceInserted[1] == 1 && rotation[4] == 0 && rotation[3] == 0 && isPieceInserted[0] != 1)
					|| (isPieceInserted[1] == 2 && rotation[4] == 180 && rotation[3] == 180 && isPieceInserted[0] != 2))
				{
					isPieceInserted[1] = 0;
					piecePosition[1] = centerPosition;
				}
				else if (isPieceInserted[1] == 0 && rotation[1] == 0 && rotation[3] == 0 && isPieceInserted[0] != 1)
				{
					isPieceInserted[1] = 1;
					piecePosition[1] = leftPosition[1];
				}
				else if (isPieceInserted[1] == 0 && rotation[1] == 180 && rotation[3] == 180 && isPieceInserted[0] != 2)
				{
					isPieceInserted[1] = 2;
					piecePosition[1] = rightPosition[1];
				}
			}
		}
		else if (selection == 2)
		{
			placed = true;
			if (isPieceInserted[0] != 0 && isPieceInserted[1] != 0)
			{
				if ((isPieceInserted[2] == 1 && rotation[5] == 0 && rotation[3] == 0 && rotation[4] == 0
					&& isPieceInserted[0] != 1 && isPieceInserted[1] != 1)
					|| (isPieceInserted[2] == 2 && rotation[5] == 180 && rotation[3] == 180 && rotation[4] == 180
					&& isPieceInserted[0] != 2 && isPieceInserted[1] != 2))
				{
					isPieceInserted[2] = 0;
					piecePosition[2] = centerPosition;
				}
				else if (isPieceInserted[2] == 0 && rotation[2] == 0 && rotation[3] == 0 && rotation[4] == 0)
				{
					isPieceInserted[2] = 1;
					piecePosition[2] = leftPosition[2];
				}
				else if (isPieceInserted[2] == 0 && rotation[2] == 180 && rotation[3] == 180 && rotation[4] == 180)
				{
					isPieceInserted[2] = 2;
					piecePosition[2] = rightPosition[2];
				}
			}
		}
		wasR2Pressed = true;
	}
	else if (pad.r2 != KEY_DOWN)
	{
		wasR2Pressed = false;
	}

	return true;
}

void Puzzle2::DrawPieces()
{
	
	for (int i = 3; i < 6; i++)
	{
		app->render->DrawTexture(texture[i], centerPosition.x, centerPosition.y, NULL, SDL_FLIP_NONE, 1.0f, rotation[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		app->render->DrawTexture(texture[i], piecePosition[i].x, piecePosition[i].y, NULL, SDL_FLIP_NONE, 1.0f, rotation[i]);
	}

	app->render->DrawTexture(texture[6 + selection], piecePosition[selection].x, piecePosition[selection].y, NULL, SDL_FLIP_NONE, 1.0f, rotation[selection]);
	
}

void Puzzle2::PlaySounds()
{
	if (rotated)
	{
		app->audio->PlayFx(rotateRockFx);
		rotated = false;

	}
	if (placed)
	{
		
		app->audio->PlayFx(placeRockFx);
		placed = false;
	}
	
	

}
bool Puzzle2::CleanUp()
{
	app->tex->UnLoad(texture[0]);
	app->tex->UnLoad(texture[1]);
	app->tex->UnLoad(texture[2]);
	app->tex->UnLoad(texture[3]);
	app->tex->UnLoad(texture[4]);
	app->tex->UnLoad(texture[5]);
	app->tex->UnLoad(texture[6]);
	app->tex->UnLoad(texture[7]);
	app->tex->UnLoad(texture[8]);
	return true;
}
