#pragma once
/* ai_conversation_participant — one participant slot in a scenario AI conversation definition (84 bytes).
 * Full DB layout. */

typedef struct ai_conversation_participant
{
    unsigned __int16 pad;                            /* 0x00 */
    unsigned __int16 flags;                          /* 0x02 */
    __int16          selection_type;                 /* 0x04 */
    __int16          actor_type;                     /* 0x06 */
    __int16          preexisting_object_name_index;  /* 0x08 */
    __int16          new_attach_object_name_index;   /* 0x0A */
    unsigned int     unused[3];                      /* 0x0C */
    __int16          dialogue_variants[6];           /* 0x18 */
    char             ai_index_name[32];              /* 0x24 */
    int              runtime_ai_index;               /* 0x44 */
    unsigned int     unused2[3];                     /* 0x48 */
} ai_conversation_participant;                       /* 84 bytes */
