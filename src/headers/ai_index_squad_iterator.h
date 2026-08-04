#pragma once
/* ai_index_squad_iterator — iterates the squads reachable from an AI index (encounter / squad / platoon).
 * 20 bytes, full DB layout. */

typedef struct ai_index_squad_iterator
{
    int encounter_index;        /* 0x00 */
    int required_platoon_index; /* 0x04 */
    int squad_index;            /* 0x08 */
    int next_squad_index;       /* 0x0C */
    int last_squad_index;       /* 0x10 */
} ai_index_squad_iterator;      /* 20 bytes */
