#include "EntityManager.h"
#include "Player.h"
#include "Npcs.h"
#include "Piezas_puzle.h"
#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "SceneVillage.h"
#include "DialogTriggerEntity.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool enabled) : Module(enabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;
	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		if (item->data->name != "Player")
		{
			entities.Del(item);
		}
		item = item->prev;
	}
	//entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::DIALOG_TRIGGER:
		entity = new DialogTrigger();
    break;
	case EntityType::NPCS:
		entity = new Npcs();
		break;
	case EntityType::PIEZAS:
		entity = new Piezas_puzle();
    break;
	case EntityType::ENEMY:
		entity = new Enemy();
		break;
	default:
		break;
	}

	entities.Add(entity);
	//entity->Awake();

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	if (app->sceneVillage->pause) return true;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}
