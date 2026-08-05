/* player_add_equipment_evaluate @0x8372BBF8 — HaloScript builtin wrapper for player_add_equipment. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void player_add_equipment(int unit_index, int16_t profile_index, uint8_t reset);

void player_add_equipment_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_add_equipment(arguments[0], ((int16_t *)arguments)[2], ((unsigned char *)arguments)[8]);
        hs_return(thread_index, 0);
    }
}
