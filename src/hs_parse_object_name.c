/* hs_parse_object_name @0x83776B78 — parse a scenario object-name atom. Resolves the token to an object-name
 * index, then checks that the named object's type is permitted for this script type via the type-mask table
 * (hs_object_type_masks[type - 43]); on success the index is stored in the node's data field (word). Failures
 * yield "this is not a valid object name." or "this is not an object of type <type>.".
 *
 * Each scenario object_names element is 36 bytes; the object type is the word at offset 32 (word index 16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_object_type_masks.h"
#include "headers/scenario.h"
#include "headers/scenario_object_name.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

#include "headers/scenario.h"
extern int16_t scenario_object_name_index_from_string(scenario *scenario, const char *name);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_object_name(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int16_t name_index = scenario_object_name_index_from_string(
                             global_scenario,
                             &hs_compile_globals.compiled_source[node->source_offset]);

    const char *error;
    if ( name_index == -1 )
    {
        error = "this is not a valid object name.";
    }
    else
    {
        int16_t object_type = ((scenario_object_name *)global_scenario->object_names.address)[name_index].runtime_object_type;
        if ( ((1 << object_type) & hs_object_type_masks[node->type - first_hs_object_name_type]) != 0 )
        {
            *(int16_t *)&node->data = name_index;
            return 1;
        }
        sprintf_0(hs_compile_globals.error_buffer, "this is not an object of type %s.", hs_type_names[node->type]);
        error = hs_compile_globals.error_buffer;
    }
    hs_compile_globals.__noop = error;
    hs_compile_globals.error_offset = node->source_offset;
    return 0;
}
