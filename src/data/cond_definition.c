/* cond_definition @ 0x8211905C — hs_function_table entry for the HaloScript "cond" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0003 -> hs_passthrough
 *   +0x04 name                   = 0x82118B88 -> "cond"
 *   +0x08 parse                  = 0x83778E70 -> hs_parse_cond
 *   +0x0C evaluate               = 0x00000000 -> NULL
 *   +0x10 documentation          = 0x82118B4C
 *   +0x14 parameters             = 0x82118B14
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_cond(int16_t function_index, int expression_index);

const hs_function_definition cond_definition =
{
    hs_passthrough,              /* return_type */
    { 0, 0 },                    /* _pad02 */
    "cond",                      /* name */
    hs_parse_cond,               /* parse */
    0,                           /* evaluate */
    "returns the value associated with the first true condition.", /* documentation */
    "(<boolean1> <result1>) [(<boolean2> <result2>) [...]]", /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
