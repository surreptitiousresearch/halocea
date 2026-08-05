/* rasterizer_frame_statistics_get_fps @0x83790310 — push the current frame's timestamp onto the rolling
 * FPS sample window and, when enabled, fill out the frame-rate statistics for the existing samples. The
 * window is a stack of up to 60 entries with the newest at index 0; each call shifts the existing samples
 * up by one and inserts the new timestamp/dropped flag at the front. Statistics are computed over the
 * pre-shift window: instantaneous fps (whole-window), sample-count average, min/max from the extreme
 * frame intervals, and the dropped-frame percentage.
 *
 * DEVIATION: the decompiler renders the PPC integer->float conversions (fcfid on 64-bit register pairs)
 * as __int128/OVERLAPPED dword shuffles with magic constants (0x82000000) and aliased high/low dwords.
 * Those are conversion artifacts only; restored here as plain (float) casts of the underlying integers. */

#include <stdint.h>
#include "headers/rasterizer_frame_statistics_s.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"


extern uint32_t system_milliseconds(void);

void rasterizer_frame_statistics_get_fps(rasterizer_frame_statistics_s *frame_statistics, uint8_t frame_dropped)
{
    int16_t new_sample_count = 0;

    if ((rasterizer_debug_options.fps_accumulation || rasterizer_debug_options.statistics_mode) && frame_statistics)
    {
        unsigned int current_time = system_milliseconds();
        int16_t dropped_count = 0;
        int16_t count = fps_sample_count;

        if (fps_sample_count)
        {
            unsigned int min_interval = current_time - time_samples[0];
            unsigned int max_interval = current_time - time_samples[0];

            for (int i = (int16_t)(fps_sample_count - 1); i > 0; i--)
            {
                if (i > 1)
                {
                    unsigned int interval = time_samples[i - 1] - time_samples[i];
                    if (interval <= min_interval)
                        min_interval = interval;
                    if (interval > max_interval)
                        max_interval = interval;
                }
                if (dropped_samples[i])
                    ++dropped_count;
                dropped_samples[i] = dropped_samples[i - 1];
                time_samples[i] = time_samples[i - 1];
            }

            unsigned int window_interval = current_time - time_samples[0];
            if (window_interval <= 1)
                window_interval = 1;
            frame_statistics->fps_sample_count = count;
            frame_statistics->fps = 1000.0f / (float)window_interval;

            unsigned int last_interval = current_time - time_samples[count - 1];
            if (last_interval <= 1)
                last_interval = 1;
            frame_statistics->fps_average = ((float)count * 1000.0f) / (float)last_interval;

            if (min_interval <= 1)
                min_interval = 1;
            frame_statistics->fps_max = 1000.0f / (float)min_interval;

            if (max_interval <= 1)
                max_interval = 1;
            frame_statistics->fps_min = 1000.0f / (float)max_interval;
            frame_statistics->frames_dropped_pct = ((float)dropped_count * 100.0f) / (float)count;
        }

        new_sample_count = count + 1;
        dropped_samples[0] = frame_dropped;
        time_samples[0] = current_time;
        if (count + 1 > 60)
        {
            fps_sample_count = 60;
            return;
        }
    }

    fps_sample_count = new_sample_count;
}
