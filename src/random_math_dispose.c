#include "headers/random_math_globals.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void random_math_dispose(void)
{
    /* canonical global is now typed random_math_globals_s; direct field access */
    dlFree(random_math_globals.random_direction_table);
}
