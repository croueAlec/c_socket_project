#include "project.h"
#include "game.h"

const item weapon_templates[] = {
	{SWORD,		"Soren's sword",	 {0, 4, 1, 0, 0, 0, 3} },
	{STAFF,		"Charlotte's staff", {-2, 2, 1, 0, 6, 4, 5}},
	{item_NONE, "",					 {0}				   }
};

const character character_templates[] = {
	{SOREN,		   "Soren",		{6, 5, 5, 8, 0, 0, 9}, NULL, SWORDSMAN },
	{CHARLOTTE,	   "Charlotte", {4, 1, 2, 4, 6, 6, 5}, NULL, MAGICIAN  },
	{BASIL,		   "Basil",		{5, 3, 2, 5, 9, 1, 8}, NULL, MONK	   },
	{char_INVALID, "",			{0},				   NULL, class_NONE}
};

const enemy enemies[] = {
	{RABBIT, {4, 1, 1, 2, 0, 0, 2},	   0},
	{WOLF,	 {10, 8, 4, 5, 2, 1, 7},   0},
	{BEAR,	 {2, 10, 10, 15, 0, 0, 2}, 0}
};
game_cfg game_config = {0};

void init_game_config()
{
	uint8_t count = 0;

	while (character_templates[count].id != char_INVALID) {
		count++;
	}
	game_config.number_of_characters = count;

	count = 0;
	while (weapon_templates[count].type != item_NONE) {
		count++;
	}
	game_config.number_of_weapons = count;
}
