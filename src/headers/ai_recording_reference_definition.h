#pragma once
/* ai_recording_reference_definition — one element of scenario.ai_recording_references (40 bytes). The name
 * at offset 0 is the recorded animation to play. Layout verbatim from the database (types_members). */

typedef struct ai_recording_reference_definition
{
    char         recording_name[32]; /* 0x00 */
    unsigned int unused[2];          /* 0x20 */
} ai_recording_reference_definition; /* 0x28 = 40 bytes */
