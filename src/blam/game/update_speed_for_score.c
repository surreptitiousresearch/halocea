/* update_speed_for_score @0x838154D8 — adjust two players' movement speed_multiplier after a kill:
 * unless the CTF variant resets on capture, gradually decay the killer's speed toward 1.0 (an extra step
 * once it's still >=1.0 after the first decrement, floor of 0.9 otherwise); unless the CTF variant is
 * "assault", gradually ramp the dead player's speed back up toward 1.5 (an extra step once it's still
 * <=1.0 after the first increment). */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);

void update_speed_for_score(int dead_player_index, int killing_player_index)
{
    player_datum *killer = DATA_ARRAY_ELEMENT(player_data, player_datum, killing_player_index);
    player_datum *dead = DATA_ARRAY_ELEMENT(player_data, player_datum, dead_player_index);

    if ( !game_engine_get_variant()->game_engine_variant.ctf.reset_on_capture )
    {
        float speed = killer->multiplayer.speed_multiplier - 0.02f;
        killer->multiplayer.speed_multiplier = speed;

        if ( speed >= 1.0f )
        {
            speed = speed - 0.15000001f;
            if ( speed <= 1.0f )
                speed = 1.0f;
            killer->multiplayer.speed_multiplier = speed;
        }

        speed = killer->multiplayer.speed_multiplier;
        if ( speed <= 0.89999998f )
            speed = 0.89999998f;
        killer->multiplayer.speed_multiplier = speed;
    }

    if ( !game_engine_get_variant()->game_engine_variant.ctf.assault )
    {
        float speed = dead->multiplayer.speed_multiplier + 0.1f;
        dead->multiplayer.speed_multiplier = speed;

        if ( speed <= 1.0f )
        {
            speed = speed + 0.1f;
            if ( speed > 1.0f )
                speed = 1.0f;
            dead->multiplayer.speed_multiplier = speed;
        }

        speed = dead->multiplayer.speed_multiplier;
        if ( speed > 1.5f )
            speed = 1.5f;
        dead->multiplayer.speed_multiplier = speed;
    }
}
