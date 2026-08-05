#pragma once

/* dialogue_definition — a unit's dialogue tag ('udlg'): one sound tag reference per vocalization
 * type (4112 bytes, DB-verified via types_members). `vocalizations` is indexed directly by
 * ai_vocalization_type, and its 209 entries are exactly NUMBER_OF_VOCALIZATION_TYPES from DB enum
 * $88A83E6B4E3F0C475309D2DD9268E98D — which is what proves the index domain.
 *
 * Recovered 2026-08-05 from unit_test_speech.c, which reached the sound tag as the raw fold
 * `TAG_GET(int, dialogue_tag)[4*type + 7]` (int units: 16 + 16*type + 12 bytes). */

#include "tag_reference.h"

typedef struct dialogue_definition
{
    short         vocalization_enum_version;  /* 0x000 */
    unsigned short pad;                       /* 0x002 */
    unsigned int  unused[3];                  /* 0x004 */
    tag_reference vocalizations[209];         /* 0x010 — indexed by ai_vocalization_type */
    tag_reference unused2[47];                /* 0xD20 — tail padding out to the 256-entry table */
} dialogue_definition;                        /* 4112 bytes */
