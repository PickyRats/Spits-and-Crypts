#include "Puzzle.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "FadeToBlack.h"

Puzzle::Puzzle(bool enabled) : Module(enabled)
{
	name.Create("Puzzle");

}

Puzzle::~Puzzle() {

}

bool Puzzle::Awake() {

	return true;
}

bool Puzzle::Start() {

	//initilize texture
	texture[0] = app->tex->Load("Assets/Textures/pieza_puzle_1.png");
	texture[1] = app->tex->Load("Assets/Textures/pieza_puzle_2.png");
	texture[2] = app->tex->Load("Assets/Textures/pieza_puzle_3.png");
	texture[3] = app->tex->Load("Assets/Textures/pieza_puzle_4.png");
	selectedTexture = app->tex->Load("Assets/Textures/selected_piece.png");
	backgroundTexture = app->tex->Load("Assets/Textures/puzzle_background.png");
	backgroundTexture2 = app->tex->Load("Assets/Textures/puzzle_background2.png");

	for (int i = 0; i < 4; i++)
	{
		piecePos[i] = pieceInitialPos[i];
	}

	return true;
}

bool Puzzle::Update(float dt)
{
	if (pieceCollected[0] && pieceCollected[1] && pieceCollected[2] && pieceCollected[3])
	{
		for (int i = 0; i < 4; ++i)
		{
			if (app->input->GetKey(SDL_SCANCODE_1 + i) == KEY_DOWN)
			{
				if (!isSelecting && !pieceInSlot[i])
				{
					selectedPiece = i;
					isSelecting = true;
					pieceInSlot[i] = true;
				}
				else if (isSelecting && !slotOccupied[i])
				{
					slotOccupied[i] = true;
					piecePos[selectedPiece] = slotPos[i];
					isSelecting = false;
				}
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			for (int i = 0; i < 4; i++)
			{
				piecePos[i] = pieceInitialPos[i];
			}
			for (int i = 0; i < 4; i++)
			{
				slotOccupied[i] = false;
				pieceInSlot[i] = false;
			}
			isSelecting = false;
			selectedPiece = -1;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		showUI = !showUI;
	}
	if (showUI) DrawPieces();

	return true;
}

void Puzzle::DrawPieces()
{
	app->render->DrawTexture(backgroundTexture, 530, 180, NULL, SDL_FLIP_NONE, 0.0f);
	app->render->DrawTexture(backgroundTexture2, 430, 460, NULL, SDL_FLIP_NONE, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		if (pieceCollected[i])
		{
			app->render->DrawTexture(texture[i], piecePos[i].x, piecePos[i].y, NULL, SDL_FLIP_NONE, 0.0f);
		}
	}

	if (isSelecting && selectedPiece >= 0 && selectedPiece < 4)
	{
		app->render->DrawTexture(selectedTexture, pieceInitialPos[selectedPiece].x, pieceInitialPos[selectedPiece].y, NULL, SDL_FLIP_NONE, 0.0f);
	}

}

bool Puzzle::CleanUp()
{
	app->tex->UnLoad(texture[0]);
	app->tex->UnLoad(texture[1]);
	app->tex->UnLoad(texture[2]);
	app->tex->UnLoad(texture[3]);

	return true;
}
