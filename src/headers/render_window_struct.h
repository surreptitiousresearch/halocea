#pragma once
/* render_window struct TAG only (no typedef) — DB type 'render_window', 172
 * (0xAC) bytes. Split out from render_window.h so translation units that also
 * declare/define the render_window() FUNCTION (0x83707268) can use
 * `struct render_window` without the typedef colliding with the function name.
 * render_window.h includes this and adds the `render_window` typedef for the
 * many consumers that reference the type by its plain name. */

#include <stdint.h>
#include "render_camera.h"

struct render_window
{
    int16_t        local_player_index;  /* 0x00 */
    unsigned char  console_window;      /* 0x02 */
    unsigned char _pad0[1]; /* db-verified padding */
    struct render_camera  render_camera;       /* 0x04 — elaborated type: member name matches type */
    struct render_camera  rasterizer_camera;   /* 0x58 */
};
