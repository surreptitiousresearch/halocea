#include <stdint.h>
#include "headers/stack_walk_globals.h"

void stack_walk_disregard_symbol_names(uint8_t disregard)
{
    stack_walk_globals.disregard_symbol_names = disregard;
}
