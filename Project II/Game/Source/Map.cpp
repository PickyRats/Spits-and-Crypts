
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "SceneCombat.h"
#include "SceneVillage.h"
#include "SceneShop.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"


Map::Map(bool enabled) : Module(enabled)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    if (config.child("player")) {
    	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
    	player->parameters = config.child("player");
    }

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += mapName;
    Load(mapPath);

    //Initialize the pathfinding
    pathfinding = new PathFinding();

    //Initialize the navigation map
    uchar* navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap);
    pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
    RELEASE_ARRAY(navigationMap);

    UpdateMapSize();

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if (mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayer;
    mapLayer = mapData.layers.start;

    // iterates the layers in the map
    while (mapLayer != NULL) {
        //Check if the property Draw exist get the value, if it's true draw the lawyer
        if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value) {
            //iterate all tiles in a layer

            UpdateTileLoadSize();

            for (int i = startWidth; i < endWidth; i++) {
                for (int j = startHeight; j < endHeight; j++) {
                    //Get the gid from tile
                    int gid = mapLayer->data->Get(i, j);


                    TileSet* tileSet = GetTilesetFromTileId(gid);
                    SDL_Rect tileRect = tileSet->GetRect(gid);

                    iPoint mapCoord = MapToWorld(i, j);

                    app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);

                }
            }
        }
        mapLayer = mapLayer->next;
    }

    return true;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    ListItem<TileSet*>* tileSet;
    tileSet = mapData.tilesets.start;
    while (tileSet != NULL) {
        set = tileSet->data;
        if (gid >= tileSet->data->firstgid && gid < (tileSet->data->firstgid + tileSet->data->tilecount)) break;
        tileSet = tileSet->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    //Clean up pathfing class
    pathfinding->CleanUp();

    // Remove all tilesets
    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;

    while (item != NULL)
    {
        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.layers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }
    mapData.layers.Clear();

    DestroyAllColliders();

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    // retrieve the paremeters of the <map> node and save it into map data

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }
    else {

        //Fill mapData variable
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

        SString orientationStr = mapFileXML.child("map").attribute("orientation").as_string();
        if (orientationStr == "orthogonal") {
            mapData.orientation = MapOrientation::ORTOGRAPHIC;
        }
        else if (orientationStr == "isometric") {
            mapData.orientation = MapOrientation::ISOMETRIC;
        }
        else {
            LOG("Map orientation not found");
            ret = false;
        }

       // Iterate the Tileset
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

            TileSet* tileset = new TileSet();

            //Load Tileset attributes
            tileset->name = tilesetNode.attribute("name").as_string();
            tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
            tileset->margin = tilesetNode.attribute("margin").as_int();
            tileset->spacing = tilesetNode.attribute("spacing").as_int();
            tileset->tileWidth = tilesetNode.attribute("tilewidth").as_int();
            tileset->tileHeight = tilesetNode.attribute("tileheight").as_int();
            tileset->columns = tilesetNode.attribute("columns").as_int();
            tileset->tilecount = tilesetNode.attribute("tilecount").as_int();

            //Load Tileset image
            SString mapTex = path;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = app->tex->Load(mapTex.GetString());

            mapData.tilesets.Add(tileset);

        }

        // Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            // Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Reserve the memory for the data 
            mapLayer->tiles = new uint[mapLayer->width * mapLayer->height];
            memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

            //Iterate over all the tiles and assign the values in the data array
            int i = 0;
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles[i] = tileNode.attribute("gid").as_uint();
                i++;
            }

            //add the layer to the map
            mapData.layers.Add(mapLayer);
        }

        //Create colliders      
        CreateColliders();

        //Load entities
        LoadEntities();

        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

            LOG("Tilesets----");

            ListItem<TileSet*>* tileset;
            tileset = mapData.tilesets.start;
            while (tileset != NULL) {
                //iterate the tilesets
                LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
                LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
                LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
                tileset = tileset->next;
            }

            LOG("Layers----");

            ListItem<MapLayer*>* mapLayer;
            mapLayer = mapData.layers.start;

            while (mapLayer != NULL) {
                LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
                LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
                mapLayer = mapLayer->next;
            }
        }

        // Find the navigation layer
        ListItem<MapLayer*>* mapLayerItem;
        mapLayerItem = mapData.layers.start;
        navigationLayer = mapLayerItem->data;

        //Search the layer in the map that contains information for navigation
        while (mapLayerItem != NULL) {
            if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
                navigationLayer = mapLayerItem->data;
                break;
            }
            mapLayerItem = mapLayerItem->next;
        }

        //Resets the map
        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    // L09: DONE 3: Get the screen coordinates of tile positions for isometric maps 
    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x * mapData.tileWidth;
        ret.y = y * mapData.tileHeight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        ret.x = x * mapData.tileWidth / 2 - y * mapData.tileWidth / 2;
        ret.y = x * mapData.tileHeight / 2 + y * mapData.tileHeight / 2;
    }

    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.Add(p);
    }

    return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* property = propertyList.start;
    Property* p = NULL;

    while (property)
    {
        if (property->data->name == name) {
            p = property->data;
            break;
        }
        property = property->next;
    }

    return p;
}

