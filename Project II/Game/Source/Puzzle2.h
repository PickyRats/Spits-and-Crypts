#ifndef __PUZZLE2_H__
#define __PUZZLE2_H__

#include "EntityManager.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

struct SDL_Texture;


class Puzzle2 : public Module
{
public:

	Puzzle2(bool enabled = true);
	virtual ~Puzzle2();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void DrawPieces();

public:

	SDL_Texture* texture[9] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	int isPieceInserted[3] = { 1, 1, 1 };
	int rotation[6] = { 0, 0, 0, 0, 0, 0 };
	int selection = 0;
	iPoint leftPosition[3] = { {250, 185},{150, 185},{70, 185}};
	iPoint rightPosition[3] = { {700, 185},{800, 185},{880, 185} };
	iPoint centerPosition = { 465, 185 };
	iPoint piecePosition[3] = { {0, 0},{ 0, 0 },{ 0, 0 } };
	bool puzzleCompleted = false;
};


#endif // __PUZZLE2_H__