/* main_present_frame @0x8368B4A0 — present the rendered frame, and when movie capture is active and no
 * screenshot is pending, export the frame as a sequentially numbered TGA. */

#include <stdint.h>
#include "headers/file_reference.h"
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled main_globals as a 2-field {movie@0, recording_frame_index@4}
 * struct; the DB-verified layout (main_globals.h) has movie@0x44 / recording_frame_index@0x50. The
 * field names match, so reusing the canonical header corrects the offsets. */


#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
extern void render_frame_present(const point2d *screenshot_index, bitmap_data *bitmap);
extern int  _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern const char *targa_export(file_reference *file, const bitmap_data *bitmap);

void main_present_frame(void)
{
    render_frame_present(0, main_globals.movie);

    if ( global_screenshot_count <= 0 && main_globals.movie )
    {
        int frame_index = main_globals.recording_frame_index++;
        file_reference ref;
        char path[512];

        _snprintf_0(path, 0x200u, "movie\\frame%06d.tga", frame_index);
        file_reference_create_from_path(&ref, path, 0);
        targa_export(&ref, main_globals.movie);
    }
}
