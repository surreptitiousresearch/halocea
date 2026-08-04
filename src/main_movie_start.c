/* main_movie_start @0x8368AA40 — begin movie capture: allocate a 640x480 capture bitmap, clear the movie
 * output directory, reset the frame index, set the per-frame delta from the requested fps (default 1/30s
 * when fps is ~0), and lock game time to 1.0x.
 *
 * Deviation: 0.033333335 is the 1/30 default; the float reciprocal math is reproduced as written. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/bitmap_format.h"
#include "headers/game_time_constants.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void directory_create_or_delete_contents(const char *directory_name);
extern void game_time_set_speed(float speed);

void main_movie_start(float frames_per_second)
{
    main_globals.movie = bitmap_2d_new(640, 480, 0, _bitmap_format_x8r8g8b8);
    if ( main_globals.movie )
    {
        directory_create_or_delete_contents("movie");
        main_globals.recording_frame_index = 0;
        if ( frames_per_second <= 0.000099999997 )
            main_globals.recording_dt = SECONDS_PER_TICK;
        else
            main_globals.recording_dt = 1.0f / frames_per_second;
        game_time_set_speed(1.0);
    }
}