iPoint Map::WorldToMap(int x, int y) {

    iPoint ret(0, 0);

    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        float half_width = mapData.tileWidth / 2;
        float half_height = mapData.tileHeight / 2;
        ret.x = int((x / half_width + y / half_height) / 2);
        ret.y = int((y / half_height - (x / half_width)) / 2);
    }

    return ret;
}

int Map::GetTileWidth() {
    return mapData.tileWidth;
}

int Map::GetTileHeight() {
    return mapData.tileHeight;
}

void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < mapData.width; x++)
    {
        for (int y = 0; y < mapData.height; y++)
        {
            //i is the index of x,y coordinate in a unidimensional array that represents the navigation map
            int i = (y * navigationLayer->width) + x;

            //Gets the gid of the map in the navigation layer
            int gid = navigationLayer->Get(x, y);

            //If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
            //!!!! make sure that you assign blockedGid according to your map
            if (gid == walkableGid) navigationMap[i] = 1;
            else if (gid == ladderBottomGid) navigationMap[i] = 2;
            else if (gid == ladderTopGid) navigationMap[i] = 3;
            else navigationMap[i] = 0;
           
        }
    }

    *buffer = navigationMap;
    width = mapData.width;
    height = mapData.height;

}

bool Map::CreateColliders()
{
    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.layers.start;

    bool ret = false;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->name == "Collisions")
        {
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    if (mapLayerItem->data->Get(x, y) != 0)
                    {
                        iPoint pos = MapToWorld(x, y);

                        PhysBody* c1 = nullptr;

                        switch (mapLayerItem->data->Get(x, y))
                        {
                        case 3:
                            c1 = app->physics->CreateRectangle(pos.x + (mapData.tileWidth / 2), pos.y + (mapData.tileHeight / 2), mapData.tileWidth, mapData.tileHeight, STATIC);
                            c1->ctype = ColliderType::WALL;
                            ret = true;
                            break;
                        case 4:
                            c1 = app->physics->CreateRectangle(pos.x + (mapData.tileWidth / 2), pos.y + (mapData.tileHeight / 2), mapData.tileWidth, mapData.tileHeight, STATIC);
                            c1->ctype = ColliderType::PLATFORM;
                            ret = true;
                            break;
                        case 5:
                            c1 = app->physics->CreateRectangleSensor(pos.x + (mapData.tileWidth / 2), pos.y, mapData.tileWidth, mapData.tileHeight*2, STATIC);
                            c1->ctype = ColliderType::DOOR_SHOP;
                            ret = true;
                            break;
                        case 6:
                            c1 = app->physics->CreateRectangleSensor(pos.x + (mapData.tileWidth / 2), pos.y , mapData.tileWidth, mapData.tileHeight*2, STATIC);
                            c1->ctype = ColliderType::DOOR_OASIS;
                            ret = true;
                            break;
                        case 7:
                            c1 = app->physics->CreateRectangleSensor(pos.x + (mapData.tileWidth / 2), pos.y , mapData.tileWidth, mapData.tileHeight*2, STATIC);
                            c1->ctype = ColliderType::DOOR_TEMPLE;
                            ret = true;
                            break;
                        case 8:
                            c1 = app->physics->CreateRectangleSensor(pos.x + (mapData.tileWidth / 2), pos.y, mapData.tileWidth, mapData.tileHeight*2, STATIC);
                            c1->ctype = ColliderType::DOOR_ALDEA;
                            ret = true;
                            break;
                        case 10:
                            c1 = app->physics->CreateRectangle(pos.x + (mapData.tileWidth / 2), pos.y + (mapData.tileHeight / 2), mapData.tileWidth, mapData.tileHeight, STATIC);
                            c1->ctype = ColliderType::COMBAT;
                            ret = true;
                            break;
                        case 11:
                            c1 = app->physics->CreateRectangleSensor(pos.x + (mapData.tileWidth / 2), pos.y , mapData.tileWidth, mapData.tileHeight*2, STATIC);
                            c1->ctype = ColliderType::DOOR_FLOOR_1;
                            ret = true;
                            break;
                        case 12:
                            c1 = app->physics->CreateRectangle(pos.x + (mapData.tileWidth / 2), pos.y, mapData.tileWidth, mapData.tileHeight * 2, STATIC);
                            c1->ctype = ColliderType::TRAP;
                            ret = true;
                            break;                                
                       /* case 5:
                            c1 = app->physics->CreateRectangle(pos.x + (mapData.tileWidth / 2), pos.y + (mapData.tileHeight / 2), mapData.tileWidth, mapData.tileHeight, STATIC);
                            c1->ctype = ColliderType::ITERACTABLE;
                            ret = true;
                            break;*/
                        default:
                            break;
                        }
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }

    return ret;
}

