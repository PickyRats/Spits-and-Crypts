#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"
#include "Player.h"

#include "PugiXml\src\pugixml.hpp"

enum MapOrientation
{
	ORTOGRAPHIC = 0,
	ISOMETRIC
};

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString name;
	int firstgid;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;
	int tilecount;
	int columns;

	SDL_Texture* texture;

	SDL_Rect GetRect(uint gid) {
		SDL_Rect rect = { 0 };

		int relativeIndex = gid - firstgid;
		rect.w = tileWidth;
		rect.h = tileHeight;
		rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
		rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);

		return rect;
	}
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value; //We assume that we are going to work only with bool for the moment
	};

	List<Property*> propertyList;

	~Properties()
	{
		ListItem<Property*>* property;
		property = propertyList.start;

		while (property != NULL)
		{
			RELEASE(property->data);
			property = property->next;
		}

		propertyList.Clear();
	}

	Property* GetProperty(const char* name);

};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* tiles;
	Properties properties;

	uint Get(int x, int y) const
	{
		return tiles[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	List<TileSet*> tilesets;

	MapOrientation orientation;

	List<MapLayer*> layers;
};

class Map : public Module
{
public:

    Map(bool enabled = true);

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& config);

	bool Start();

	// Called each loop iteration
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
	bool Load(SString mapFileName);

	iPoint MapToWorld(int x, int y) const;

	// Obtain map coordinates from screen coordinates 
	iPoint WorldToMap(int x, int y);

	// Implement function to the Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int gid) const;

	// Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// Create navigation map for pathfinding
	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	int GetTileWidth();
	int GetTileHeight();

	void UpdateMapSize();
	void UpdateTileLoadSize();


	void EnableLayer(const char* layerName, bool enable = true);

private:

	bool CreateColliders();
	void DestroyAllColliders();
	bool LoadEntities();
	void CreateEntities(const char* nodeName, EntityType entityType, iPoint pos);

public: 
	Player* player;
	Player* player2;
	SString mapName;
	SString path;
	PathFinding* pathfinding;

	List <PhysBody*> tempColliders;
	int mapIdx = 1; //1= sceneVillage, 2 = sceneShop, 3 = sceneOasisFaraon, 4 = sceneTemple

	List<PhysBody*> wallEndCollision;

	pugi::xml_document configFile;
	pugi::xml_node configNode;

private:

	MapData mapData;
	bool mapLoaded;
	MapLayer* navigationLayer;
	int blockedGid = 49;
	int walkableGid = 4;
	int ladderTopGid = 1;
	int ladderBottomGid = 2;
	int startHeight = 0;
	int endHeight = 0;
	int startWidth = 0;
	int endWidth = 0;
	int startMapHeight = 0;
	int endMapHeight = 12;
	int startMapWidth = 45;
	int endMapWidth = 0;
	int tilesToLoad = 22;
	int tilesSize = 64;
};

#endif // __MAP_H__