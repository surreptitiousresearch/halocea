#pragma once
/* scenario_machine_datum — one scenario 'machines' block element (64 bytes, DB layout). */

#include "scenario_object_datum.h"
#include "scenario_device_datum.h"

typedef struct scenario_machine_datum
{
    scenario_object_datum object;    /* 0x00 */
    scenario_device_datum device;    /* 0x28 */
    unsigned int          flags;     /* 0x30 — bit0..bit3 forwarded into the machine object's device flags */
    unsigned int          unused[3]; /* 0x34 */
} scenario_machine_datum;            /* 64 bytes */
