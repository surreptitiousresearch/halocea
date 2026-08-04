#pragma once
/* _equipment_datum — the equipment-level runtime fields, embedded at offset 0x22C (556) inside the
 * composite equipment_datum object datum. Layout from the database (104 bytes). */

#include "equipment_datum_network_data.h"

typedef struct _equipment_datum
{
    unsigned int    flags;                          /* 0x00 */
    int             ignore_object_index;            /* 0x04 */
    float           detonation_timer;               /* 0x08 */
    float           detonation_timer_delta;         /* 0x0C */
    float           arming_timer;                   /* 0x10 */
    float           arming_timer_delta;             /* 0x14 */
    unsigned __int8 baseline_valid;                 /* 0x18 */
    unsigned __int8 baseline_index;                 /* 0x19 */
    unsigned __int8 message_index;                  /* 0x1A */
    char            _pad1B[1];                       /* 0x1B */
    equipment_datum_network_data baseline;          /* 0x1C (28) */
    unsigned __int8 last_network_data_valid;        /* 0x40 (64) */
    char            _pad41[3];                       /* 0x41 */
    equipment_datum_network_data last_network_data; /* 0x44 (68) */
} _equipment_datum;                                 /* 104 bytes */
