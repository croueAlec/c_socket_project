#ifndef GAME_H
#define GAME_H

#include "project.h"
// #include "_typedefs.h"

#define MAX_NAME_LENGTH 32

/* Generic */

typedef struct game_config
{
	uint8_t number_of_characters;
	uint8_t number_of_weapons;
	uint8_t number_of_enemies;
} game_cfg;

typedef enum entity_type
{
	entity_CHARACTER = 0,
	entity_ENEMY = 1,
} entity_type;

typedef enum STATS
{
	SPD = 0,
	ATK = 1,
	DEF = 2,
	HP = 3,
	MATK = 4,
	MDEF = 5,
	ACC = 6,
} STATS;

/**
 * @brief
 * Speed (SPD) --
 * Attack (ATK) --
 * Defense (DEF) --
 * Health (HP) --
 * Magic Attack (MATK) --
 * Magic Defense (MDEF) --
 * Accuracy (ACC)
 */
typedef uint8_t statistics[7];

/* Classes */

typedef enum CLASS
{
	SWORDSMAN = 0,
	MAGICIAN = 1,
	MONK = 2,
	class_NONE = 3,
} CLASS;

/* Items */

typedef enum ITEM_TYPE
{
	item_MELEE = 0b10000000,
	item_RANGED = 0b01000000,
	SWORD = 0b0001 | item_MELEE,
	STAFF = 0b0010 | item_RANGED,
	item_CUSTOM = 0b0100,
	item_NONE = 3,
} ITEM_TYPE;

typedef struct item
{
	const ITEM_TYPE	 type;
	char			 name[MAX_NAME_LENGTH + 1];
	const statistics stats;
} item;

/* Enemies */

typedef enum ENEMY_TYPE
{
	RABBIT = 0,
	WOLF = 1,
	BEAR = 2,
} ENEMY_TYPE;

typedef struct enemy
{
	const ENEMY_TYPE type;
	const statistics stats;
	uint8_t			 health_points;
} enemy;

/* Special Skills */

/*
 TODO: Implement special skills
 tenacity : health falls to 1 when hit should have killed
 aggressive : chance to attack twice
 focus : chance to increase speed
 */

/* Characters */

typedef enum CHARACTER_ID
{
	SOREN = 0,
	CHARLOTTE = 1,
	BASIL = 2,
	char_CUSTOM = 3,
	char_INVALID = 4,
} CHARACTER_ID;

typedef struct character
{
	const CHARACTER_ID id;
	char			   name[MAX_NAME_LENGTH + 1];
	statistics		   stats;
	const item		  *weapon;
	const CLASS class;
} character;

/* Player */

typedef struct player
{
	const uint8_t id;
	char		  name[MAX_NAME_LENGTH + 1];
	character	  character;
	uint8_t		  health_points;
	size_t		  xp; /* xp required to level up */
	uint8_t		  level;
} player;

/* Externs */

extern const item	   weapon_templates[];
extern const character character_templates[];
extern const enemy	   enemies[];

/* Function */

void attack(entity_type assailant_type, void *assailant, entity_type defender_type, void *defender, STATS attack_type);

#endif
