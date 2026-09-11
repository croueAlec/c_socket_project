#include "project.h"
#include "game.h"

static int calculate_damage(const statistics assailant, const statistics defender, STATS attack_type)
{
	STATS defense_type = (attack_type == ATK) ? DEF : MDEF;
	printf("defender def : %hhu\n", defender[defense_type]);
	printf("assailant def : %hhu\n", assailant[attack_type]);

	return (defender[defense_type] - assailant[attack_type]);
}

/**
 * @brief Returns true on hit, this is the dodge mechanic
 *
 * @param accuracy_stat entity accuracy + item (if applicable)
 * @param speed_stat entity speed + item (if applicable)
 */
static bool is_hit(uint8_t accuracy_stat, uint8_t speed_stat)
{
	return accuracy_stat > speed_stat;
}

static int take_damage(const character *assailant, entity_type defender_type, void *defender, STATS attack_type)
{
	enemy *defender_enemy = defender;

	int damage = calculate_damage(assailant->stats, defender_enemy->stats, attack_type);
	printf("calculated damage : %d\n", damage);
	if (defender_enemy->health_points <= damage) {
		defender_enemy->health_points = 0;
	} else {
		defender_enemy->health_points += damage;
	}

	(void)defender_type;
	return damage;
}

static void character_attack(const character *assailant, entity_type defender_type, void *defender, STATS attack_type)
{
	enemy *defender_enemy = defender; // TODO: implement character on character attack

	if (is_hit(assailant->stats[ACC], defender_enemy->stats[SPD]) == true) {
		printf("entity was hit and lost %d hp\n", take_damage(assailant, defender_type, defender_enemy, attack_type));
	} else {
		printf("entity dodged\n");
	}

	(void)defender_type;
}

static void enemy_attack(const enemy *assailant, entity_type defender_type, void *defender, STATS attack_type)
{
	character *defender_character = defender;

	if (is_hit(assailant->stats[ACC], defender_character->stats[SPD]) == true) {
		uint8_t damage = calculate_damage(assailant->stats, defender_character->stats, attack_type);
		printf("character was hit and list %hhu hp\n", damage);
	} else {
		printf("character dodged\n");
	}

	(void)defender_type;
}

void attack(entity_type assailant_type, void *assailant, entity_type defender_type, void *defender, STATS attack_type)
{
	if (assailant_type == entity_CHARACTER) {
		character_attack(assailant, defender_type, defender, attack_type);
	} else if (assailant_type == entity_ENEMY) {
		enemy_attack(assailant, defender_type, defender, attack_type);
	}
}
