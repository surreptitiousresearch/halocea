/* main_loop_of_death @0x8368C368 — the standalone (editor/tool-build) render-only main loop: forever pump
 * input, idle the shell, update the event manager and UI widgets, render the pre-game frame and present it,
 * optionally dumping each frame to a movie\frameNNNNNN.tga while recording.
 *
 * Deviations:
 *   - input_frame_begin/input_update/shell_idle/event_manager_update/process_ui_widgets/main_pregame_render
 *     and input_frame_end are all void no-arg; the decompiler chained them through r3. Unchained.
 *   - targa_export's return feeds the (ignored) input_frame_end arg in the decompiler; dropped.
 *   - the decompiler's `__noreturn` attribute is dropped from the attested signature: the loop back-edge
 *     is a plain `b loc_8368C388` and the function genuinely never falls out, but the spelling is a
 *     Hex-Rays-ism, not part of the interface. */

#include <stdint.h>
#include "headers/file_reference.h"
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
extern void input_frame_begin(void);
extern void input_update(void);
extern void shell_idle(void);
extern void event_manager_update(void);
extern void process_ui_widgets(void);
extern void main_pregame_render(void);
extern void input_frame_end(void);
extern void render_frame_present(const point2d *screenshot_index, bitmap_data *bitmap);
extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern const char *targa_export(file_reference *file, const bitmap_data *bitmap);

void main_loop_of_death(void)
{
    while ( 1 )
    {
        input_frame_begin();
        input_update();
        shell_idle();
        event_manager_update();
        process_ui_widgets();
        main_pregame_render();
        render_frame_present(0, main_globals.movie);

        if ( global_screenshot_count <= 0 && main_globals.movie )
        {
            char path[544];
            int frame_index = main_globals.recording_frame_index++;
            _snprintf_0(path, 0x200u, "movie\\frame%06d.tga", frame_index);

            file_reference reference;
            file_reference_create_from_path(&reference, path, 0);
            targa_export(&reference, main_globals.movie);
        }

        input_frame_end();
    }
}
