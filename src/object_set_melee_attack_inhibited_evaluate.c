/* object_set_melee_attack_inhibited_evaluate @0x83727E40 — HaloScript builtin evaluator. Argument [0] = object index (long), [4] = boolean.
 * No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void object_set_melee_attack_inhibited(int object_index, uint8_t inhibited);
void object_set_melee_attack_inhibited_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { object_set_melee_attack_inhibited(arguments[0], ((unsigned char *)arguments)[4]); hs_return(thread_index, 0); }
}
