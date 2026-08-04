#pragma once
/* scenario_device_group — a scenario-authored device group definition (DB types_members-confirmed, 52 bytes). */

typedef struct scenario_device_group
{
    char         name[32];       /* 0x00 */
    float        initial_value;  /* 0x20 */
    unsigned int flags;          /* 0x24 */
    unsigned int unused[3];      /* 0x28 */
} scenario_device_group; /* 52 bytes */
