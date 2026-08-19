/* hs_compile_postprocess @0x837780C0 — final type-checking pass over the compiled HaloScript syntax tree. Walks
 * every syntax node, resolving each node's value type: primitives are parsed (and validated against the source
 * buffer), global references resolve through hs_global_get_type, script references take the referenced script's
 * return type, and function calls resolve the callee name to a function index (caching it back into the node).
 * Each resolved type is then checked for castability to the node's expected usage type. On any failure the walk
 * stops and the error message / source location are returned. Returns 1 on success, 0 on failure.
 *
 * DEVIATION: in the script-reference branch the `script->script_type == _hs_script_stub` fallback test reads the script pointer
 * left over from a previous iteration when script_index is out of range — a latent quirk of the original
 * (compound short-circuit with an assignment side effect). Reproduced faithfully; `script` is loop-carried and
 * initialized to nullptr here (the original left it indeterminate). */

#include <stdint.h>
#include "headers/hs_compile_globals.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_function_definition.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/hs_script_type.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t hs_parse_primitive(int expression_index);
extern int16_t hs_global_get_type(int16_t designator);
extern int16_t hs_find_function_by_name(const char *name);
extern uint8_t hs_can_cast(int16_t actual_type, int16_t desired_type);

uint8_t hs_compile_postprocess(const char **error_message_pointer, const char **error_source_pointer)
{
    int success = 1;
    hs_compile_globals.compiled_source = (char *)global_scenario->hs_string_constants.address;
    hs_compile_globals.compiled_source_size = global_scenario->hs_string_constants.size - 1024;
    hs_compile_globals.__noop = nullptr;
    hs_compile_globals.variables_predetermined = 1;
    *error_message_pointer = nullptr;
    *error_source_pointer = nullptr;

    int16_t resolved_type = 0;
    hs_script *script = nullptr;  /* loop-carried; see DEVIATION note */

    for (int index = data_next_index(hs_syntax_data, -1); index != -1;
         index = data_next_index(hs_syntax_data, index))
    {
        hs_syntax_node *node = &HS_SYNTAX_NODE(index);

        if (node->type >= first_hs_type && node->type < number_of_hs_node_types)
        {
            if (node->flags & (1u << _hs_syntax_node_primitive_bit))
            {
                /* primitive / leaf expression */
                if (node->type >= hs_type_string || (node->flags & (1u << _hs_syntax_node_variable_bit)))
                {
                    int source_offset = node->source_offset;
                    uint8_t offset_ok = 1;
                    if (source_offset < 0 || source_offset >= hs_compile_globals.compiled_source_size)
                    {
                        offset_ok = 0;
                        hs_compile_globals.__noop = "bad source offset (you need to recompile.)";
                    }
                    success = offset_ok;
                    if (offset_ok)
                        success = hs_parse_primitive(index);
                }

                if (success && (node->flags & (1u << _hs_syntax_node_variable_bit)))
                    resolved_type = hs_global_get_type(node->data);
                else
                    resolved_type = node->___u1.constant_type;
            }
            else if (node->flags & (1u << _hs_syntax_node_script_bit))
            {
                /* script reference */
                int16_t script_index = node->___u1.script_index;
                if ((script_index >= 0 && script_index < global_scenario->hs_scripts.count
                     && (script = (hs_script *)global_scenario->hs_scripts.address + script_index,
                         script->script_type == _hs_script_static))
                    || script->script_type == _hs_script_stub)
                {
                    resolved_type = script->return_type;
                    goto check_cast;
                }
                hs_compile_globals.__noop = "bad script index (you need to recompile.)";
                success = 0;
            }
            else if ((int)node->data == -1 || HS_SYNTAX_NODE(node->data).type != hs_function_name)
            {
                hs_compile_globals.__noop = "corrupt syntax tree (you need to recompile scripts.)";
                success = 0;
            }
            else
            {
                /* function call: resolve the callee name node to a function index */
                hs_syntax_node *function_name_node = &HS_SYNTAX_NODE(node->data);
                int name_offset = function_name_node->source_offset;
                uint8_t offset_ok = 1;
                if (name_offset < 0 || name_offset >= hs_compile_globals.compiled_source_size)
                {
                    offset_ok = 0;
                    hs_compile_globals.__noop = "bad source offset (you need to recompile.)";
                }
                if (offset_ok)
                {
                    int16_t function_index = hs_find_function_by_name(
                        &hs_compile_globals.compiled_source[function_name_node->source_offset]);
                    if (function_index != -1)
                    {
                        node->___u1.function_index = function_index;
                        resolved_type = hs_function_table[function_index]->return_type;
                        goto check_cast;
                    }
                    hs_compile_globals.__noop = "missing function (you need to recompile scripts.)";
                }
                success = 0;
            }

        check_cast:
            if (success)
            {
                if (((resolved_type < first_hs_type || resolved_type >= number_of_hs_node_types) && resolved_type != hs_passthrough)
                    || !hs_can_cast(resolved_type, node->type))
                {
                    hs_compile_globals.__noop = "type is inconsistent with usage (you need to recompile scripts.)";
                    success = 0;
                }
                else
                {
                    success = 1;
                }
            }
        }
        else if (node->type != hs_function_name)
        {
            hs_compile_globals.__noop = "missing type (you need to recompile scripts.)";
            success = 0;
        }

        if (!success)
            break;
    }

    if (!success)
    {
        *error_message_pointer = hs_compile_globals.__noop;
        if (hs_compile_globals.error_offset != -1)
            *error_source_pointer = &hs_compile_globals.compiled_source[hs_compile_globals.error_offset];
    }

    hs_compile_globals.compiled_source = nullptr;
    hs_compile_globals.__noop = nullptr;
    hs_compile_globals.variables_predetermined = 0;
    return success;
}
