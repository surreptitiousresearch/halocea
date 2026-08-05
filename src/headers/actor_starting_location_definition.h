#pragma once
/* actor_starting_location_definition — a spawn slot passed to actor_place (28 bytes). Full DB layout. */
#include <stdint.h>
#include "real_point3d.h"

typedef struct actor_starting_location_definition
{
    real_point3d    position;             /* 0x00 */
    float           facing;               /* 0x0C */
    int16_t         cluster_index;         /* 0x10 */
    char            sequence_id;           /* 0x12 */
    uint8_t flags;                 /* 0x13 */
    int16_t         default_state;         /* 0x14 */
    int16_t         initial_state;         /* 0x16 */
    int16_t         actor_palette_index;   /* 0x18 */
    int16_t         command_list_index;    /* 0x1A */
} actor_starting_location_definition;      /* 28 bytes */
