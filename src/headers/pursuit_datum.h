#pragma once
#include <stdint.h>
/* pursuit_datum — one shared pursuit record (40 bytes), chained per-encounter via
 * encounter_datum.first_pursuit_index. Full DB types_members layout. */

typedef struct pursuit_datum
{
    int16_t identifier;              /* 0x00 */
    int16_t firing_position_index;   /* 0x02 */
    int     last_examined_time;      /* 0x04 */
    int16_t actor_count;             /* 0x08 */
    int16_t next_actor_index_index;  /* 0x0A */
    int     actor_indices[6];        /* 0x0C */
    int     next_pursuit_index;      /* 0x24 */
} pursuit_datum;                     /* 40 bytes */
