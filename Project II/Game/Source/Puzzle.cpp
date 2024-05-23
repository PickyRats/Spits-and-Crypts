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
	startPuzzleFx = app->audio->LoadFx("Assets/Audio/Fx/abrir_Puzle.wav");
	placeRockFx = app->audio->LoadFx("Assets/Audio/Fx/Colocar_Piezas.wav");

	for (int i = 0; i < 4; i++)
	{
		piecePos[i] = pieceInitialPos[i];
	}

	return true;
}

bool Puzzle::Update(float dt)
{
	PlaySounds();
	if (pieceCollected[0] && pieceCollected[1] && pieceCollected[2] && pieceCollected[3])
	{
		for (int i = 0; i < 4; ++i)
		{
			if (app->input->GetKey(SDL_SCANCODE_1 + i) == KEY_DOWN)
			{
				SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
				if (!isSelecting && !pieceInSlot[i])
				{
					selectedPiece = i;
					isSelecting = true;
					
					pieceInSlot[i] = true;
				}
				else if (isSelecting && slotOccupied[i] == -1)
				{
					slotOccupied[i] = selectedPiece;
					piecePos[selectedPiece] = slotPos[i];
					placedpiece = true;
					isSelecting = false;
				}
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) ResetPuzzle();
	}

	if (slotOccupied[0] != -1 && slotOccupied[1] != -1 && slotOccupied[2] != -1 && slotOccupied[3] != -1 && !isPuzzleCompleted)
	{
		if (slotOccupied[0] == correctPieces[0] 
			&& slotOccupied[1] == correctPieces[1] 
			&& slotOccupied[2] == correctPieces[2] 
			&& slotOccupied[3] == correctPieces[3])
		{
			isPuzzleCompleted = true;
		}

		if (!isPuzzleCompleted) ResetPuzzle();
		else
		{
			LOG("PUZZLE COMPLETED");
			SDL_JoystickSetLED(app->input->joy, 0, 255, 0);
		}
	}

	if (canInteract && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		startPuzzle = true;
		showUI = !showUI;
	}
	if (showUI) DrawPieces();

	return true;
}

void Puzzle::ResetPuzzle()
{
	SDL_JoystickSetLED(app->input->joy, 255, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		piecePos[i] = pieceInitialPos[i];
	}
	for (int i = 0; i < 4; i++)
	{
		slotOccupied[i] = -1;
		pieceInSlot[i] = false;
	}
	isSelecting = false;
	selectedPiece = -1;
	isPuzzleCompleted = false;
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
void Puzzle::PlaySounds()
{
	if (placedpiece)
	{
			app->audio->PlayFx(placeRockFx);
			placedpiece = false;
		
	}
	if (startPuzzle)
	{
		if (!startSoundPlaying)
		{
			app->audio->PlayFx(startPuzzleFx);
			startSoundPlaying = true;
		}
	}
	else if(startSoundPlaying)
	{
		startSoundPlaying = false;
		app->audio->PauseFx(startPuzzleFx);
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
