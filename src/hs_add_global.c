/* hs_add_global @0x83777000 — compile a "(global <type> <name> <initial value>)" definition into the
 * scenario's hs_globals block. Validates the 3-argument shape, the type keyword, and the (non-empty, <32 char,
 * not-already-defined) name, parses the initial-value expression with blocks/sets disallowed, then appends a
 * 92-byte global entry (name at +0, type at word 16, initializer node at dword 10). Returns 1 on success.
 *
 * Deviation: name length checks are inlined strlen byte-loops in the decompiler; reproduced with a helper. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_global_internal.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern int16_t string_list_find(const char *string, int16_t list_count, const char **string_list);
extern int16_t hs_find_global_by_name(const char *name);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);
extern int16_t tag_block_add_element(tag_block *block);
extern char *strcpy(char *dst, const char *src);

static int name_length(const char *s)
{
    const char *p = s;
    while ( *p++ )
        ;
    return p - s - 1;
}

int hs_add_global(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);

    /* walk the argument chain: <global> -> <type> -> <name> -> <value> */
    int type_node = HS_SYNTAX_NODE(node->data).next_node_index;
    if ( type_node == -1
      || (HS_SYNTAX_NODE(type_node).next_node_index) == -1 )
    {
        hs_compile_globals.__noop = "i expected (global<type> <name> <initial value>)";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    int name_node = HS_SYNTAX_NODE(type_node).next_node_index;
    int value_node = HS_SYNTAX_NODE(name_node).next_node_index;
    if ( value_node == -1
      || HS_SYNTAX_NODE(value_node).next_node_index != -1 )
    {
        hs_compile_globals.__noop = "i expected (global<type> <name> <initial value>)";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }

    int16_t type = string_list_find(&hs_compile_globals.compiled_source[HS_SYNTAX_NODE(type_node).source_offset], number_of_hs_node_types, hs_type_names);
    if ( type < first_hs_type || type >= number_of_hs_node_types )
    {
        hs_compile_globals.__noop = "this is not a valid type.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(type_node).source_offset;
        return 0;
    }

    const char *name = &hs_compile_globals.compiled_source[HS_SYNTAX_NODE(name_node).source_offset];
    int length = name_length(name);
    if ( length == 0 || (unsigned int)length > 0x1F )
    {
        hs_compile_globals.__noop = "i expected a global variable name less than 32 characters.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(name_node).source_offset;
        return 0;
    }

    if ( hs_find_global_by_name(name) != -1 )
    {
        hs_compile_globals.__noop = "there is already a variable by this name.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(name_node).source_offset;
        return 0;
    }

    hs_compile_globals.disallow_blocks = 1;
    hs_compile_globals.disallow_sets = 1;
    if ( hs_parse(value_node, type) )
    {
        int16_t element = tag_block_add_element(&global_scenario->hs_globals);
        if ( element != -1 )
        {
            hs_global_internal *entry =
                &((hs_global_internal *)global_scenario->hs_globals.address)[element];
            strcpy(entry->name, name);
            entry->type = type;
            entry->initialization_expression_index = value_node;
            hs_compile_globals.disallow_blocks = 0;
            hs_compile_globals.disallow_sets = 0;
            return 1;
        }
        hs_compile_globals.__noop = "i couldn't allocate space for this global.";
        hs_compile_globals.error_offset = node->source_offset;
    }
    hs_compile_globals.disallow_blocks = 0;
    hs_compile_globals.disallow_sets = 0;
    return 0;
}
