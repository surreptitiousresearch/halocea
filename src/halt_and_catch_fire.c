/* halt_and_catch_fire @0x8368AB68 — the engine's terminal fatal-error handler ("halt and catch fire"): it never
 * returns and instead renders a static error screen forever. Guarded by recursion_lock so a fault raised while it
 * is already running falls straight through to exit_23(0). On first entry it locks, stops every gamepad's
 * rumble, and picks a font (the interface font, or the "old tags\internal system plain" fallback when there is no
 * scenario or no interface font). It then spins an endless present loop: reset the rasterizer, begin a frame and
 * window with a default origin-facing camera (40-degree half-FOV narrowed by a 0.75 aspect into a vertical FOV,
 * blue atmospheric fog), draw the build banner and the current error string, flush the transparent-geometry and
 * debug passes, and present — reading input each iteration so the machine stays responsive at the error screen.
 *
 * DEVIATION: reg-alloc-failure decompile. The vertical FOV pun is 2*atan2(tan(0.698..)*0.75, 1.0) (0.698 rad ~
 * 40 deg). viewport_bounds is the packed literal 31457920 = {y0 0, x0 0, y1 480, x1 0} (disasm 0x8368AD1C-30).
 * The rasterizer end chain (debug_draw / window_end / windows_end / frame_end) is four no-arg calls; the
 * decompiler's threaded return values are spurious (disasm 0x8368AE28-34). The frame-begin parameters are a
 * zeroed 16-byte block (the OVERLAPPED v0 fill). global_real_rgb_blue / global_real_argb_white are pointer
 * globals, used by dereference / by direct pass. */

#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_frame_begin_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/render_globals.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/scenario.h"
#include <string.h>
#include <math.h>
#include "headers/rasterizer_target.h"
#include "headers/interface_tag_index.h"
#include "headers/text_justification.h"
#include "headers/blam_data_globals.h"


#include "headers/real_argb_color.h"
#include "headers/render_camera.h"
#include "headers/real_rectangle2d.h"
#include "headers/render_frustum.h"
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern void input_set_gamepad_vibrate_state(int16_t gamepad_index, uint16_t left_speed, uint16_t right_speed);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern void rasterizer_reset_state(void);
extern void rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters);
extern uint8_t rasterizer_windows_begin(void);
extern void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern void draw_string_set_color(const real_argb_color *color);
extern void rasterizer_draw_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const char *string);
extern char * error_get(void);
extern void rasterizer_transparent_geometry_draw(uint8_t water);
extern void rasterizer_debug_draw(void);
extern void rasterizer_window_end(void);
extern void rasterizer_windows_end(void);
extern void rasterizer_frame_end(void);
extern void rasterizer_present(bitmap_data *screenshot_bitmap, const point2d *screenshot_index);
extern void input_update(void);
extern void exit_23(int code);

void halt_and_catch_fire(void)
{
    if ( !recursion_lock )
    {
        recursion_lock = 1;

        for ( __int16 gamepad = 0; gamepad < 4; ++gamepad )
        {
            if ( input_has_gamepad(gamepad) )
                input_set_gamepad_vibrate_state(gamepad, 0, 0);
        }

        int font_tag_index;
        if ( !global_scenario || (font_tag_index = interface_get_tag_index(_interface_font_terminal)) == -1 )
            font_tag_index = tag_loaded(0x666F6E74, "old tags\\internal system plain");   /* 'font' */

        while ( 1 )
        {
            rasterizer_reset_state();

            rasterizer_frame_begin_parameters frame_params;
            memset(&frame_params, 0, sizeof(frame_params));
            rasterizer_frame_begin(&frame_params);
            rasterizer_windows_begin();

            rasterizer_window_begin_parameters window;
            memset(&window, 0, sizeof(window));
            window.camera.position = *global_origin3d;
            window.camera.forward = *global_forward3d;
            window.camera.up = *global_up3d;
            window.camera.mirrored = 0;
            window.camera.vertical_field_of_view =
                    ((float)atan2((float)(tan(0.6981316804885864) * 0.75), 1.0) * 2.0f);
            window.camera.z_near = rasterizer_globals.z_near;
            window.camera.z_far = rasterizer_globals.z_far;
            window.camera.viewport_bounds.n[0] = 0;    /* packed literal 31457920: {y0,x0,y1,x1} = {0,0,480,0} */
            window.camera.viewport_bounds.n[1] = 0;
            window.camera.viewport_bounds.n[2] = 480;
            window.camera.viewport_bounds.n[3] = 0;
            render_camera_build_frustum(&window.camera, nullptr, &window.frustum, 1);
            window.rasterizer_target = _rasterizer_target_render_primary;
            window.fog.atmospheric_maximum_distance = 0.0f;
            window.fog.atmospheric_minimum_distance = 0.0f;
            window.fog.planar_mode = 0;
            window.fog.atmospheric_color = *global_real_rgb_blue;
            memcpy(&render.camera, &window.camera, sizeof(render.camera));
            rasterizer_window_begin(&window);

            if ( font_tag_index != -1 )
            {
                point2d cursor[2];
                cursor[0].n[0] = 0;
                cursor[0].n[1] = 0;
                rectangle2d frame_bounds = rasterizer_globals.frame_bounds;
                draw_string_set_draw_mode(font_tag_index, -1, _text_justification_left, 0, global_real_argb_white);
                draw_string_set_tab_stops(nullptr, 0);
                draw_string_set_color(global_real_argb_white);
                rasterizer_draw_string(&frame_bounds, nullptr, cursor, -4,
                        "halo xbox 01.00.01.0563 built at: Jun 24 2011 13:31:56");
                frame_bounds.n[0] = cursor[0].n[1] - 1;
                const char *error_text = error_get();
                rasterizer_draw_string(&frame_bounds, nullptr, cursor, -4, error_text);
            }

            rasterizer_transparent_geometry_draw(1);
            rasterizer_transparent_geometry_draw(0);
            rasterizer_debug_draw();
            rasterizer_window_end();
            rasterizer_windows_end();
            rasterizer_frame_end();
            rasterizer_present(nullptr, nullptr);
            input_update();
        }
    }

    exit_23(0);
}
