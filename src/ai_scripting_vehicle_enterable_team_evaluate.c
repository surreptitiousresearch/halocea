/* ai_scripting_vehicle_enterable_team_evaluate @0x8372A940 — HaloScript builtin
 * (ai_vehicle_enterable_team) evaluator. Argument block: [0] = vehicle unit index (long),
 * [4] = team index (short/enum). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_vehicle_enterable_team(int unit_index, int16_t team_index);

void ai_scripting_vehicle_enterable_team_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_vehicle_enterable_team(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
