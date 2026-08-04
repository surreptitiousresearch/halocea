/* cinematic_set_title @0x83691888 — show a cinematic title immediately (zero delay). */

#include <stdint.h>

extern void cinematic_set_title_delayed(int16_t index, float delay);

void cinematic_set_title(int16_t index)
{
    cinematic_set_title_delayed(index, 0.0f);
}
