/* ai_debug_communication_focus_definition @ 0x821192E0 — hs_function_table entry for the HaloScript "ai_debug_communication_focus" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x82118464 -> "ai_debug_communication_focus"
 *   +0x08 parse                  = 0x83779B70 -> hs_parse_debug_string
 *   +0x0C evaluate               = 0x83690590 -> hs_evaluate_debug_string
 *   +0x10 documentation          = 0x82118424
 *   +0x14 parameters             = 0x82118508
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_debug_string(int16_t function_index, int expression_index);
extern void hs_evaluate_debug_string(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition ai_debug_communication_focus_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "ai_debug_communication_focus", /* name */
    hs_parse_debug_string,       /* parse */
    hs_evaluate_debug_string,    /* evaluate */
    "focuses (or stops focusing) a set of unit vocalization types.", /* documentation */
    "<string(s)>",               /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
