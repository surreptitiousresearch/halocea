#pragma once
/* rendered_particle_datum — one particle selected for rendering this frame (8 bytes, DB layout). */

typedef struct rendered_particle_datum
{
    __int16          particle_index;                /* 0x00 */
    __int16          definition_index;               /* 0x02 */
    __int16          cluster_index;                   /* 0x04 */
    unsigned __int8  attached_to_first_person_weapon;  /* 0x06 */
} rendered_particle_datum;                             /* 8 bytes */
