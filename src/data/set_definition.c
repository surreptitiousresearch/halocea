/* set_definition @ 0x82119078 — hs_function_table entry for the HaloScript "set" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0003 -> hs_passthrough
 *   +0x04 name                   = 0x82050D18 -> "set"
 *   +0x08 parse                  = 0x83778F40 -> hs_parse_set
 *   +0x0C evaluate               = 0x8368F290 -> hs_evaluate_set
 *   +0x10 documentation          = 0x82118AF0
 *   +0x14 parameters             = 0x82118AD0
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_set(int16_t function_index, int expression_index);
extern void hs_evaluate_set(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition set_definition =
{
    hs_passthrough,              /* return_type */
    { 0, 0 },                    /* _pad02 */
    "set",                       /* name */
    hs_parse_set,                /* parse */
    hs_evaluate_set,             /* evaluate */
    "set the value of a global variable.", /* documentation */
    "<variable name> <expression>", /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
