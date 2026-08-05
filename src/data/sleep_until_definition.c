/* sleep_until_definition @ 0x82119238 — hs_function_table entry for the HaloScript "sleep_until" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x821186F4 -> "sleep_until"
 *   +0x08 parse                  = 0x837797F8 -> hs_parse_sleep_until
 *   +0x0C evaluate               = 0x8368FF20 -> hs_evaluate_sleep_until
 *   +0x10 documentation          = 0x82118660
 *   +0x14 parameters             = 0x8211864C
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_sleep_until(int16_t function_index, int expression_index);
extern void hs_evaluate_sleep_until(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition sleep_until_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "sleep_until",               /* name */
    hs_parse_sleep_until,        /* parse */
    hs_evaluate_sleep_until,     /* evaluate */
    "pauses execution of this script until the specified condition is true, checking once per second unless a different number of ticks is specified.", /* documentation */
    "<boolean> [<short>]",       /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
