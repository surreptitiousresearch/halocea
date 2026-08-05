/* players_definition @ 0x8211933C — hs_function_table entry for the HaloScript "players" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0017 -> hs_type_object_list
 *   +0x04 name                   = 0x82029820 -> "players"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x837271D8 -> hs_players_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void hs_players_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition players_definition =
{
    hs_type_object_list,         /* return_type */
    { 0, 0 },                    /* _pad02 */
    "players",                   /* name */
    hs_macro_function_parse,     /* parse */
    hs_players_evaluate,         /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
