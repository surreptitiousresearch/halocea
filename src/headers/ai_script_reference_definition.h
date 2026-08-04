#pragma once
/* ai_script_reference_definition — one element of scenario.ai_script_references (40 bytes). The name at
 * offset 0 is the hs script to wake. Layout verbatim from the database (types_members). */

typedef struct ai_script_reference_definition
{
    char         script_name[32];   /* 0x00 */
    unsigned int unused[2];         /* 0x20 */
} ai_script_reference_definition;   /* 0x28 = 40 bytes */
