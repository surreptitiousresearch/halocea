/* hs_parse_variable @0x83775E80 — try to resolve a primitive atom as a HaloScript variable (global) reference.
 * On success it records the global designator on the node, type-checks the global against the slot's expected
 * type, adopts the global's type if the slot was untyped, marks the node as a global reference (flag 4), and
 * returns 1. If the name isn't a global it returns 0 (an error only when variables are predetermined). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_find_global_by_name(const char *name);
extern int16_t hs_global_get_type(int16_t designator);
extern uint8_t hs_can_cast(int16_t actual_type, int16_t desired_type);
extern const char *hs_global_get_name(int16_t designator);
extern int sprintf_0(char *string, const char *format, ...);

uint8_t hs_parse_variable(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    __int16 designator = hs_find_global_by_name(&hs_compile_globals.compiled_source[node->source_offset]);
    node->data = designator;

    if ( designator == -1 )
    {
        if ( !hs_compile_globals.variables_predetermined )
            return 0;
        hs_compile_globals.__noop = "this is not a valid variable name.";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }

    __int16 global_type = hs_global_get_type(designator);
    if ( node->type && !hs_can_cast(global_type, node->type) )
    {
        const char *type_name = hs_type_names[global_type];
        const char *variable_name = hs_global_get_name(node->data);
        sprintf_0(hs_compile_globals.error_buffer,
                  "i expected a value of type %s, but the variable %s has type %s",
                  hs_type_names[node->type], variable_name, type_name);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    if ( !node->type )
        node->type = global_type;
    node->flags |= (1u << _hs_syntax_node_variable_bit);
    return 1;
}
