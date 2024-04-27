#include "Puzzle2.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
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


	for (int i = 0; i < 3; i++)
	{
		piecePosition[i] = leftPosition[i];
	}
	
	return true;
}

bool Puzzle2::Update(float dt)
{
	DrawPieces();

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		for (int i = 0; i < 3; i++)
		{
			if (selection == i)
			{
				if (rotation[i + 3] >= 90) rotation[i + 3] -= 90;
				else rotation[i + 3] = 270;
				if (isPieceInserted[i] == 0)
				{
					if (rotation[i] >= 90) rotation[i] -= 90;
					else rotation[i] = 270;
				}
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		for (int i = 0; i < 3; i++)
		{
			if (selection == i)
			{
				if (rotation[i + 3] < 270) rotation[i + 3] += 90;
				else rotation[i + 3] = 0;
				if (isPieceInserted[i] == 0)
				{
					if (rotation[i] < 270) rotation[i] += 90;
					else rotation[i] = 0;
				}
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (selection < 2) selection++;
		else selection = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		if (selection == 0)
		{
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
	}

	return true;
}

void Puzzle2::ResetPuzzle()
{
	
}

void Puzzle2::DrawPieces()
{
	for (int i = 3; i < 6; i++)
	{
		app->render->DrawTexture(texture[i], centerPosition.x, centerPosition.y, NULL, SDL_FLIP_NONE, 0.0f, rotation[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		app->render->DrawTexture(texture[i], piecePosition[i].x, piecePosition[i].y, NULL, SDL_FLIP_NONE, 0.0f, rotation[i]);
	}

	app->render->DrawTexture(texture[6 + selection], piecePosition[selection].x, piecePosition[selection].y, NULL, SDL_FLIP_NONE, 0.0f, rotation[selection]);
	
}

bool Puzzle2::CleanUp()
{

	return true;
}
