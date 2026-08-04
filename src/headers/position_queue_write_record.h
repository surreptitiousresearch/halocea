#pragma once
/* position_queue_write_record — the 20-byte record packed by position_queue_write and unpacked by
 * position_queue_read (entry[0]=id, entry[1]=age threshold, entry[2..4]=position floats). Structurally
 * identical to position_queue.h's forward-declared position_entry; kept as the write-side local record. */

#include "real_point3d.h"

typedef struct position_queue_write_record
{
    int          client_update_id;      /* 0x00 */
    int          updates_until_useful;  /* 0x04 */
    real_point3d position;              /* 0x08 */
} position_queue_write_record;          /* 20 bytes */
