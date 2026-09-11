#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "project.h"
#include "game.h"

void init_player(player *pl, const char *player_name, const character *selected_character, item *weapon, const item *selected_weapon)
{
	memcpy(pl->name, player_name, strlen(player_name));
	memcpy(&pl->character, selected_character, sizeof(character));
	memcpy(weapon, selected_weapon, sizeof(item));
	pl->character.weapon = weapon;
	pl->health_points = selected_character->stats[HP];
}

void init_mob(enemy *mob)
{
	mob->health_points = mob->stats[HP];
}

void print_stats(const statistics stats)
{
	printf("\nStatistics :\n");
	printf("Speed : %hhu\n", stats[SPD]);
	printf("Attack : %hhu\n", stats[ATK]);
	printf("Defense : %hhu\n", stats[DEF]);
	printf("Health Points : %hhu\n", stats[HP]);
	printf("Magic Attack : %hhu\n", stats[MATK]);
	printf("Magic Defense : %hhu\n", stats[MDEF]);
	printf("Accuracy : %hhu\n\n", stats[ACC]);
}

void introduce_player(player *pl)
{
	printf("Player name : %s\n", pl->name);
	printf("Selected character : %s\n", pl->character.name);
	printf("Selected weapon : %s\n", pl->character.weapon->name);
	print_stats(pl->character.stats);
}

void introduce_mob(enemy *mob)
{
	printf("mob is wolf\n");
	print_stats(mob->stats);
}

void combat(player *pl, enemy *mob)
{
	for (size_t i = 0; i < 50; i++) {
		printf("%zu - %s (%s) attacked wolf\n", i, pl->character.name, pl->name);
		attack(entity_CHARACTER, &pl->character, entity_ENEMY, mob, ATK);
		printf("current hps player : %hhu, mob : %hhu\n\n", pl->health_points, mob->health_points);

		if (mob->health_points == 0) {
			printf("enemy died in %zu rounds\n", i + 1);
			break;
		}
	}
}

int main(int argc, const char *argv[])
{
	player me = {0};
	item   sword = {0};

	enemy wolf = {0};
	memcpy(&wolf, &enemies[WOLF], sizeof(enemy));

	init_player(&me, "Alec", &character_templates[SOREN], &sword, &weapon_templates[SWORD]);
	init_mob(&wolf);

	me.character.stats[ACC] = 55;

	introduce_player(&me);
	introduce_mob(&wolf);

	printf("\nPlaying :\n");
	combat(&me, &wolf);

	(void)argc;
	(void)argv;
	return 0;
}
