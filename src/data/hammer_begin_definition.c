/* hammer_begin_definition @ 0x8211BD0C — hs_function_table entry for the HaloScript "hammer_begin" function.
 * Initializer reconstructed from the binary .rdata record (40 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x82116758 -> "hammer_begin"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x8372D098 -> hammer_begin_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 5  (+0x1C formal_parameters = hs_type_string, hs_type_string, hs_type_long_integer, hs_type_short_integer, hs_type_short_integer)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void hammer_begin_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition hammer_begin_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "hammer_begin",              /* name */
    hs_macro_function_parse,     /* parse */
    hammer_begin_evaluate,       /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    5,                           /* formal_parameter_count */
    { hs_type_string, hs_type_string, hs_type_long_integer, hs_type_short_integer, hs_type_short_integer },  /* formal_parameters */
};
