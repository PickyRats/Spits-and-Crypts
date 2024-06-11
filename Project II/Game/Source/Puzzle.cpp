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
#include "SceneFloor1.h"
#include "Puertas.h"

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
	failPuzzleFx = app->audio->LoadFx("Assets/Audio/Fx/failPuzzleFx.wav");
	completedPuzzleFx = app->audio->LoadFx("Assets/Audio/Fx/completedPuzzleFx.wav");

	for (int i = 0; i < 4; i++)
	{
		piecePos[i] = pieceInitialPos[i];
	}

	return true;
}

bool Puzzle::Update(float dt)
{


	
	GamePad& pad = app->input->pads[0];
	PlaySounds();

	if (pieceCollected[0] && pieceCollected[1] && pieceCollected[2] && pieceCollected[3])
	{
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN ||(pad.l2 == KEY_DOWN && !wasL2Pressed))
		{
			SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
			if (!isSelecting && !pieceInSlot[0])
			{
				selectedPiece = 0;
				isSelecting = true;

				pieceInSlot[0] = true;
			}
			else if (isSelecting && slotOccupied[0] == -1)
			{
				slotOccupied[0] = selectedPiece;
				piecePos[selectedPiece] = slotPos[0];
				placedpiece = true;
				isSelecting = false;
			}
			wasL2Pressed = true;
		}
		else if (pad.l2 != KEY_DOWN)
		{
			wasL2Pressed = false;
		}


		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || (pad.r2 == KEY_DOWN && !wasR2Pressed))
		{
			SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
			if (!isSelecting && !pieceInSlot[1])
			{
				selectedPiece = 1;
				isSelecting = true;

				pieceInSlot[1] = true;
			}
			else if (isSelecting && slotOccupied[1] == -1)
			{
				slotOccupied[1] = selectedPiece;
				piecePos[selectedPiece] = slotPos[1];
				placedpiece = true;
				isSelecting = false;
			}
			wasR2Pressed = true;
		}
		else if (pad.r2 != KEY_DOWN)
		{
			wasR2Pressed = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN || (pad.l1 == KEY_DOWN && !wasL1Pressed))
		{
			SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
			if (!isSelecting && !pieceInSlot[2])
			{
				selectedPiece = 2;
				isSelecting = true;

				pieceInSlot[2] = true;
			}
			else if (isSelecting && slotOccupied[2] == -1)
			{
				slotOccupied[2] = selectedPiece;
				piecePos[selectedPiece] = slotPos[2];
				placedpiece = true;
				isSelecting = false;
			}
			wasL1Pressed = true;
		}
		else if (pad.l1 != KEY_DOWN)
		{
			wasL1Pressed = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || (pad.r1 == KEY_DOWN && !wasR1Pressed))
		{
			SDL_JoystickSetLED(app->input->joy, 255, 255, 0);
			if (!isSelecting && !pieceInSlot[3])
			{
				selectedPiece = 3;
				isSelecting = true;

				pieceInSlot[3] = true;
			}
			else if (isSelecting && slotOccupied[3] == -1)
			{
				slotOccupied[3] = selectedPiece;
				piecePos[selectedPiece] = slotPos[3];
				placedpiece = true;
				isSelecting = false;
			}
			wasR1Pressed = true;
		}
		else if (pad.r1 != KEY_DOWN)
		{
			wasR1Pressed = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || (pad.b == KEY_DOWN && !wasBPressed))
		{
			ResetPuzzle();
		}
		else if (pad.b != KEY_DOWN)
		{
			wasBPressed = false;
		}
	}

	if (slotOccupied[0] != -1 && slotOccupied[1] != -1 && slotOccupied[2] != -1 && slotOccupied[3] != -1 && !isPuzzleCompleted)
	{
		if (slotOccupied[0] == correctPieces[0]
			&& slotOccupied[1] == correctPieces[1]
			&& slotOccupied[2] == correctPieces[2]
			&& slotOccupied[3] == correctPieces[3])
		{
			isPuzzleCompleted = true;
			app->audio->PlayFx(completedPuzzleFx);
		}

		if (!isPuzzleCompleted) ResetPuzzle();
		else
		{
			LOG("PUZZLE COMPLETED");
			SDL_JoystickSetLED(app->input->joy, 0, 255, 0);
			app->sceneFloor1->levelWidth = 169 * 64;
			app->sceneFloor1->puertas[1]->puzle1Completed = true;
		
		}
	}

	if (canInteract && (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN||(pad.x==KEY_DOWN && !wasXPressed)))
	{
		startPuzzle = true;
		showUI = !showUI;
		wasXPressed=true;
	}
	else if (pad.x != KEY_DOWN)
	{
		wasXPressed = false;
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
	app->audio->PlayFx(failPuzzleFx);
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
	app->audio->UnloadFx(startPuzzleFx);
	app->audio->UnloadFx(placeRockFx);
	app->audio->UnloadFx(failPuzzleFx);
	app->audio->UnloadFx(completedPuzzleFx);

	return true;
}
