#pragma once
/* metered_panel_definition — a HUD panel that combines a static background icon with a meter overlay
 * (shield / health / auxilary). 240-byte DB layout. The 16-byte "extras" union (DB member ___u2)
 * carries the per-panel-kind gradient thresholds/colors (shield overcharge colors, health min/mid/max
 * cutoffs, or the auxilary min cutoff + flags). $-names kept verbatim (MSVC extension). */

#include "static_hud_element_definition.h"
#include "meter_hud_element_definition.h"

/* DB struct _83712DCF29B41002E04714F8AEBCF031 (16 bytes) — shield arm. */
typedef struct _83712DCF29B41002E04714F8AEBCF031
{
    unsigned int overcharge_min_color;   /* 0x00 */
    unsigned int overcharge_max_color;   /* 0x04 */
    unsigned int overcharge_flash_color; /* 0x08 */
    unsigned int overcharge_empty_color; /* 0x0C */
} _83712DCF29B41002E04714F8AEBCF031;

/* DB struct _E37349842DBDCBD4CBEFAB393C76E40E (16 bytes) — health arm. */
typedef struct _E37349842DBDCBD4CBEFAB393C76E40E
{
    unsigned int mid_color;   /* 0x00 */
    float        max_cutoff;  /* 0x04 */
    float        min_cutoff;  /* 0x08 */
    int          pad;         /* 0x0C */
} _E37349842DBDCBD4CBEFAB393C76E40E;

/* DB struct _80BBBA19DACA070B7642C2C2B2376415 (16 bytes) — auxilary arm. */
typedef struct _80BBBA19DACA070B7642C2C2B2376415
{
    float        min_cutoff;  /* 0x00 */
    unsigned int flags;       /* 0x04 */
    int          pad[2];      /* 0x08 */
} _80BBBA19DACA070B7642C2C2B2376415;

/* DB union _252E9FE088D35182A107D73A30A298F4 (16 bytes) — metered_panel_definition member ___u2. */
typedef union _252E9FE088D35182A107D73A30A298F4
{
    struct _83712DCF29B41002E04714F8AEBCF031 shield_extras; /* 0x00 */
    struct _E37349842DBDCBD4CBEFAB393C76E40E health_extras; /* 0x00 */
    struct _80BBBA19DACA070B7642C2C2B2376415 aux_extras;    /* 0x00 */
} _252E9FE088D35182A107D73A30A298F4;

typedef struct metered_panel_definition
{
    static_hud_element_definition background; /* 0x00  (104) */
    meter_hud_element_definition  meter;      /* 0x68  (104) */
    union _252E9FE088D35182A107D73A30A298F4 ___u2; /* 0xD0 (16) — per-kind extras */
    int                           unused[4];  /* 0xE0 */
} metered_panel_definition;                   /* 0xF0 = 240 bytes */
