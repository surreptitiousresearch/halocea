/* multiply_definition @ 0x82119104 — hs_function_table entry for the HaloScript "*" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0006 -> hs_type_real
 *   +0x04 name                   = 0x82026138 -> "*"
 *   +0x08 parse                  = 0x837792B8 -> hs_parse_arithmetic
 *   +0x0C evaluate               = 0x8368F648 -> hs_evaluate_arithmetic
 *   +0x10 documentation          = 0x821189A8
 *   +0x14 parameters             = 0x82118A1C
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_arithmetic(int16_t function_index, int expression_index);
extern void hs_evaluate_arithmetic(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition multiply_definition =
{
    hs_type_real,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "*",                         /* name */
    hs_parse_arithmetic,         /* parse */
    hs_evaluate_arithmetic,      /* evaluate */
    "returns the product of all specified expressions.", /* documentation */
    "<number(s)>",               /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
