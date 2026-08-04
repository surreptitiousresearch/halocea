/* hs_parse_wake @0x83779920 — parse (wake <script>). Takes one script-name argument (type 10); the named
 * script must not be static (type 3) or stub (type 4), which cannot be awakened. Each hs_scripts element is
 * 92 bytes with the script type at offset 32 (word index 16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/scenario.h"
#include "headers/hs_script.h"
#include "headers/data_array.h"
#include "headers/hs_script_type.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse(int expression_index, int16_t expected_type);
extern uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index);

int hs_parse_wake(int16_t function_index, int expression_index)
{
    int script;
    if ( hs_get_parameter_indices(hs_function_table[function_index]->name, 1, &script, expression_index) )
    {
        if ( hs_parse(script, hs_type_script) )
        {
            __int16 script_index = (__int16)HS_SYNTAX_NODE(script).data;
            __int16 script_type = ((hs_script *)global_scenario->hs_scripts.address)[script_index].script_type;
            if ( script_type != _hs_script_static && script_type != _hs_script_stub )
                return 1;
            hs_compile_globals.__noop = "this static script cannot be awakened.";
            hs_compile_globals.error_offset = HS_SYNTAX_NODE(script).source_offset;
        }
    }
    return 0;
}
