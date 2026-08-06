#include <stdint.h>
/* DEVIATION: this TU hand-declared `extern int global_screenshot_count`, but the definition is
 * int16_t — every reader is `lhz`+`extsh` and every writer `sth` (0x8368AA0C, 0x8368B4C8,
 * 0x8368C704, 0x83791E8C). On this big-endian target the wrong width reads the wrong bytes.
 * Including the generated canonical header instead of hand-declaring is what makes that a
 * compile error rather than a silent divergence. */
#include "headers/blam_data_globals.h"

BOOL screenshot_in_progress(void)
{
    return global_screenshot_count > 1 || (global_screenshot_count == 1 && global_screenshot_size > 1);
}
