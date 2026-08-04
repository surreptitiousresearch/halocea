#pragma once
/* vehicle_update_queue — a circular queue of buffered remote-player vehicle updates (24 bytes, DB layout):
 * a simple_circular_queue plus its allocated entry bank. */

#include "simple_circular_queue.h"
#include "vehicle_update_entry.h"

typedef struct vehicle_update_queue
{
    simple_circular_queue queue;       /* 0x00 */
    vehicle_update_entry *entry_bank;  /* 0x14 */
} vehicle_update_queue;                /* 24 bytes */
