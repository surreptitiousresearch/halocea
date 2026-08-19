/* input_abstraction_is_idle @ 0x83724AD0 */
#include <stdint.h>
#include "headers/input_abstraction_globals.h"
uint8_t input_abstraction_is_idle(void)
{
    return input_abstraction_globals.all_idle;
}
