/* find_unused_local_player_index @0x836A7BD8 — picks a local player slot (0..3) not yet claimed by an
 * existing player. It first prefers a slot whose gamepad is plugged in but has no player; failing that,
 * any slot 0..3 with no player. Returns -1 if every slot is taken. Each player_data record stores its
 * owning local/controller index at +2. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

static int local_index_in_use(int local_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *player = data_iterator_next(&iterator);
         player;
         player = data_iterator_next(&iterator))
    {
        if (player->local_player_index == local_index)
            return 1;
    }
    return 0;
}

int16_t find_unused_local_player_index(void)
{
    int result = -1;

    for (int local_index = 0; local_index < 4; ++local_index)
    {
        if (input_has_gamepad(local_index) && !local_index_in_use(local_index))
            return local_index;
    }

    for (int local_index = 0; local_index < 4; ++local_index)
    {
        if (!local_index_in_use(local_index))
            return local_index;
    }

    return result;
}
