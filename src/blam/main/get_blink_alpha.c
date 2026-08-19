/* get_blink_alpha @0x83747C80 — a 2700ms-period half-sine pulse (0.0011635528... == pi/2700), used to fade
 * HUD blink indicators in and out. DEVIATION: the decompiler rendered the 64-bit-double intermediate as a
 * broken HIDWORD/LODWORD split; reproduced as the equivalent plain double computation. */

#include <stdint.h>

extern double sin(double x);
extern uint32_t system_milliseconds(void);

float get_blink_alpha(void)
{
    unsigned int elapsed = system_milliseconds() % 0xA8C;

    return (float)sin((double)elapsed * 0.001163552834662885);
}
