/* hs_parse_set @0x83778F40 — parse (set <global> <value>). Requires exactly a global-variable name and a
 * value. The global is resolved (hs_find_global_by_name) and its type recorded on the variable node; if the
 * group already expects a type the global's type must be castable to it. The value is then parsed against the
 * global's type, and the group type defaults to the global's type.
 *
 * Deviation: the decompiler renders the two error %s arguments as raw type words cast to const char*; the real
 * code indexes hs_type_names[] (the same pattern used by every other type-name error here). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_find_global_by_name(const char *name);
extern int16_t hs_global_get_type(int16_t designator);
extern uint8_t hs_can_cast(int16_t actual_type, int16_t desired_type);
extern uint8_t hs_parse_variable(int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_set(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);

    int variable = HS_SYNTAX_NODE(group->data).next_node_index;
    if ( variable == -1 )
    {
        hs_compile_globals.__noop = "i expected a variable to set and a value.";
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }

    hs_syntax_node *variable_node = &HS_SYNTAX_NODE(variable);
    int value = variable_node->next_node_index;
    if ( value == -1 )
    {
        hs_compile_globals.__noop = "i expected an assignment value.";
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }

    if ( HS_SYNTAX_NODE(value).next_node_index != -1 )
    {
        hs_compile_globals.__noop = "i didn't expect this argument.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(HS_SYNTAX_NODE(value).next_node_index).source_offset;
        return 0;
    }

    int16_t global = hs_find_global_by_name(&hs_compile_globals.compiled_source[variable_node->source_offset]);
    if ( global == -1 )
    {
        hs_compile_globals.__noop = "this is not a valid global variable.";
        hs_compile_globals.error_offset = variable_node->source_offset;
        return 0;
    }

    int16_t global_type = hs_global_get_type(global);
    variable_node->type = global_type;
    if ( group->type && !hs_can_cast(global_type, group->type) )
    {
        sprintf_0(hs_compile_globals.error_buffer,
                  "you cannot pass the result of this set (type %s) to a function that expects type %s.",
                  hs_type_names[variable_node->type],
                  hs_type_names[group->type]);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }

    hs_parse_variable(variable);
    if ( !group->type )
        group->type = variable_node->type;
    return hs_parse(value, variable_node->type) != 0;
}
