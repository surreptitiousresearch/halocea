/* update_server_start @0x836F6450 — (re)initialize the server update queues at game start: validate and
 * clear the queue array, allocate and initialize one action queue per existing player, then start the
 * client-side queues too. */

#include <stdint.h>
#include "headers/update_server_globals.h"
#include "headers/action_queue.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/update_server_queue_datum.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);
extern void data_delete_all(data_array *data);
extern int datum_new_at_index(data_array *data, int index);
extern void action_queue_initialize(action_queue *queue);
extern void update_client_start(void);

/* attested: callers (4/4) ignore r3; the trailing update_client_start return is threaded residue -> void */
void update_server_start(void)
{
    data_make_valid(update_server_globals.queues);
    data_delete_all(update_server_globals.queues);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    while ( data_iterator_next(&iterator) )
    {
        uint16_t queue_index = datum_new_at_index(update_server_globals.queues, iterator.index); /* clrlwi 16: absolute-index part */
        action_queue_initialize(&DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, queue_index)->queue);
    }
    update_client_start();
}
