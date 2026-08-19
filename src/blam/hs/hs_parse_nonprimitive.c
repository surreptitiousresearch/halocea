/* hs_parse_nonprimitive @0x83777CA8 — parse a group "(...)" syntax node. The first child must be a primitive
 * naming what the group is: in a script/global-definition context it must be the word "script" or "global"
 * (dispatched to hs_add_script / hs_add_global); otherwise it must name a function or static script, whose
 * return type is checked against the slot's expected type before the function's own parser runs. A non-atom
 * head, an unknown name, a non-static script, or a blocked/disallowed construct each produce an error.
 *
 * Deviation: the decompiler inlines the "global"/"script" keyword comparison as byte loops; reproduced here
 * with strcmp for clarity (semantically identical). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_script.h"
#include "headers/hs_type.h"
#include "headers/hs_script_type.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern int strcmp(const char *a, const char *b);
extern int sprintf_0(char *string, const char *format, ...);
extern int hs_add_script(int expression_index);
extern int hs_add_global(int expression_index);
extern void hs_parse_call_predicate(int expression_index);
extern uint8_t hs_can_cast(int16_t actual_type, int16_t desired_type);

uint8_t hs_parse_nonprimitive(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    hs_syntax_node *head = &HS_SYNTAX_NODE(node->data);   /* first child: the name */
    int16_t expected_type = node->type;

    if ( (head->flags & (1u << _hs_syntax_node_primitive_bit)) == 0 )
    {
        const char *wanted = (expected_type == hs_special_form) ? "\"script\" or \"global\"" : "a function name";
        sprintf_0(hs_compile_globals.error_buffer, "i expected %s, but i got an expression.", wanted);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = head->source_offset;
        return 0;
    }

    if ( expected_type == hs_special_form )     /* script/global definition context */
    {
        const char *name = &hs_compile_globals.compiled_source[head->source_offset];
        if ( !strcmp(name, "global") )
            return hs_add_global(expression_index);
        if ( !strcmp(name, "script") )
            return hs_add_script(expression_index);
        hs_compile_globals.__noop = "i expected \"script\" or \"global\".";
        hs_compile_globals.error_offset = head->source_offset;
        return 0;
    }

    /* function or script call */
    hs_parse_call_predicate(expression_index);
    int function_index = node->___u1.function_index;
    if ( function_index == -1 )
    {
        hs_compile_globals.__noop = "this is not a valid function or script name.";
        hs_compile_globals.error_offset = head->source_offset;
        return 0;
    }

    if ( (node->flags & (1u << _hs_syntax_node_script_bit)) != 0 )       /* script call */
    {
        hs_script *script = &((hs_script *)global_scenario->hs_scripts.address)[function_index];
        int16_t script_type = script->script_type;
        if ( script_type != _hs_script_static && script_type != _hs_script_stub )
        {
            hs_compile_globals.__noop = "this is not a static script.";
            hs_compile_globals.error_offset = node->source_offset;
            return 0;
        }
        if ( node->type && !hs_can_cast(script->return_type, node->type) )
        {
            sprintf_0(hs_compile_globals.error_buffer,
                      "i expected a %s, but this script returns a %s.",
                      hs_type_names[node->type],
                      hs_type_names[script->return_type]);
            hs_compile_globals.__noop = hs_compile_globals.error_buffer;
            hs_compile_globals.error_offset = node->source_offset;
            return 0;
        }
        if ( !node->type )
            node->type = script->return_type;
        return 1;
    }

    /* built-in function call */
    const hs_function_definition *function = hs_function_table[function_index];
    if ( node->type && !hs_can_cast(function->return_type, node->type) )
    {
        sprintf_0(hs_compile_globals.error_buffer,
                  "i expected a %s, but this function returns a %s.",
                  hs_type_names[node->type],
                  hs_type_names[function->return_type]);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    if ( hs_compile_globals.disallow_blocks
      && (node->___u1.function_index == _hs_function_sleep || node->___u1.function_index == _hs_function_sleep_until) )
    {
        hs_compile_globals.__noop = "it is illegal to block in this context.";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    if ( hs_compile_globals.disallow_sets && node->___u1.function_index == _hs_function_set )
    {
        hs_compile_globals.__noop = "it is illegal to set the value of variables in this context.";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    if ( !node->type )
    {
        int16_t return_type = function->return_type;
        if ( return_type != hs_passthrough )    /* leave <passthrough> slots untyped */
            node->type = return_type;
    }
    /* funcptr cast removed: function->parse is already unsigned char(*)(__int16,int) — call directly */
    return function->parse(node->___u1.function_index, expression_index);
}
