/* hs_compile_finish @0x8377A018 — finalize a permanent (scenario) compile: build the scenario's
 * string-constant block from the source buffer, walk every global initializer and script root expression to
 * mark nodes persistent and intern their strings, garbage-collect transient syntax nodes, then pad the
 * string-constant block with a 1KB zero tail. Bails early on a failing tag_data_resize. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/hs_global_internal.h"
#include "headers/hs_script.h"
#include "headers/blam_data_globals.h"

extern uint8_t tag_data_resize(tag_data *data, int size);
extern int hs_concatenate_string_constant(const char *string);
extern void hs_concatenate_expression(int expression_index);
extern void hs_node_gc(void);
extern void *memset(void *dst, int value, unsigned int n);

/* scenario hs_globals / hs_scripts entries are 92-byte typed elements. */
#define HS_GLOBAL_INITIALIZER(scn, i)  (((hs_global_internal *)(scn)->hs_globals.address)[i].initialization_expression_index)
#define HS_SCRIPT_ROOT(scn, i)         (((hs_script *)(scn)->hs_scripts.address)[i].root_expression_index)

/* disasm: r3 at blr is the tag_data_resize/memset callee residue (a bool on the fail path, a pointer on
 * the success path — inconsistent), 0 sampled consumers (caller hs_compile_dispose ignores it) => void */
void hs_compile_finish(void)
{
    scenario *scn = global_scenario;
    tag_data *string_constants = &scn->hs_string_constants;

    if ( !tag_data_resize(string_constants, hs_compile_globals.compiled_source_size) )
        return;

    hs_compile_globals.string_constant_buffer = (char *)scn->hs_string_constants.address;
    hs_compile_globals.string_constant_buffer_offset = 0;
    hs_compile_globals.string_constant_buffer_size = hs_compile_globals.compiled_source_size;

    for ( int g = 0; g < scn->hs_globals.count; g = (__int16)(g + 1) )
    {
        int node_index = HS_GLOBAL_INITIALIZER(global_scenario, g);
        hs_syntax_node *node = &HS_SYNTAX_NODE(node_index);
        node->flags |= (1u << _hs_syntax_node_permanent_bit);
        if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )   /* primitive initializer */
        {
            if ( node->type == hs_function_name )
            {
                if ( node->source_offset == -1 )
                    node->source_offset = hs_concatenate_string_constant(hs_function_table[node->___u1.function_index]->name);
                else
                    node->source_offset = hs_concatenate_string_constant(&hs_compile_globals.compiled_source[node->source_offset]);
            }
            else if ( (node->flags & (1u << _hs_syntax_node_variable_bit)) != 0 || node->type >= hs_type_string )
            {
                node->source_offset = hs_concatenate_string_constant(&hs_compile_globals.compiled_source[node->source_offset]);
            }
        }
        else                            /* group initializer: recurse over the argument list */
        {
            for ( int i = node->data; i != -1; i = HS_SYNTAX_NODE(i).next_node_index )
                hs_concatenate_expression(i);
        }
    }

    for ( int s = 0; s < scn->hs_scripts.count; s = (__int16)(s + 1) )
        hs_concatenate_expression(HS_SCRIPT_ROOT(scn, s));

    hs_node_gc();

    int final_offset = hs_compile_globals.string_constant_buffer_offset;
    if ( tag_data_resize(string_constants, hs_compile_globals.string_constant_buffer_offset + 1024) )
        memset((char *)scn->hs_string_constants.address + final_offset, 0, 0x400u);
}
