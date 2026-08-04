#pragma once
/* platoon_definition — element of encounter_definition.platoons (172 bytes), per an authoritative
 * structure dump (C:\halo\headers_ref\platoon_definition.h). Resolves the doc's "platoon def+60"
 * (maneuvering_rule) and "platoon def+48" (attacking_defending_rule) citations. */

#include "platoon_rule.h"
#include "tag_block.h"

typedef struct platoon_definition
{
    char           name[32];                  /* 0x00 */
    unsigned int   flags;                      /* 0x20 */
    unsigned int   unused1[3];                  /* 0x24 */
    platoon_rule   attacking_defending_rule;     /* 0x30 (48) */
    unsigned int   unused2;                       /* 0x38 */
    platoon_rule   maneuvering_rule;               /* 0x3C (60) */
    unsigned int   unused3;                         /* 0x44 */
    unsigned int   unused4[16];                      /* 0x48 */
    tag_block      unused_blocks[3];                  /* 0x88 */
} platoon_definition; /* 0xAC = 172 bytes */
