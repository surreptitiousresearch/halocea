#pragma once
#include <stdint.h>
/* ai_conversation_participant — one participant slot in a scenario AI conversation definition (84 bytes).
 * Full DB layout. */

typedef struct ai_conversation_participant
{
    uint16_t pad;                            /* 0x00 */
    uint16_t flags;                          /* 0x02 */
    int16_t          selection_type;                 /* 0x04 */
    int16_t          actor_type;                     /* 0x06 */
    int16_t          preexisting_object_name_index;  /* 0x08 */
    int16_t          new_attach_object_name_index;   /* 0x0A */
    unsigned int     unused[3];                      /* 0x0C */
    int16_t          dialogue_variants[6];           /* 0x18 */
    char             ai_index_name[32];              /* 0x24 */
    int              runtime_ai_index;               /* 0x44 */
    unsigned int     unused2[3];                     /* 0x48 */
} ai_conversation_participant;                       /* 84 bytes */
