/* units_set_maximum_vitality_definition @ 0x82119DB8 — hs_function_table entry for the HaloScript "units_set_maximum_vitality" function.
 * Initializer reconstructed from the binary .rdata record (36 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x82117CDC -> "units_set_maximum_vitality"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x83728AB0 -> units_scripting_set_maximum_vitality_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 3  (+0x1C formal_parameters = hs_type_object_list, hs_type_real, hs_type_real)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void units_scripting_set_maximum_vitality_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition units_set_maximum_vitality_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "units_set_maximum_vitality", /* name */
    hs_macro_function_parse,     /* parse */
    units_scripting_set_maximum_vitality_evaluate, /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    3,                           /* formal_parameter_count */
    { hs_type_object_list, hs_type_real, hs_type_real },  /* formal_parameters */
};
