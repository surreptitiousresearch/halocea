#pragma once
#include <stdint.h>
/* ai_command_definition — one atom (command) of a scenario AI command list (32 bytes). The opcode is
 * `atom_type`; `atom_modifier` is the per-opcode variant. Layout verbatim from the database. */

typedef struct ai_command_definition
{
    int16_t          atom_type;                  /* 0x00 opcode */
    int16_t          atom_modifier;              /* 0x02 */
    float            parameter1;                 /* 0x04 */
    float            parameter2;                 /* 0x08 */
    int16_t          point1_index;               /* 0x0C */
    int16_t          point2_index;               /* 0x0E */
    int16_t          animation_reference_index;  /* 0x10 */
    int16_t          script_reference_index;     /* 0x12 */
    int16_t          recording_reference_index;  /* 0x14 */
    int16_t          command_index;              /* 0x16 */
    int16_t          object_name_index;          /* 0x18 */
    uint16_t pad;                        /* 0x1A */
    unsigned int     unused;                     /* 0x1C */
} ai_command_definition;                         /* 0x20 = 32 bytes */
