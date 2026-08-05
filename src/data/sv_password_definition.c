/* sv_password_definition @ 0x8211C9A4 — hs_function_table entry for the HaloScript "sv_password" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x821159CC -> "sv_password"
 *   +0x08 parse                  = 0x83778978 -> hs_macro_function_strings_parse
 *   +0x0C evaluate               = 0x8372EAB0 -> sv_password_evaluate
 *   +0x10 documentation          = 0x82115978
 *   +0x14 parameters             = 0x82115968
 *   +0x18 console_flags          = 0x1D
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"
#include "../headers/hs_console_flags.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_strings_parse(int16_t function_index, int expression_index);
extern void sv_password_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition sv_password_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "sv_password",               /* name */
    hs_macro_function_strings_parse, /* parse */
    sv_password_evaluate,        /* evaluate */
    "Sets the server password. If no password is given, displays the\ncurrent password.", /* documentation */
    "[password]",                /* parameters */
    (1u << _hs_console_allowed) | (1u << _hs_console_dedicated_server) | (1u << _hs_console_ingame) | (1u << _hs_console_idle), /* console_flags = 0x1D */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
