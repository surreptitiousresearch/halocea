#include <stdint.h>

extern uint32_t system_milliseconds(void);
extern uint32_t system_seconds(void);
extern int rand(void);

unsigned int get_number_suitable_for_initializing_random_seed(void)
{
    return rand() ^ (system_milliseconds() ^ system_seconds());
}
