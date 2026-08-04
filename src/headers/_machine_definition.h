#pragma once
/* _machine_definition — the machine-level payload ('mach' tag, 148 bytes) at offset 0x290 (656) inside
 * the composite machine_definition, after _object_definition (0x00) and _device_definition (0x17C).
 * Layout DB-verified against type '_machine_definition' (2026-07-14). */

typedef struct _machine_definition
{
    __int16      type;                     /* 0x00 — enum machine_type (machine_type.h) */
    unsigned __int16 flags;                /* 0x02 — 0x4 = elevator */
    float        door_open_time;           /* 0x04 */
    unsigned int unused1[20];              /* 0x08 */
    __int16      collision_response;       /* 0x58 (88) */
    __int16      elevator_node_index;      /* 0x5A (90) */
    unsigned int unused2[13];              /* 0x5C (92) */
    int          runtime_door_open_ticks;  /* 0x90 (144) */
} _machine_definition;                     /* 148 bytes */
