#pragma once
#include <stdint.h>
/* rendered_particle_datum — one particle selected for rendering this frame (8 bytes, DB layout). */

typedef struct rendered_particle_datum
{
    int16_t          particle_index;                /* 0x00 */
    int16_t          definition_index;               /* 0x02 */
    int16_t          cluster_index;                   /* 0x04 */
    uint8_t  attached_to_first_person_weapon;  /* 0x06 */
} rendered_particle_datum;                             /* 8 bytes */
