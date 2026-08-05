/* wake_definition @ 0x82119254 — hs_function_table entry for the HaloScript "wake" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x82118644 -> "wake"
 *   +0x08 parse                  = 0x83779920 -> hs_parse_wake
 *   +0x0C evaluate               = 0x8368E4E8 -> hs_evaluate_wake
 *   +0x10 documentation          = 0x82118618
 *   +0x14 parameters             = 0x82118608
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_wake(int16_t function_index, int expression_index);
extern void hs_evaluate_wake(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition wake_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "wake",                      /* name */
    hs_parse_wake,               /* parse */
    hs_evaluate_wake,            /* evaluate */
    "wakes a sleeping script in the next update.", /* documentation */
    "<script name>",             /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
