#pragma once
/* scripted_vibrate_datum — scripted rumble override applied on top of per-player impulses.
 * Layout from the database. */

typedef struct scripted_vibrate_datum
{
    float left_frequency;  /* 0x00 */
    float right_frequency; /* 0x04 */
    float scale;           /* 0x08 */
} scripted_vibrate_datum;  /* 12 bytes */
