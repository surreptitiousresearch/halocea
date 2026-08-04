/* game_engine_variant_cleanup @0x83749EB8 — sanitize a loaded game variant in place: clamp the engine
 * index to [1,5], normalize boolean flags to 0/1, clamp the respawn/penalty/lives counts to non-negative,
 * the health multiplier to [0.25, 4.0], the weapon set to [0,13], and the vehicle set's top-nibble index
 * to <=8. CTF (engine 1) forces teams on and clamps single-flag time; the slayer path (engine 2) just
 * normalizes its shared boolean flags.
 *
 * The "x < 0 ? 0 : x" non-negative clamps reproduce the decompiler's bit-twiddle idiom
 * ((x==0)+(x>>31)-1)&x and the __CFADD__(x,0x80000000) carry test (both == max(0, (int)x)). */

#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"

void game_engine_variant_cleanup(game_variant *variant)
{
    int mode;
    if (variant->game_engine_index >= game_engine_ctf)
        mode = variant->game_engine_index <= game_engine_race ? variant->game_engine_index : 5;
    else
        mode = 1;
    variant->game_engine_index = mode;

    universal_variant *u = &variant->universal_variant;
    u->teams = u->teams != 0;
    u->odd_man_out = u->odd_man_out != 0;
    u->respawn_time_growth = u->respawn_time_growth < 0 ? 0 : u->respawn_time_growth;
    u->respawn_time = u->respawn_time < 0 ? 0 : u->respawn_time;
    u->suicide_penalty = u->suicide_penalty < 0 ? 0 : u->suicide_penalty;
    u->lives = u->lives < 0 ? 0 : u->lives;

    if (u->health < 0.25)
        u->health = 0.25;
    else if (u->health > 4.0)
        u->health = 4.0;

    if (u->weapon_set < 0)
        u->weapon_set = _game_engine_weapons_normal;
    else if (u->weapon_set > NUMBER_OF_GAME_ENGINE_WEAPON_SETS - 1)
        u->weapon_set = _game_engine_weapons_no_grenades;

    /* clamp the vehicle-set preset (bits 28-31) to the last valid value; the bitfield write
     * preserves the per-type count fields (was: (idx<<28) | (value & 0xFFFFFFF)). */
    if (u->vehicle_set.__s1.preset > _game_engine_vehicles_custom)
        u->vehicle_set.__s1.preset = _game_engine_vehicles_custom;

    if (mode == 1)
    {
        ctf_variant *ctf = &variant->game_engine_variant.ctf;
        u->teams = 1;
        ctf->assault = ctf->assault != 0;
        ctf->reset_on_capture = ctf->reset_on_capture != 0;
        ctf->flag_must_reset = ctf->flag_must_reset != 0;
        ctf->flag_at_home_to_score = ctf->flag_at_home_to_score != 0;
        ctf->single_flag_time = ctf->single_flag_time < 0 ? 0 : ctf->single_flag_time;
    }
    else if (mode == 2)
    {
        ctf_variant *ctf = &variant->game_engine_variant.ctf;
        ctf->assault = ctf->assault != 0;
        ctf->reset_on_capture = ctf->reset_on_capture != 0;
        ctf->flag_must_reset = ctf->flag_must_reset != 0;
    }
}
