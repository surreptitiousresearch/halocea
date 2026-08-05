/* sv_status_definition @ 0x8211C8D8 — hs_function_table entry for the HaloScript "sv_status" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x82115C04 -> "sv_status"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x8372E8C8 -> sv_status_evaluate
 *   +0x10 documentation          = 0x82115BE8
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x4F
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"
#include "../headers/hs_console_flags.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void sv_status_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition sv_status_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "sv_status",                 /* name */
    hs_macro_function_parse,     /* parse */
    sv_status_evaluate,          /* evaluate */
    "Shows status of the server", /* documentation */
    0,                           /* parameters */
    (1u << _hs_console_allowed) | (1u << _hs_console_listen_server) | (1u << _hs_console_dedicated_server) | (1u << _hs_console_ingame) | (1u << _hs_console_ship_game), /* console_flags = 0x4F */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
