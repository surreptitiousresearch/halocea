/* vehicle_load_magic_definition @ 0x82119E24 — hs_function_table entry for the HaloScript "vehicle_load_magic" function.
 * Initializer reconstructed from the binary .rdata record (36 bytes, big-endian):
 *   +0x00 return_type            = 0x0007 -> hs_type_short_integer
 *   +0x04 name                   = 0x82117C90 -> "vehicle_load_magic"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x83728BB8 -> vehicle_scripting_load_magic_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 3  (+0x1C formal_parameters = hs_type_unit, hs_type_string, hs_type_object_list)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void vehicle_scripting_load_magic_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition vehicle_load_magic_definition =
{
    hs_type_short_integer,       /* return_type */
    { 0, 0 },                    /* _pad02 */
    "vehicle_load_magic",        /* name */
    hs_macro_function_parse,     /* parse */
    vehicle_scripting_load_magic_evaluate, /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    3,                           /* formal_parameter_count */
    { hs_type_unit, hs_type_string, hs_type_object_list },  /* formal_parameters */
};
