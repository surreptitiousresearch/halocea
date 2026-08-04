#pragma once
/* render_window @ 172 (0xAC) bytes — one rendered viewport: the player it follows, whether it is the
 * console overlay window, and the render/rasterizer camera pair. */

#include "render_camera.h"
#include "render_window_struct.h"

/* No `render_window` typedef: the ordinary-namespace name is taken by the FUNCTION
 * render_window @0x83707268; the type is the bare tag `struct render_window`
 * (render_window_struct.h). Consumers write `struct render_window`. */
