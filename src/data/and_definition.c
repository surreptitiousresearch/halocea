/* and_definition @ 0x82119094 — hs_function_table entry for the HaloScript "and" function.
 * Initializer reconstructed from the binary .rdata record (28 bytes, big-endian):
 *   +0x00 return_type            = 0x0005 -> hs_type_boolean
 *   +0x04 name                   = 0x82064704 -> "and"
 *   +0x08 parse                  = 0x83779158 -> hs_parse_logical
 *   +0x0C evaluate               = 0x8368F428 -> hs_evaluate_logical
 *   +0x10 documentation          = 0x82118A9C
 *   +0x14 parameters             = 0x82118A8C
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 0
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_parse_logical(int16_t function_index, int expression_index);
extern void hs_evaluate_logical(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition and_definition =
{
    hs_type_boolean,             /* return_type */
    { 0, 0 },                    /* _pad02 */
    "and",                       /* name */
    hs_parse_logical,            /* parse */
    hs_evaluate_logical,         /* evaluate */
    "returns true if all specified expressions are true.", /* documentation */
    "<boolean(s)>",              /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    0                            /* formal_parameter_count */
};
