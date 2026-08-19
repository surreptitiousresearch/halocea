/* scripted_player_control_set_camera_control_evaluate @0x8372B7C0 — HaloScript builtin evaluator. Argument
 * [0] = boolean. Returns the new state packed into the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern char scripted_player_control_set_camera_control(char enabled);
void scripted_player_control_set_camera_control_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(scripted_player_control_set_camera_control(*arguments))) << 24;
        hs_return(thread_index, result);
    }
}
