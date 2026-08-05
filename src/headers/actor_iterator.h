#pragma once
/* actor_iterator — cursor over the actor_data array, with encounter sub-iteration (28 bytes). */
#include <stdint.h>
#include "data_iterator.h"

typedef struct actor_iterator
{
    data_iterator   encounter_iterator;          /* 0x00 */
    uint8_t iterated_encounterless_list;  /* 0x10 */
    uint8_t active_only;                  /* 0x11 */
    unsigned char _pad0[2]; /* db-verified padding */
    int             index;                        /* 0x14 — current actor index */
    int             next_index;                   /* 0x18 */
} actor_iterator;                                 /* 28 bytes */

#include "headers/actor_datum.h"
extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
