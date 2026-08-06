#pragma once
/* encounter_actor_iterator — cursor over the actors in one encounter (12 bytes), used with
 * encounter_actor_iterator_new/next. The current actor index sits at +0x4. */
#include "actor_datum.h"

typedef struct encounter_actor_iterator
{
    int encounter_index; /* 0x0 — encounter being iterated (-1 = encounterless list) */
    int index;           /* 0x4 — current actor index */
    int next_index;      /* 0x8 — next actor index to visit (meta.next_actor_index chain) */
} encounter_actor_iterator;  /* 12 bytes */

#ifdef __cplusplus
extern "C" {
#endif
extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
#ifdef __cplusplus
}
#endif
