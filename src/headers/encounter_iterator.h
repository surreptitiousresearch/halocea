#pragma once
/* encounter_iterator — cursor over the encounter_data array (24 bytes). */
#include <stdint.h>
#include "data_iterator.h"

typedef struct encounter_iterator
{
    data_iterator   encounter_iterator;  /* 0x00 */
    int             index;                /* 0x10 — current encounter index */
    unsigned __int8 active_only;          /* 0x14 */
} encounter_iterator;                     /* 24 bytes */

#include "headers/encounter_datum.h"
extern void encounter_iterator_new(encounter_iterator *iterator, uint8_t active_only); // boundary — Blam C export
extern encounter_datum *encounter_iterator_next(encounter_iterator *iterator);
