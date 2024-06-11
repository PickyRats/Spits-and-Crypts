#ifndef __PUZZLE2_H__
#define __PUZZLE2_H__

#include "EntityManager.h"
#include "Entity.h"
//#include "Animation.h"
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

	void PlaySounds();

public:

	SDL_Texture* texture[9] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	int isPieceInserted[3] = { 1, 1, 1 };
	int rotation[6] = { 0, 0, 0, 0, 0, 0 };
	int selection = 0;
	iPoint centerPosition = { 10000, 1010 };
	iPoint leftPosition[3] = { {centerPosition.x - 215, centerPosition.y},{centerPosition.x - 315, centerPosition.y},{centerPosition.x - 395, centerPosition.y}};
	iPoint rightPosition[3] = { {centerPosition.x + 235, centerPosition.y},{centerPosition.x + 335, centerPosition.y},{centerPosition.x + 415, centerPosition.y} };
	//{ 10218, 10378 };
	//{ 465, 185 };
	//900, 100
	iPoint piecePosition[3] = { {0, 0},{ 0, 0 },{ 0, 0 } };
	bool puzzleCompleted = false;
	uint rotateRockFx;
	uint placeRockFx;

	bool rotated = false;
	bool placed = false;
};


#endif // __PUZZLE2_H__