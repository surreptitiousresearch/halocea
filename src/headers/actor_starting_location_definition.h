#pragma once
/* actor_starting_location_definition — a spawn slot passed to actor_place (28 bytes). Full DB layout. */
#include "real_point3d.h"

typedef struct actor_starting_location_definition
{
    real_point3d    position;             /* 0x00 */
    float           facing;               /* 0x0C */
    __int16         cluster_index;         /* 0x10 */
    char            sequence_id;           /* 0x12 */
    unsigned __int8 flags;                 /* 0x13 */
    __int16         default_state;         /* 0x14 */
    __int16         initial_state;         /* 0x16 */
    __int16         actor_palette_index;   /* 0x18 */
    __int16         command_list_index;    /* 0x1A */
} actor_starting_location_definition;      /* 28 bytes */
