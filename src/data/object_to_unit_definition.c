/* object_to_unit_definition @ 0x8211928C — hs_function_table entry for the HaloScript "unit" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0026 -> hs_type_unit
 *   +0x04 name                   = 0x8211463C -> "unit"
 *   +0x08 parse                  = 0x83779AF0 -> hs_parse_object_cast_up
 *   +0x0C evaluate               = 0x83690458 -> hs_evaluate_object_cast_up
 *   +0x10 documentation          = 0x82118588
 *   +0x14 parameters             = 0x8211857C
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_object_cast_up(int16_t function_index, int expression_index);
extern void hs_evaluate_object_cast_up(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition object_to_unit_definition =
{
    hs_type_unit,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "unit",                      /* name */
    hs_parse_object_cast_up,     /* parse */
    hs_evaluate_object_cast_up,  /* evaluate */
    "converts an object to a unit.", /* documentation */
    "<object>",                  /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
