#pragma once
#include "Module.h"
#include "Entity.h"

#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -8.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
	DYNAMIC,
	STATIC,
	KINEMATIC
};

enum class ColliderType {
	PLAYER,
	PLAYER_BODY,
	NPC,
	PLATFORM,
	DOOR_SHOP,
	DOOR_OASIS,
	DOOR_TEMPLE,
	DOOR_ALDEA,
	DOOR_FLOOR_1,
	DOOR_CHOZA,
	DOOR_PUZLE_2,
	COMBAT1,
	COMBAT2,
	COMBAT3,
	WALL,
	DOOR,
	DIALOG_TRIGGER,
	TRAP,
	PUZZLE,
	ITERACTABLE,
	ITEM,
	ENEMY,
	STAIRS,
	LIGHT1,
	LIGHT2,
	ROCK,
	SENSOR_ROCK,
	TEMP_PLATFORM,
	UNKNOWN
	// ..
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL), ctype(ColliderType::UNKNOWN)
	{}

	~PhysBody() {}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Entity* listener;
	ColliderType ctype;
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener // TODO
{
public:

	// Constructors & Destructors
	Physics();
	~Physics();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	// Create basic physics objects
	PhysBody* CreateRectangle(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateCircle(int x, int y, int radious, bodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, bodyType type);
	
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	// Box2D World
	b2World* world;

	// Debug mode
	bool debug;
};