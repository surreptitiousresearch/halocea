#pragma once
/* ai_command_definition — one atom (command) of a scenario AI command list (32 bytes). The opcode is
 * `atom_type`; `atom_modifier` is the per-opcode variant. Layout verbatim from the database. */

typedef struct ai_command_definition
{
    __int16          atom_type;                  /* 0x00 opcode */
    __int16          atom_modifier;              /* 0x02 */
    float            parameter1;                 /* 0x04 */
    float            parameter2;                 /* 0x08 */
    __int16          point1_index;               /* 0x0C */
    __int16          point2_index;               /* 0x0E */
    __int16          animation_reference_index;  /* 0x10 */
    __int16          script_reference_index;     /* 0x12 */
    __int16          recording_reference_index;  /* 0x14 */
    __int16          command_index;              /* 0x16 */
    __int16          object_name_index;          /* 0x18 */
    unsigned __int16 pad;                        /* 0x1A */
    unsigned int     unused;                     /* 0x1C */
} ai_command_definition;                         /* 0x20 = 32 bytes */
