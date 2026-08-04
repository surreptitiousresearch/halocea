#include <stdint.h>
#include "headers/data_array.h"

extern void cseries_initialize(void);
extern void errors_initialize(void);
extern void tag_files_open(void);
extern void real_math_initialize(void);
extern void game_state_initialize(void);
extern int rasterizer_initialize(void);
extern void sound_enable(uint8_t enabled);
extern void sound_initialize(void);
extern void physical_memory_verify(void);

int shell_initialize(void)
{
    cseries_initialize();
    errors_initialize();
    tag_files_open();
    real_math_initialize();
    game_state_initialize();
    rasterizer_initialize();
    sound_enable(0);
    sound_initialize();
    physical_memory_verify();
    return 0;
}
