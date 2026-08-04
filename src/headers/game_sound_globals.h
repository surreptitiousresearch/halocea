#pragma once
/* game_sound_global_data — global game-sound state (12 bytes). Layout from the database. */

typedef struct game_sound_global_data
{
    int frame_index;           /* 0x00 */
    int background_loop_index;  /* 0x04 */
    int render_time;            /* 0x08 */
} game_sound_global_data;

extern game_sound_global_data *game_sound_globals;
