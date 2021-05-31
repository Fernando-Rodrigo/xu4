/*
 * config.h
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

class ImageSet;
struct Layout;
struct Armor;
struct Weapon;
class Coords;
class Creature;
class Map;
struct TileRule;
class Tileset;
class TileAnimSet;
class ConfigElement;
struct RGBA;
struct UltimaSaveIds;

/**
 * Config is a singleton data provider interface which hides the storage
 * format of the game configuration.
 * It provides data in a form easily digestible for game engine modules.
 */
class Config {
public:
    virtual ~Config();

    //const char** getGames();
    //void setGame(const char* name);

    const char* symbolName( Symbol s ) const;
    Symbol intern( const char* name );
    void internSymbols( Symbol* table, uint16_t count, const char* name );
    const char* confString( StringId id ) const;

    // Primary configurable elements.
    const RGBA* egaPalette();
    const Layout* layouts( uint32_t* plen ) const;
    const char* musicFile( uint32_t id );
    const char* soundFile( uint32_t id );
    const char** schemeNames();
    ImageSet* newScheme( uint32_t id );
    TileAnimSet* newTileAnims(const char* name) const;
    const Armor*  armor( uint32_t id );
    const Weapon* weapon( uint32_t id );
    int armorType( const char* name );
    int weaponType( const char* name );
    const Creature* creature( uint32_t id ) const;
    const Creature* const* creatureTable( uint32_t* plen ) const;
    const TileRule* tileRule( Symbol name ) const;
    const Tileset* tileset() const;
    const UltimaSaveIds* usaveIds() const;
    Map* map(uint32_t id);
    Map* restoreMap(uint32_t id);
    const Coords* moongateCoords(int phase) const;

protected:
    void* backend;
};

extern Config* configInit();
extern void    configFree(Config*);

#define foreach(it, col)    for(it = col.begin(); it != col.end(); ++it)

#endif /* CONFIG_H */
