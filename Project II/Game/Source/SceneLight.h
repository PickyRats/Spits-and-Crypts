#ifndef __SCENELIGHT_H__
#define __SCENELIGHT_H__

#include "Module.h"
#include "Player.h"
#include "SceneMenu.h"
#include "Timer.h"
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
struct Trapdoor
{
	iPoint position;
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

	void ShowNotification();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetRays();

	void DrawLightMirrors();

	void StartCameraShakeX(float duration, float intensity);

	void StartCameraShakeY(float duration, float intensity);

	void UpdateCameraShake();

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	bool pause = false;

	bool interactMirror = false;
	bool interactTrapdoor = false;	
	
	bool isInteractingMirror = false;
	bool isInteractingTrapdoor = false;
	bool wasUpPressed = false;
	bool wasDownPressed = false;
	bool wasL1Pressed = false;
	bool wasR1Pressed = false;
	bool wasL2Pressed = false;
	bool wasR2Pressed = false;
private:

	bool cameraInitialized = false;
	bool shakingCameraX = false;
	bool shakingCameraY = false;
	float shakeDuration;
	float shakeIntensity;
	float shakeTimer;

	SDL_Texture* backgroundTexture;

	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 20 * 64;
	int levelHeight = 19 * 64;

	int playerX, playerY, cameraX, cameraY;

	SDL_Texture* notificationTexture;
	SDL_Texture* lightMirrorTexture;
	SDL_Texture* lightRayTexture;
	SDL_Rect lightMirrorRect[10] = { {0,0,64,64}, {64,0,64,64}, {128,0,64,64}, {192,0,64,64}, 
									{0,64,64,64}, {64,64,64,64}, {128,64,64,64}, {192,64,64,64}, 
									{0,128,64,64}, {64,128,64,64} };
	SDL_Rect lightRayRect[4] = { {0,0,64,64}, {64,0,64,64}, {128,0,64,64}, {192,0,64,64} };

	//audios
	uint trampillafx;
	uint mirror_rotationFx;

	int mirrorRotation[4] = { 0, 90, 180, 270 };
	int rayRotation[4] = { 0, 90, 180, 270 };

	LightMirror lightMirrors[11];
	Trapdoor trapdoors[2];
	RayTiles rayTiles[20][18];

	int mirrorIndex = 0;
	int trapdoorIndex = 0;

	bool notificationCreated = false;
	bool isVisible = false;
	bool isUp = false;
	bool isDown = false;
	int offset[7] = { 680, 690, 660, 1000, 1000, 650, 650 };
	Timer timer;

	PhysBody* mirrorBody = nullptr;
	PhysBody* trapdoorBody = nullptr;


};

#endif // __SCENELIGHT_H__