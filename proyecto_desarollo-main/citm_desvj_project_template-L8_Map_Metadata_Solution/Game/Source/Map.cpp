#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"
#include "Defs.h"
#include "Log.h"
#include "player.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}
int cambio = 10;
// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    Load(mapPath);

    //Initialize pathfinding 
    pathfinding = new PathFinding();

    //Initialize the navigation map
    uchar* navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap);
    pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
    RELEASE_ARRAY(navigationMap);

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if (mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayer;
    mapLayer = mapData.layers.start;

    // Comprueba si se ha presionado la tecla F1
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
    {
        // Invierte el valor de la propiedad "Draw" de la capa de colisiones
        while (mapLayer != NULL)
        {
            // Comprueba si la capa actual es la capa de colisiones
            if (mapLayer->data->name == "Collisions")
            {
                Properties::Property* property = mapLayer->data->properties.GetProperty("Draw");
                if (property != NULL)
                {
                    property->value = !property->value;
                }
            }
            mapLayer = mapLayer->next;
        }

        // Reinicia el iterador de capas
        mapLayer = mapData.layers.start;
    }

    // Dibuja las capas
    while (mapLayer != NULL)
    {
        Properties::Property* property = mapLayer->data->properties.GetProperty("Draw");
        if (property != NULL && property->value)
        {
            // Aqu� va tu c�digo para dibujar las colisiones...
            if ((app->scene->GetPlayer()->position.x) / 32 - 40 > 0)
            {
                for (int i = (app->scene->GetPlayer()->position.x) / 32 - 40; i < (app->scene->GetPlayer()->position.x) / 32 + 40; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->data->Get(i, j);
                        TileSet* tileSet = GetTilesetFromTileId(gid);
                        SDL_Rect tileRect = tileSet->GetRect(gid);
                        iPoint mapCoord = MapToWorld(i, j);
                        app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);
                    }
                }
            }
            else
            {
                for (int i = 0; i < (app->scene->GetPlayer()->position.x) / 32 + 40; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->data->Get(i, j);
                        TileSet* tileSet = GetTilesetFromTileId(gid);
                        SDL_Rect tileRect = tileSet->GetRect(gid);
                        iPoint mapCoord = MapToWorld(i, j);
                        app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);
                    }
                }
            }
        }

        mapLayer = mapLayer->next;
    }

    


    


    return ret;
}