void Map::DestroyAllColliders()
{
    b2World* physicsWorld = app->physics->world;
    b2Body* body = physicsWorld->GetBodyList();

    while (body)
    {
        b2Body* nextBody = body->GetNext();
        PhysBody* physBody = static_cast<PhysBody*>(body->GetUserData());

        if (physBody)
        {
            ColliderType ctype = physBody->ctype;

            // Comprueba el tipo del collider y elimina solo los tipos específicos

            if (ctype == ColliderType::WALL 
                || ctype == ColliderType::PLATFORM 
                || ctype == ColliderType::DOOR_SHOP 
                || ctype == ColliderType::DOOR_OASIS 
                || ctype == ColliderType::DOOR_TEMPLE
                || ctype == ColliderType::DOOR_ALDEA
                || ctype == ColliderType::DOOR_FLOOR_1
                || ctype == ColliderType:: COMBAT)
            {
                physicsWorld->DestroyBody(body);
            }
        }

        body = nextBody;
    }
}

void Map::UpdateMapSize()
{

    if (mapIdx == 1)
    {
        startMapWidth = 0;
        startMapHeight = 0;
        endMapWidth = 45;
        endMapHeight = 12;
    }
    else
    {
        startMapWidth = 0;
        startMapHeight = 0;
        endMapWidth = mapData.width;
        endMapHeight = mapData.height;
    }
}

void Map::UpdateTileLoadSize()
{

    iPoint playerPosition = app->map->player->position;

    int playerX = playerPosition.x / tilesSize;
    int playerY = playerPosition.y / tilesSize;

    startWidth = (playerX - tilesToLoad < startMapWidth) ? startMapWidth : playerX - tilesToLoad;
    endWidth = (playerX + tilesToLoad > mapData.width) ? mapData.width : playerX + tilesToLoad;
    startHeight = (playerY - tilesToLoad < startMapHeight) ? startMapHeight : playerY - tilesToLoad;
    endHeight = (playerY + tilesToLoad > mapData.height) ? mapData.height : playerY + tilesToLoad;
}

bool Map::LoadEntities()
{

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.layers.start;
    bool ret = false;

    pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
    if (parseResult) configNode = configFile.child("config").child("scene");

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->name == "Entities") {
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);
                    SDL_Rect r = tileset->GetRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    /*if (gid == 4153) CreateEntities("key", EntityType::KEY, pos);
                    if (gid == 4154) CreateEntities("equipment", EntityType::EQUIPMENT, pos);
                    if (gid == 4155) CreateEntities("lockDoor", EntityType::LOCK_DOOR, pos);
                    if (gid == 4156) CreateEntities("jumper", EntityType::JUMPER, pos);
                    if (gid == 4157) CreateEntities("crumblingPlatform", EntityType::CRUMBLING_PLATFORM, pos);
                    if (gid == 4158) CreateEntities("skeleton", EntityType::SKELETON, pos);
                    if (gid == 4159) CreateEntities("ghost", EntityType::GHOST, pos);
                    if (gid == 4160) CreateEntities("obstacle", EntityType::OBSTACLE, pos);
                    if (gid == 4161) CreateEntities("chest", EntityType::CHEST, pos);
                    if (gid == 4162) CreateEntities("log", EntityType::LOG_OBSTACLE, pos);
                    if (gid == 4163) CreateEntities("torch", EntityType::TORCH, pos);
                    if (gid == 4164) CreateEntities("life", EntityType::LIFE, pos);
                    if (gid == 4165) CreateEntities("crumblingPlatform2", EntityType::CRUMBLING_PLATFORM, pos);*/
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return false;
}

void Map::CreateEntities(const char* nodeName, EntityType entityType, iPoint pos)
{
    pugi::xml_node entityNode = configNode.child(nodeName);

    Entity* entity = app->entityManager->CreateEntity(entityType);
    entity->parameters = entityNode;
    entity->position = iPoint(pos.x, pos.y);
    
}
