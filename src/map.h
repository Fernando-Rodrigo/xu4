/*
 * map.h
 */

#ifndef MAP_H
#define MAP_H

#include <map>
#include <vector>

#include "coords.h"
#include "direction.h"
#include "object.h"
#include "savegame.h"
#include "types.h"
#include "u4file.h"

#define MAP_IS_OOB(mapptr, c) (((c).x) < 0 || ((c).x) >= (static_cast<int>((mapptr)->width)) || ((c).y) < 0 || ((c).y) >= (static_cast<int>((mapptr)->height)) || ((c).z) < 0 || ((c).z) >= (static_cast<int>((mapptr)->levels)))

class AnnotationMgr;
class Map;
class Object;
class Person;
class Creature;
class Tileset;
struct Portal;
struct _Dungeon;

typedef std::vector<Portal *> PortalList;
//typedef std::list<int> CompressedChunkList;
typedef std::vector<TileId> MapData;

/* flags */
#define SHOW_AVATAR (1 << 0)
#define NO_LINE_OF_SIGHT (1 << 1)
#define FIRST_PERSON (1 << 2)

/* mapTileAt flags */
#define WITHOUT_OBJECTS     0
#define WITH_GROUND_OBJECTS 1
#define WITH_OBJECTS        2

/**
 * MapCoords class
 */
class MapCoords : public Coords {
public:
    MapCoords(int initx = 0, int inity = 0, int initz = 0) : Coords(initx, inity, initz) {}
    MapCoords(const Coords &a) : Coords(a.x, a.y, a.z) {}

    MapCoords &operator=(const Coords &a) { x = a.x; y = a.y; z = a.z; return *this; }
    bool operator==(const MapCoords &a) const;
    bool operator!=(const MapCoords &a) const;
    bool operator<(const MapCoords &a)  const;

    MapCoords &wrap(const class Map *map);
    MapCoords &putInBounds(const class Map *map);
    MapCoords &move(Direction d, const class Map *map = NULL);
    MapCoords &move(int dx, int dy, const class Map *map = NULL);
    int getRelativeDirection(const MapCoords &c, const class Map *map = NULL) const;
    Direction pathTo(const MapCoords &c, int valid_dirs = MASK_DIR_ALL, bool towards = true, const class Map *map = NULL) const;
    Direction pathAway(const MapCoords &c, int valid_dirs = MASK_DIR_ALL) const;
    int movementDistance(const MapCoords &c, const class Map *map = NULL) const;
    int distance(const MapCoords &c, const class Map *map = NULL) const;

    static MapCoords nowhere;
};

/**
 * Map class
 */
class Map {
public:
    enum Type {
        WORLD,
        CITY,
        SHRINE,
        COMBAT,
        DUNGEON
    };

    enum BorderBehavior {
        BORDER_WRAP,
        BORDER_EXIT2PARENT,
        BORDER_FIXED
    };

    Map();
    virtual ~Map();

    // Member functions
    virtual const char* getName() const;

    void queryBlocking(int x, int y, uint8_t* blocking, int bw, int bh) const;
    void queryVisible(const Coords &coords, int radius,
                      void (*func)(const Coords*, VisualId, void*),
                      void* user) const;
    const Object* objectAt(const Coords &coords) const;
    Object* objectAt(const Coords &coords) {
        return (Object*) static_cast<const Map*>(this)->objectAt(coords);
    }
    const Portal *portalAt(const Coords &coords, int actionFlags);
    TileId getTileFromData(const Coords &coords) const;
    const Tile* tileTypeAt(const Coords &coords, int withObjects) const;
    void setTileAt(const Coords &coords, TileId tid);
    bool isWorldMap() const;
    bool isEnclosed(const Coords &party);
    class Creature *addCreature(const class Creature *m, Coords coords);
    class Object *addObject(MapTile tile, MapTile prevTile, Coords coords);
    class Object *addObject(Object *obj, Coords coords);
    void removeObject(const class Object *rem, bool deleteObject = true);
    ObjectDeque::iterator removeObject(ObjectDeque::iterator rem, bool deleteObject = true);
    void clearObjects();
    class Creature *moveObjects(MapCoords avatar);
    int getNumberOfCreatures();
    int getValidMoves(const MapCoords& from, MapTile transport);
    bool move(Object *obj, Direction d);
    void alertGuards();
    const MapCoords &getLabel(Symbol name) const;
    const char* labelAt(const MapCoords&) const;

    // u4dos compatibility
    void fillMonsterTable(SaveGameMonsterRecord* table) const;
    void fillMonsterTableDungeon(SaveGameMonsterRecord* table) const;

public:
    StringId        fname;
    MapId           id;
    uint8_t         type;
    uint8_t         border_behavior;    // BorderBehavior
    uint8_t         _pad;
    uint16_t        width,
                    height,
                    levels;
    uint16_t        chunk_width,
                    chunk_height;
    uint16_t        flags;
    uint16_t        music;
    unsigned int    offset;

    //CompressedChunkList compressed_chunks;
    PortalList      portals;
    AnnotationMgr  *annotations;
    MapData         data;
#ifdef USE_GL
    uint8_t* chunks;
#endif
    ObjectDeque     objects;
    std::map<Symbol, MapCoords> labels;
    const Tileset  *tileset;

private:
    // disallow map copying: all maps should be created and accessed
    // through the MapMgr
    Map(const Map &map);
    Map &operator=(const Map &map);

    void findWalkability(Coords coords, int *path_data);
};

inline bool isCity(const Map* map)      { return map->type == Map::CITY; }
inline bool isCombatMap(const Map* map) { return map->type == Map::COMBAT; }
inline bool isDungeon(const Map* map)   { return map->type == Map::DUNGEON; }

#endif