// L08: DONE 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    ListItem<TileSet*>* tileSet;
    tileSet = mapData.tilesets.start;
    while (tileSet != NULL) {
        set = tileSet->data;
        if (gid >= tileSet->data->firstgid && gid < tileSet->data->tilecount) break;
        tileSet = tileSet->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L05: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;
    
    while (tileset != NULL) {
        RELEASE(tileset->data);
        tileset = tileset->next;
    }

    mapData.tilesets.Clear();

    // L06: DONE 2: clean up all layer data
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.layers.start;

    while (layerItem != NULL) {
        RELEASE(layerItem->data->tiles);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    // L05: DONE 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }
    else {

        //Fill mapData variable
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tilewidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileheight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L05: DONE 4: Implement the LoadTileSet function to load the tileset properties
       // Iterate the Tileset
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

            TileSet* tileset = new TileSet();

            //Load Tileset attributes
            tileset->name = tilesetNode.attribute("name").as_string();
            tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
            tileset->margin = tilesetNode.attribute("margin").as_int();
            tileset->spacing = tilesetNode.attribute("spacing").as_int();
            tileset->tilewidth = tilesetNode.attribute("tilewidth").as_int();
            tileset->tileheight = tilesetNode.attribute("tileheight").as_int();
            tileset->columns = tilesetNode.attribute("columns").as_int();
            tileset->tilecount = tilesetNode.attribute("tilecount").as_int();

            //Load Tileset image
            SString mapTex = path;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = app->tex->Load(mapTex.GetString());

            mapData.tilesets.Add(tileset);

        }

        // L06: DONE 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L06: DONE 4: Implement a function that loads a single layer layer
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L08: TODO 6 Call Load Layer Properties
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


        // L07 TODO 3: Create colliders
        // Later you can create a function here to load and create the colliders from the map
        PhysBody* c1 = app->physics->CreateRectangle(224 + 128, 544 + 32, 256, 64, STATIC);
        c1->ctype = ColliderType::PLATFORM;

        PhysBody* c2 = app->physics->CreateRectangle(352 + 64, 384 + 32, 128, 64, STATIC);
        c2->ctype = ColliderType::PLATFORM;

        PhysBody* c3 = app->physics->CreateRectangle(256, 704 + 32, 576, 64, STATIC);
        c3->ctype = ColliderType::PLATFORM;

        PhysBody* c4 = app->physics->CreateRectangle(47, 350, 32, 704, STATIC);
        c4->ctype = ColliderType::PLATFORM;

        PhysBody* c5 = app->physics->CreateRectangle(815, 736, 352, 64, STATIC);
        c5->ctype = ColliderType::PLATFORM;

        PhysBody* c6 = app->physics->CreateRectangle(815, 736, 352, 64, STATIC);
        c6->ctype = ColliderType::PLATFORM;


        LoadColisions();
        // L07 TODO 7: Assign collider type
        

        // L05: DONE 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tilewidth, mapData.tileheight);

            LOG("Tilesets----");

            ListItem<TileSet*>* tileset;
            tileset = mapData.tilesets.start;
            while (tileset != NULL) {
                //iterate the tilesets
                LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
                LOG("tile width : %d tile height : %d", tileset->data->tilewidth, tileset->data->tileheight);
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

        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tilewidth;
    ret.y = y * mapData.tileheight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    ret.x = x / mapData.tilewidth;
    ret.y = y / mapData.tileheight;

    return ret;
}

// L08: TODO 6: Load a group of properties from a node and fill a list with it
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
bool Map :: Getdied()
{
    if (died)
    {
        return true;
    }
    return false;
}

bool Map::LoadColisions()
{
    bool ret = true;
    

    ListItem<MapLayer*>* mapLayer;
    mapLayer = mapData.layers.start;

    // L06: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
    
    // iterates the layers in the map
    while (mapLayer != NULL) {
        //Check if the property Draw exist get the value, if it's true draw the lawyer
        
        
            
            if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value == false) {
                //iterate all tiles in a layer


                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        //Get the gid from tile

                        {
                            int gid = mapLayer->data->Get(i, j);

                            //L08: DONE 3: Obtain the tile set using GetTilesetFromTileId
                            //Get the Rect from the tileSetTexture;
                            TileSet* tileSet = GetTilesetFromTileId(gid);
                            SDL_Rect tileRect = tileSet->GetRect(gid);
                            //SDL_Rect tileRect = mapData.tilesets.start->data->GetRect(gid); // (!!) we are using always the first tileset in the list

                            //Get the screen coordinates from the tile coordinates
                            iPoint mapCoord = MapToWorld(i, j);

                            // L06: DONE 9: Complete the draw function
                            /*app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);*/

                            LOG("Firstgid: %d    GID: %d", tileSet->firstgid, gid);

                            if (gid == 49 + 0)
                            {

                                PhysBody* c1 = app->physics->CreateRectangle(mapCoord.x + 16, mapCoord.y + 16, 32, 32, STATIC);
                                c1->ctype = ColliderType::PLATFORM;
                                
                            }
                            else if (gid == 49 + 1 )
                            {
                                PhysBody* c2 = app->physics->CreateRectangle(mapCoord.x + 16, mapCoord.y + 16, 32, 32, STATIC);
                                c2->ctype = ColliderType::DIE;
                            }

                        }



                    }
                }
            }
            
        //if (cambio == 120)
        //{
        //    for (int i = (app->scene->GetPlayer()->position.x) / 32 - 80; i < (app->scene->GetPlayer()->position.x) / 20 - 40; i++) {
        //        for (int j = 0; j < mapData.height; j++) {
        //            //Get the gid from tile

        //            {
        //                int gid = mapLayer->data->Get(i, j);

        //                //L08: DONE 3: Obtain the tile set using GetTilesetFromTileId
        //                //Get the Rect from the tileSetTexture;
        //                TileSet* tileSet = GetTilesetFromTileId(gid);
        //                SDL_Rect tileRect = tileSet->GetRect(gid);
        //                //SDL_Rect tileRect = mapData.tilesets.start->data->GetRect(gid); // (!!) we are using always the first tileset in the list

        //                //Get the screen coordinates from the tile coordinates
        //                iPoint mapCoord = MapToWorld(i, j);

        //                // L06: DONE 9: Complete the draw function
        //                /*app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);*/

        //                LOG("Firstgid: %d    GID: %d", tileSet->firstgid, gid);

        //                if (c1)
        //                {
        //                    b2World* world = app->physics->GetWorld();

        //                    if (world) {
        //                        if (c1->body) {
        //                            world->DestroyBody(c1->body);
        //                            c1->body = nullptr; // Establece el puntero del cuerpo a nullptr para evitar problemas posteriores
        //                        }
        //                    }
        //                    
        //                }

        //            }
        //        }
        //    }
        //}
        
        mapLayer = mapLayer->next;
    }
   /* return ret;*/


    return false;
}

// L08: TODO 7: Implement a method to get the value of a custom property
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

// L13: Create navigationMap map for pathfinding
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
            if (gid == blockedGid) navigationMap[i] = 0;
            else navigationMap[i] = 1;
        }
    }

    *buffer = navigationMap;
    width = mapData.width;
    height = mapData.height;

}

