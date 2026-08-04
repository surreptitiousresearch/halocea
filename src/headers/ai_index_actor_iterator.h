#pragma once
/* ai_index_actor_iterator — iterates the actors reachable from an AI index (encounter/squad/platoon).
 * 24 bytes, full DB layout. */
#include "encounter_actor_iterator.h"

typedef struct ai_index_actor_iterator
{
    int                      encounter_index; /* 0x0 */
    int                      squad_index;     /* 0x4 */
    int                      platoon_index;   /* 0x8 */
    encounter_actor_iterator iterator;        /* 0xC */
} ai_index_actor_iterator;                    /* 24 bytes */
