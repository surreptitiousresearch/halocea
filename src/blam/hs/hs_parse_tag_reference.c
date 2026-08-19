/* hs_parse_tag_reference @0x83776530 — parse a tag-reference atom. Scans scenario.hs_references for an entry
 * whose tag name matches the token and whose group tag matches the one expected for this script type
 * (hs_tag_reference_type_group_tags[type - 24]); on a match the resolved tag index is stored in the node's data
 * field. Always returns 1 (a non-match simply leaves data unset — the same behaviour as the original).
 *
 * Deviation: the decompiler inlines the name compare as a byte loop; reproduced with strcmp. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tag_reference.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int strcmp(const char *a, const char *b);

int hs_parse_tag_reference(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int count = global_scenario->hs_references.count;
    if ( count <= 0 )
        return 1;

    const char *token = &hs_compile_globals.compiled_source[node->source_offset];
    hs_tag_reference *references = (hs_tag_reference *)global_scenario->hs_references.address;

    for ( int16_t i = 0; i < count; i = (int16_t)(i + 1) )
    {
        hs_tag_reference *entry = &references[i];
        if ( !strcmp(entry->reference.name, token)
          && entry->reference.group_tag == hs_tag_reference_type_group_tags[node->type - first_hs_tag_reference_type] )
        {
            node->data = entry->reference.index;
            return 1;
        }
    }
    return 1;
}
