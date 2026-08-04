/* vehicle_hover_evaluate @0x8372CC68 — HaloScript builtin wrapper for vehicle_hover; (object, bool). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void vehicle_hover(int vehicle_index, uint8_t hover_flag);

void vehicle_hover_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed argument block: object index @0, bool byte @+4 (BE high byte of slot 1). */
        vehicle_hover(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
