#include "headers/stack_walk_globals.h"

int stack_walk_global_function_offset(void)
{
    return stack_walk_globals.fixup == -1 ? 0 : stack_walk_globals.fixup;
}
