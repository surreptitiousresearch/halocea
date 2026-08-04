/* hs_parse_primitive @0x83777B38 — parse a primitive (atom) syntax node against its already-assigned type. A
 * bare script/variable definition or a value forced into a <void> slot is an error. Otherwise it first tries
 * to resolve the atom as a variable, and failing that dispatches to the per-type primitive parser from
 * hs_type_primitive_parsers[]. An unsupported type yields a formatted error. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse_variable(int expression_index);
extern unsigned char (*hs_type_primitive_parsers[])(int expression_index);
extern int sprintf_0(char *string, const char *format, ...);

uint8_t hs_parse_primitive(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int result = 0;
    __int16 type = node->type;

    if ( type == hs_special_form )
    {
        hs_compile_globals.__noop = "i expected a script or variable definition.";
        hs_compile_globals.error_offset = node->source_offset;
        return result;
    }
    if ( type == hs_type_void )
    {
        hs_compile_globals.__noop = "the value of this expression (in a <void> slot) can never be used.";
        hs_compile_globals.error_offset = node->source_offset;
        return result;
    }

    unsigned char variables_predetermined = hs_compile_globals.variables_predetermined;
    if ( !variables_predetermined || (node->flags & (1u << _hs_syntax_node_variable_bit)) != 0 )
    {
        result = hs_parse_variable(expression_index);
        variables_predetermined = hs_compile_globals.variables_predetermined;
    }
    if ( (unsigned char)result )
        return result;

    __int16 resolved_type = node->type;
    if ( resolved_type
      && !hs_compile_globals.__noop
      && (!variables_predetermined || (node->flags & (1u << _hs_syntax_node_variable_bit)) == 0) )
    {
        unsigned char (*parser)(int) = hs_type_primitive_parsers[resolved_type];
        if ( parser )
            return parser(expression_index);
        sprintf_0(hs_compile_globals.error_buffer,
                  "expressions of type %s are currently unsupported.",
                  hs_type_names[resolved_type]);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    return result;
}
