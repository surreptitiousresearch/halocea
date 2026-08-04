#pragma once
/* position_queue — a circular queue of buffered remote-player positions (24 bytes): a simple_circular_queue
 * plus its allocated entry bank. Mirrors action_queue's layout. */

#include "simple_circular_queue.h"

struct position_entry; /* forward decl — pointee reconciled by its own ledger entry */

typedef struct position_queue
{
    simple_circular_queue   queue;       /* 0x00 */
    struct position_entry  *entry_bank;  /* 0x14 */
} position_queue;                        /* 24 bytes */
