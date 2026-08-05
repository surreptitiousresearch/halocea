/* ai_playfight_definition @ 0x8211AA0C — hs_function_table entry for the HaloScript "ai_playfight" function.
 * Initializer reconstructed from the binary .rdata record (32 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x821174D4 -> "ai_playfight"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x8372A818 -> ai_scripting_playfight_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 2  (+0x1C formal_parameters = hs_type_ai, hs_type_boolean)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void ai_scripting_playfight_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition ai_playfight_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "ai_playfight",              /* name */
    hs_macro_function_parse,     /* parse */
    ai_scripting_playfight_evaluate, /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    2,                           /* formal_parameter_count */
    { hs_type_ai, hs_type_boolean },  /* formal_parameters */
};
