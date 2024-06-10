#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include "EntityManager.h"
#include "Entity.h"
//#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class Puzzle : public Module
{
public:

	Puzzle(bool enabled = true);
	virtual ~Puzzle();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void ResetPuzzle();

	bool CleanUp();

	void DrawPieces();
	
	void PlaySounds();

public:

	SDL_Texture* texture[4] = { NULL,NULL,NULL,NULL };

	SDL_Texture* selectedTexture = NULL;
	SDL_Texture* backgroundTexture = NULL;
	SDL_Texture* backgroundTexture2 = NULL;

	int selectedPiece = -1;
	int pieceSlot = -1;
	bool isSelecting = false;
	int slotOccupied[4] = { -1, -1, -1, -1 };
	bool pieceInSlot[4] = { false, false, false, false };
	bool pieceCollected[4] = { false, false, false, false };
	int correctPieces[4] = { 2, 3, 1, 0 };
	float dt;
	bool showUI = false;
	bool isPuzzleCompleted = false;
	bool canInteract = false;
	bool placedpiece = false;
	bool startPuzzle = false;
	bool startSoundPlaying = false;
	iPoint slotPos[4] = { {550, 200},{ 650, 200 },{ 550, 300 },{ 650, 300 } };

	iPoint pieceInitialPos[4] = { {455, 485},{ 550, 485 },{ 645, 485 },{ 740, 485 } };

	iPoint piecePos[4] = {{0,0},{0,0},{0,0},{0,0}};

	uint startPuzzleFx;
	uint rotateRockFx;
	uint placeRockFx;
	uint failPuzzleFx;
	uint completedPuzzleFx;

};


#endif // __PUZZLE_H__