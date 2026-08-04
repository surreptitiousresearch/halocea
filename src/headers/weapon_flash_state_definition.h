#pragma once
/* weapon_flash_state_definition — the "flash cutoff" thresholds for a weapon HUD interface. When the live
 * weapon readout crosses one of these cutoffs the corresponding HUD state begins flashing. Layout from the
 * database (44 bytes). */

typedef struct weapon_flash_state_definition
{
    __int16 flags;        /* 0x00 — bit0 = state-flags supplied externally (parent-driven) */
    __int16 pad;          /* 0x02 */
    __int16 total_ammo;   /* 0x04 — low-total-ammo cutoff */
    __int16 loaded_ammo;  /* 0x06 — low-loaded-ammo cutoff */
    __int16 heat;         /* 0x08 — heat percentage cutoff */
    __int16 age;          /* 0x0A — age percentage cutoff */
    int     unused[8];    /* 0x0C */
} weapon_flash_state_definition;  /* 44 bytes */
