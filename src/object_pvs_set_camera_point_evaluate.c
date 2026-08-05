/* object_pvs_set_camera_point_evaluate @0x83728200 — HaloScript builtin evaluator. Argument [0] = cutscene
 * camera point index (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void object_pvs_set_camera_point(int16_t camera_point_index);
void object_pvs_set_camera_point_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { object_pvs_set_camera_point(*arguments); hs_return(thread_index, 0); }
}
