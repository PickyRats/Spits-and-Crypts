#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

class PhysBody;

enum class EntityType
{
	PLAYER,
	UNKNOWN
};

class EntityPlayer
{
public:

	EntityPlayer(EntityType type) : type(type), active(true) {}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	bool isDead = false;
	int health = 100;
	int totalPoints = 10;
	int currentPoints = 10;
	bool setLoadPosition = false;
};

#endif // __ENTITYPLAYER_H__