#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* hud_state_message — the runtime state-message payload embedded in each hud_messaging_datum
 * (556 bytes). Holds the assembled unicode text, the per-element resolved info slots, a back
 * pointer to the source definition, and validity/flags. Layout from the database. */

#include "state_info.h"
#include "hud_state_message_definition.h"

/* The text-arm struct _BECA036B426CABFAF46B8904C2CEA3F4 (4 bytes) comes from state_info.h
 * (included above) — it was previously redefined here, which is a C2011 in C. */

/* DB anonymous union _F584AD1D4110086BB34AEB3B2388D418 (4 bytes) — hud_state_message member ___u1,
 * layout-identical to state_info (icon/text of the whole message). */
typedef union _F584AD1D4110086BB34AEB3B2388D418
{
    const icon_hud_element_definition *icon;       /* 0x00 */
    struct _BECA036B426CABFAF46B8904C2CEA3F4 text; /* 0x00 */
} _F584AD1D4110086BB34AEB3B2388D418;

typedef struct hud_state_message
{
    wchar_t                       message_buffer[256];  /* 0x000 */
    union _F584AD1D4110086BB34AEB3B2388D418 ___u1;      /* 0x200 (512) — icon/text of the whole message */
    state_info                    info[8];              /* 0x204 (516) — per-element resolved slots */
    hud_state_message_definition *state_message;        /* 0x224 (548) — source definition, if any */
    unsigned char                 valid;                /* 0x228 (552) */
    unsigned char                 is_text_flags;        /* 0x229 (553) — per-element icon-vs-text bitmask */
    unsigned char                 pad[2];               /* 0x22A (554) */
} hud_state_message;                                    /* 0x22C (556 bytes) */
