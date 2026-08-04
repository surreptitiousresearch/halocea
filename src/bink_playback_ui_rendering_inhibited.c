#include <stdint.h>
/* bink_playback_ui_rendering_inhibited @0x837249CC — whether Bink video playback is currently suppressing
 * UI rendering. Bink is stubbed out in this build, so it always returns 0 (never inhibited). */

uint8_t bink_playback_ui_rendering_inhibited(void)
{
    return 0;
}
