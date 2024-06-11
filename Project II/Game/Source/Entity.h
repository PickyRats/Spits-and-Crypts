#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

class PhysBody;

enum class EntityType
{
	PLAYER,
	DIALOG_TRIGGER,
	NPCS,
	PIEZAS,
	ENEMY,
	TRAP,
	UNKNOWN,
	PUERTAS
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void UpdatePoints(int pointsCost)
	{
		currentPoints -= pointsCost;
	}

	virtual bool HasAvailablePoints(int pointsCost)
	{
		return currentPoints >= pointsCost;
	}

	virtual void SetCombatAnimation(int animationIndex) {
		// 0 = idle
		// 1 = walk
		// 2 = attack
		// 3 = ability
		// 4 = hit
		// 5 = death
	};

	virtual bool AnimationFinished() {
		return true;
	};

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void OnExitCollision(PhysBody* physA, PhysBody* physB) {
	
	};

	virtual void Entity::ResetEntity() {

	};

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	bool isFacingRight = true;

	bool isDead = false;
	int health = 100;
	int totalPoints = 6;
	int currentPoints = 6;
	int attackRange = 3;
	bool setLoadPosition = false;
	int attackDamage = 40;
};

#endif // __ENTITY_H__