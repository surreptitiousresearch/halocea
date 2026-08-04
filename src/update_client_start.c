/* update_client_start @0x836F5ECC — (re)initialize the client update queues at game start: validate and
 * clear the queue array, then allocate one queue datum per existing player.
 *
 * DEVIATION: the decompiler types the result as void* (the trailing null iterator result); ignored. */

#include "headers/update_client_globals.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);
extern void data_delete_all(data_array *data);
extern int datum_new_at_index(data_array *data, int index);

void *update_client_start(void)
{
    data_make_valid(update_client_globals.queues);
    data_delete_all(update_client_globals.queues);

    data_iterator iterator;
    void *player;
    data_iterator_new(&iterator, player_data);
    for ( player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
        datum_new_at_index(update_client_globals.queues, iterator.index);
    return player;
}
