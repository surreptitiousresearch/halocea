/* hs_add_script @0x837772E8 — compile a "(script <type> [<return type>] <name> <expression(s)>)" definition
 * into the scenario's hs_scripts block. Static/stub scripts carry an explicit return type; startup/dormant/
 * continuous scripts are implicitly void (type 4). The body expressions are wrapped in a synthetic "begin"
 * group and parsed against the return type, and a 92-byte script entry (name@0, type@32, return type@34, root
 * expression@36) is appended. A static script may override an existing stub of the same return type (and a
 * stub redeclaring a matching static is accepted as a no-op). Returns 1 on success.
 *
 * Deviation: name length checks are inlined strlen byte-loops in the decompiler; reproduced with a helper. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_script.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/hs_script_type.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern int16_t string_list_find(const char *string, int16_t list_count, const char **string_list);
extern int16_t hs_find_script_by_name(const char *name);
extern int16_t tag_block_add_element(tag_block *block);
extern int datum_new(data_array *data);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);
extern char *strcpy(char *dst, const char *src);

static int name_length(const char *s)
{
    const char *p = s;
    while ( *p++ )
        ;
    return p - s - 1;
}

int hs_add_script(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);

    int type_keyword_node = HS_SYNTAX_NODE(node->data).next_node_index;
    if ( type_keyword_node == -1 )
    {
        hs_compile_globals.__noop = "i expected (script <type> <name> <expression(s)>)";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }

    __int16 script_type = string_list_find(&hs_compile_globals.compiled_source[HS_SYNTAX_NODE(type_keyword_node).source_offset], NUMBER_OF_HS_SCRIPT_TYPES, hs_script_type_names);
    if ( script_type == -1 )
    {
        hs_compile_globals.__noop = "script type must be \"startup\", \"dormant\", \"continuous\", or \"static\".";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(type_keyword_node).source_offset;
        return 0;
    }

    int name_node = -1;
    __int16 return_type = hs_type_void;  /* implicit void for non-static/stub scripts */
    int ready = 0;

    if ( script_type != _hs_script_static && script_type != _hs_script_stub )   /* startup / dormant / continuous */
    {
        name_node = HS_SYNTAX_NODE(type_keyword_node).next_node_index;
        return_type = hs_type_void;
        ready = 1;
    }
    else                                  /* static (3) or stub (4): explicit return type */
    {
        if ( HS_SYNTAX_NODE(type_keyword_node).next_node_index == -1 )
        {
            hs_compile_globals.__noop = "i expected (script local <type> <name> <expression(s)>).";
            hs_compile_globals.error_offset = node->source_offset;
        }
        else
        {
            int return_type_node = HS_SYNTAX_NODE(type_keyword_node).next_node_index;
            __int16 found = string_list_find(&hs_compile_globals.compiled_source[HS_SYNTAX_NODE(return_type_node).source_offset], number_of_hs_node_types, hs_type_names);
            name_node = HS_SYNTAX_NODE(return_type_node).next_node_index;
            return_type = found;
            if ( found >= first_hs_type && found < number_of_hs_node_types )
            {
                ready = 1;
            }
            else
            {
                hs_compile_globals.__noop = "this is not a valid return type.";
                hs_compile_globals.error_offset = HS_SYNTAX_NODE(return_type_node).source_offset;
            }
        }
    }

    if ( !ready )
        return 0;

    int body_node = (name_node != -1) ? HS_SYNTAX_NODE(name_node).next_node_index : -1;
    if ( name_node == -1 || body_node == -1 )
    {
        const char *message;
        if ( script_type == _hs_script_static )
            message = "i expected (script static <type> <name> <expression(s)>)";
        else if ( script_type == _hs_script_stub )
            message = "i expected (script stub <type> <name> <expression(s)>)";
        else
            message = "i expected (script <type> <name> <expression(s)>)";
        hs_compile_globals.__noop = message;
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }

    const char *name = &hs_compile_globals.compiled_source[HS_SYNTAX_NODE(name_node).source_offset];
    int length = name_length(name);
    if ( length == 0 || (unsigned int)length > 0x1F )
    {
        hs_compile_globals.__noop = "i expected a script name less than 32 characters.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(name_node).source_offset;
        return 0;
    }

    scenario *scn = global_scenario;
    __int16 existing = hs_find_script_by_name(name);
    int script_index = existing;
    if ( existing == -1 )
    {
        script_index = tag_block_add_element(&scn->hs_scripts);
        if ( script_index == -1 )
        {
            hs_compile_globals.__noop = "i couldn't allocate a script.";
            hs_compile_globals.error_offset = node->source_offset;
            return 0;
        }
    }
    else
    {
        hs_script *existing_entry = &((hs_script *)scn->hs_scripts.address)[existing];
        __int16 existing_type = existing_entry->script_type;
        if ( existing_type == _hs_script_stub && existing_entry->return_type == return_type && script_type == _hs_script_static )
        {
            /* overriding a stub with a matching static — fall through to (re)define */
        }
        else if ( existing_type == _hs_script_static && existing_entry->return_type == return_type && script_type == _hs_script_stub )
        {
            return 1;    /* stub redeclaring an existing static of the same type: no-op */
        }
        else
        {
            hs_compile_globals.__noop = "only static scripts of the same type can override stub scripts.";
            hs_compile_globals.error_offset = node->source_offset;
            return 0;
        }
    }

    hs_script *entry = &((hs_script *)scn->hs_scripts.address)[script_index];

    /* wrap the body in a synthetic "(begin <body...>)" group */
    int wrapper_index = datum_new(hs_syntax_data);
    int begin_index = datum_new(hs_syntax_data);
    if ( wrapper_index == -1 || begin_index == -1 )
    {
        hs_compile_globals.__noop = "i couldn't allocate a syntax node.";
        return 0;
    }
    hs_syntax_node *wrapper = &HS_SYNTAX_NODE(wrapper_index);
    hs_syntax_node *begin = &HS_SYNTAX_NODE(begin_index);
    wrapper->data = begin_index;
    wrapper->next_node_index = -1;
    wrapper->source_offset = node->source_offset;
    wrapper->flags = 0;
    begin->next_node_index = body_node;
    begin->source_offset = -1;
    begin->___u1.function_index = _hs_function_begin;
    begin->flags = (1u << _hs_syntax_node_primitive_bit);
    begin->type = hs_function_name;

    if ( hs_parse(wrapper_index, return_type) )
    {
        strcpy(entry->name, name);
        entry->script_type = script_type;
        entry->return_type = return_type;
        entry->root_expression_index = wrapper_index;
        return 1;
    }
    return 0;
}
