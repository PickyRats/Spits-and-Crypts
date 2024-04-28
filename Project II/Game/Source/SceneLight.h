#ifndef __SCENELIGHT_H__
#define __SCENELIGHT_H__

#include "Module.h"
#include "Player.h"
#include "Npcs.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SceneMenu.h"
#include <vector>

struct SDL_Texture;
struct RayTiles
{
	iPoint position;
	int type = -1;
	int rotation = 0;
};
struct LightMirror
{
	iPoint position;
	int type = 0;
	int rotation = 0;
};

class SceneLight : public Module
{
public:

	SceneLight(bool enabled = true);

	// Destructor
	virtual ~SceneLight();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetRays();

	void DrawLightMirrors();

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	GuiControlButton* gcButtom;

	bool pause = false;

private:
	SDL_Texture* backgroundTexture;

	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 20 * 64;
	int levelHeight = 19 * 64;

	int playerX, playerY, cameraX, cameraY;

	SDL_Texture* lightMirrorTexture;
	SDL_Texture* lightRayTexture;
	SDL_Rect lightMirrorRect[4] = { {0,0,64,64}, {64,0,64,64}, {128,0,64,64}, {192,0,64,64} };
	SDL_Rect lightRayRect[4] = { {0,0,64,64}, {64,0,64,64}, {128,0,64,64}, {192,0,64,64} };
	int mirrorRotation[4] = { 0, 90, 180, 270 };
	int rayRotation[4] = { 0, 90, 180, 270 };

	LightMirror lightMirrors[8];
	RayTiles rayTiles[20][18];
};

#endif // __SCENELIGHT_H__