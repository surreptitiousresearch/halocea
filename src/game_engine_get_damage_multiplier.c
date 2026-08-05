/* game_engine_get_damage_multiplier @0x8374DDE0 — compute the multiplayer damage multiplier for an
 * attacker→defender pair: the inverse of the variant health modifier (clamped 0.25..4.0), times 1.5 if
 * the attacker has the damage-boost trait, times 0.5 if the defender has the damage-resistance trait
 * (trait 3). Returns 1.0 outside a running game engine. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/game_engine_trait.h"


float game_engine_get_damage_multiplier(int attacker_index, int defender_index)
{
    float multiplier = 1.0f;

    if ( game_engine )
    {
        float health = global_variant.universal_variant.health;
        if ( health < 0.25f )
            health = 0.25f;
        else if ( health > 4.0f )
            health = 4.0f;
        multiplier = 1.0f / health;
    }

    if ( attacker_index == -1 || defender_index == -1 || !game_engine )
        return multiplier;

    /* DEVIATION: decompiler dropped the args of the attacker test_trait call, rendering it
     * `(int(*)(void))(...)()`. disasm_range(0x8374DE60,0x8374DE84) keeps r3=attacker_index and sets
     * r4=2, so the true call is test_trait(attacker_index, _game_trait_extra_damage). */
    char attacker_boost = 0;
    if ( game_engine->game_engine_test_trait )
        attacker_boost = game_engine->game_engine_test_trait(attacker_index, _game_trait_extra_damage);
    if ( attacker_boost )
        multiplier = multiplier * 1.5f;

    uint8_t defender_resist = 0;
    if ( game_engine && game_engine->game_engine_test_trait )
        defender_resist = game_engine->game_engine_test_trait(defender_index, _game_trait_damage_resistant);
    if ( defender_resist )
        multiplier = multiplier * 0.5f;

    return multiplier;
}
