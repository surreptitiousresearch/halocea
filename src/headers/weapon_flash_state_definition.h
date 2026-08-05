#pragma once
#include <stdint.h>
/* weapon_flash_state_definition — the "flash cutoff" thresholds for a weapon HUD interface. When the live
 * weapon readout crosses one of these cutoffs the corresponding HUD state begins flashing. Layout from the
 * database (44 bytes). */

typedef struct weapon_flash_state_definition
{
    int16_t flags;        /* 0x00 — bit0 = state-flags supplied externally (parent-driven) */
    int16_t pad;          /* 0x02 */
    int16_t total_ammo;   /* 0x04 — low-total-ammo cutoff */
    int16_t loaded_ammo;  /* 0x06 — low-loaded-ammo cutoff */
    int16_t heat;         /* 0x08 — heat percentage cutoff */
    int16_t age;          /* 0x0A — age percentage cutoff */
    int     unused[8];    /* 0x0C */
} weapon_flash_state_definition;  /* 44 bytes */
