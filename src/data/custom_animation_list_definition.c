/* custom_animation_list_definition @ 0x82119C1C — hs_function_table entry for the HaloScript "custom_animation_list" function.
 * Initializer reconstructed from the binary .rdata record (36 bytes, big-endian):
 *   +0x00 return_type            = 0x0005 -> hs_type_boolean
 *   +0x04 name                   = 0x82117E18 -> "custom_animation_list"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x837286A0 -> unit_scripting_start_user_animation_list_evaluate
 *   +0x10 documentation          = 0x8200155A
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x00
 *   +0x1A formal_parameter_count = 4  (+0x1C formal_parameters = hs_type_object_list, hs_type_animation_graph, hs_type_string, hs_type_boolean)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void unit_scripting_start_user_animation_list_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition custom_animation_list_definition =
{
    hs_type_boolean,             /* return_type */
    { 0, 0 },                    /* _pad02 */
    "custom_animation_list",     /* name */
    hs_macro_function_parse,     /* parse */
    unit_scripting_start_user_animation_list_evaluate, /* evaluate */
    "",                          /* documentation */
    0,                           /* parameters */
    0,                           /* console_flags */
    { 0 },                       /* _pad19 */
    4,                           /* formal_parameter_count */
    { hs_type_object_list, hs_type_animation_graph, hs_type_string, hs_type_boolean },  /* formal_parameters */
};
