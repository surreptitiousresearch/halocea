/* player_set_action_result @0x836A9810 — records the "action" (e.g. enter-vehicle / pick-up prompt)
 * currently offered to a player, but only when the new action outranks the one already stored. An
 * action_result of _player_action_result_flip_vehicle (11) always wins. When the new result ties the stored one, the closer action object
 * (to the player's unit) wins. Otherwise a signed/priority comparison of the result codes decides. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/player_action_result.h"
#include "headers/blam_data_globals.h"

extern float       __fsqrts(float x);

void player_set_action_result(int player_index, int16_t action_result, int action_object_index,
                              int16_t action_seat_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    char accept;

    if (action_result == _player_action_result_flip_vehicle)
    {
        accept = 1;
    }
    else
    {
        unsigned int current_result = player->action_result;
        if (current_result == (unsigned int)action_result)
        {
            object_datum *new_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, action_object_index)->datum;
            object_datum *current_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                   player->action_object_index)->datum;
            object_datum *unit =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;

            float cur_dx = current_object->object.position.n[0] - unit->object.position.n[0];
            float cur_dy = current_object->object.position.n[1] - unit->object.position.n[1];
            float cur_dz = current_object->object.position.n[2] - unit->object.position.n[2];
            float new_dx = new_object->object.position.n[0] - unit->object.position.n[0];
            float new_dy = new_object->object.position.n[1] - unit->object.position.n[1];
            float new_dz = new_object->object.position.n[2] - unit->object.position.n[2];

            float current_distance = __fsqrts(cur_dy * cur_dy + (cur_dx * cur_dx + cur_dz * cur_dz));
            float new_distance     = __fsqrts(new_dy * new_dy + (new_dx * new_dx + new_dz * new_dz));
            accept = current_distance > new_distance;
        }
        else
        {
            /* branchless-signed XOR idiom == signed (action_result > current_result): new result outranks stored */
            accept = action_result > (int)current_result;
        }
    }

    if (accept)
    {
        player->action_result = action_result;
        player->action_object_index = action_object_index;
        player->action_seat_index = action_seat_index;
    }
}
