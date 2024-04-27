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
	texture1 = app->tex->Load("Assets/Textures/pieza_puzle_1.png");
	texture2 = app->tex->Load("Assets/Textures/pieza_puzle_2.png");
	texture3 = app->tex->Load("Assets/Textures/pieza_puzle_3.png");
	texture4 = app->tex->Load("Assets/Textures/pieza_puzle_4.png");
	selectedTexture = app->tex->Load("Assets/Textures/selected_piece.png");
	backgroundTexture = app->tex->Load("Assets/Textures/puzzle_background.png");
	backgroundTexture2 = app->tex->Load("Assets/Textures/puzzle_background2.png");

	piece1Pos = piece1InitialPos;
	piece2Pos = piece2InitialPos;
	piece3Pos = piece3InitialPos;
	piece4Pos = piece4InitialPos;

	return true;
}

bool Puzzle::Update(float dt)
{
	if (pieceCollected[0] && pieceCollected[1] && pieceCollected[2] && pieceCollected[3])
	{

		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			if (!isSelecting && !pieceInSlot[0])
			{
				selectedPiece = 0;
				isSelecting = true;
				pieceInSlot[0] = true;
			}
			else if (isSelecting && !slotOccupied[0])
			{
				slotOccupied[0] = true;
				if (selectedPiece == 0) piece1Pos = slot1Pos;
				else if (selectedPiece == 1) piece2Pos = slot1Pos;
				else if (selectedPiece == 2) piece3Pos = slot1Pos;
				else if (selectedPiece == 3) piece4Pos = slot1Pos;
				isSelecting = false;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			if (!isSelecting && !pieceInSlot[1])
			{
				selectedPiece = 1;
				isSelecting = true;
				pieceInSlot[1] = true;
			}
			else if (isSelecting && !slotOccupied[1])
			{
				slotOccupied[1] = true;
				if (selectedPiece == 0) piece1Pos = slot2Pos;
				else if (selectedPiece == 1) piece2Pos = slot2Pos;
				else if (selectedPiece == 2) piece3Pos = slot2Pos;
				else if (selectedPiece == 3) piece4Pos = slot2Pos;
				isSelecting = false;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			if (!isSelecting && !pieceInSlot[2])
			{
				selectedPiece = 2;
				isSelecting = true;
				pieceInSlot[2] = true;
			}
			else if (isSelecting && !slotOccupied[2])
			{
				slotOccupied[2] = true;
				if (selectedPiece == 0) piece1Pos = slot3Pos;
				else if (selectedPiece == 1) piece2Pos = slot3Pos;
				else if (selectedPiece == 2) piece3Pos = slot3Pos;
				else if (selectedPiece == 3) piece4Pos = slot3Pos;
				isSelecting = false;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			if (!isSelecting && !pieceInSlot[3])
			{
				selectedPiece = 3;
				isSelecting = true;
				pieceInSlot[3] = true;
			}
			else if (isSelecting && !slotOccupied[3])
			{
				slotOccupied[3] = true;
				if (selectedPiece == 0) piece1Pos = slot4Pos;
				else if (selectedPiece == 1) piece2Pos = slot4Pos;
				else if (selectedPiece == 2) piece3Pos = slot4Pos;
				else if (selectedPiece == 3) piece4Pos = slot4Pos;
				isSelecting = false;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			piece1Pos = piece1InitialPos;
			piece2Pos = piece2InitialPos;
			piece3Pos = piece3InitialPos;
			piece4Pos = piece4InitialPos;
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
	if (pieceCollected[0]) app->render->DrawTexture(texture1, piece1Pos.x, piece1Pos.y, NULL, SDL_FLIP_NONE, 0.0f);
	if (pieceCollected[1]) app->render->DrawTexture(texture2, piece2Pos.x, piece2Pos.y, NULL, SDL_FLIP_NONE, 0.0f);
	if (pieceCollected[2]) app->render->DrawTexture(texture3, piece3Pos.x, piece3Pos.y, NULL, SDL_FLIP_NONE, 0.0f);
	if (pieceCollected[3]) app->render->DrawTexture(texture4, piece4Pos.x, piece4Pos.y, NULL, SDL_FLIP_NONE, 0.0f);
	if (isSelecting)
	{
		if (selectedPiece == 0) app->render->DrawTexture(selectedTexture, piece1InitialPos.x, piece1InitialPos.y, NULL, SDL_FLIP_NONE, 0.0f);
		else if (selectedPiece == 1) app->render->DrawTexture(selectedTexture, piece2InitialPos.x, piece2InitialPos.y, NULL, SDL_FLIP_NONE, 0.0f);
		else if (selectedPiece == 2) app->render->DrawTexture(selectedTexture, piece3InitialPos.x, piece3InitialPos.y, NULL, SDL_FLIP_NONE, 0.0f);
		else if (selectedPiece == 3) app->render->DrawTexture(selectedTexture, piece4InitialPos.x, piece4InitialPos.y, NULL, SDL_FLIP_NONE, 0.0f);
	}
}

bool Puzzle::CleanUp()
{
	app->tex->UnLoad(texture1);
	app->tex->UnLoad(texture2);
	app->tex->UnLoad(texture3);
	app->tex->UnLoad(texture4);

	return true;
}
