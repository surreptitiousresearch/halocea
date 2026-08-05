/* hs_parse_scenario_datum @0x83776918 — shared parser for atoms naming an element of a scenario tag block
 * (trigger volumes, cutscene flags, device groups, etc.). Scans `block` for an element whose name string
 * (at byte `offset` within the `element_size`-byte element) matches the token; on a match the element index is
 * stored in the node's data field (word). A non-match builds a "this is not a valid <type> name" error. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/tag_block.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);
extern int stricmp(const char *a, const char *b);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int found = 0;

    if ( block->count > 0 )
    {
        const char *token = &hs_compile_globals.compiled_source[node->source_offset];
        for ( int16_t i = 0; i < block->count; i = (int16_t)(i + 1) )
        {
            const char *element = (const char *)tag_block_get_element_with_size(block, i, element_size);
            if ( !stricmp(&element[offset], token) )
            {
                *(int16_t *)&node->data = i;
                found = 1;
                break;
            }
        }
    }

    if ( !(unsigned char)found )
    {
        sprintf_0(hs_compile_globals.error_buffer, "this is not a valid %s name", hs_type_names[node->type]);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = node->source_offset;
    }
    return found;
}
