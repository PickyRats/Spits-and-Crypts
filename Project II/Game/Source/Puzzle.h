#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
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

	bool CleanUp();

	void DrawPieces();

public:

	SDL_Texture* texture1 = NULL;
	SDL_Texture* texture2 = NULL;
	SDL_Texture* texture3 = NULL;
	SDL_Texture* texture4 = NULL;
	SDL_Texture* selectedTexture = NULL;
	SDL_Texture* backgroundTexture = NULL;
	SDL_Texture* backgroundTexture2 = NULL;

	int selectedPiece = -1;
	int pieceSlot = -1;
	bool isSelecting = false;
	bool slotOccupied[4] = { false, false, false, false };
	bool pieceInSlot[4] = { false, false, false, false };
	bool pieceCollected[4] = { false, false, false, false };
	float dt;
	bool showUI = false;

	iPoint slot1Pos = { 550, 200 };
	iPoint slot2Pos = { 650, 200 };
	iPoint slot3Pos = { 550, 300 };
	iPoint slot4Pos = { 650, 300 };

	iPoint piece1InitialPos = { 455, 485 };
	iPoint piece2InitialPos = { 550, 485 };
	iPoint piece3InitialPos = { 645, 485 };
	iPoint piece4InitialPos = { 740, 485 };

	iPoint piece1Pos = {0,0};
	iPoint piece2Pos = {0,0};
	iPoint piece3Pos = {0,0};
	iPoint piece4Pos = {0,0};

};


#endif // __PUZZLE_H__