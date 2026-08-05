/* update_client_start @0x836F5ECC — (re)initialize the client update queues at game start: validate and
 * clear the queue array, then allocate one queue datum per existing player.
 *
 * DEVIATION: the decompiler typed the result as void* (the trailing null iterator result). Attested void:
 * all 3 sampled callers (game_time_start, update_server_start, update_queues_reset_and_fill_with_lies)
 * ignore r3, and the only r3 at the blr is threaded out of data_iterator_next. */

#include "headers/update_client_globals.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);
extern void data_delete_all(data_array *data);
extern int datum_new_at_index(data_array *data, int index);

void update_client_start(void)
{
    data_make_valid(update_client_globals.queues);
    data_delete_all(update_client_globals.queues);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    while ( data_iterator_next(&iterator) )
        datum_new_at_index(update_client_globals.queues, iterator.index);
}
