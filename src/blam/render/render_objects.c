/* render_objects @0x836E7518 — render all rendered objects plus the first-person weapon, ordering the two
 * passes per rasterizer_debug_options.draw_first_person_weapon_first.
 *
 * The two-iteration toggle loop runs the object pass (process_rendered_objects) and the first-person weapon
 * pass (first_person_weapon_draw) in the order chosen by the debug option. Only object_render_data.shadow is
 * initialised (to 0); the remaining fields are left as-is, matching the original. Typed void — the int
 * returns are decompiler artifacts of the begin/end helpers being read as int. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/rasterizer_debug_options.h"

extern void rasterizer_models_begin(uint8_t sky);
extern void rasterizer_models_end(void);
extern void find_rendered_objects(void);
extern void first_person_weapon_draw(void);
extern void process_rendered_objects(object_render_data *data);

void render_objects(void)
{
    rasterizer_models_begin(0);
    find_rendered_objects();

    object_render_data data;
    data.shadow = 0;

    int pass = 0;
    do
    {
        if (pass == rasterizer_debug_options.draw_first_person_weapon_first)
            process_rendered_objects(&data);
        else
            first_person_weapon_draw(); /* attested void: r3-thread + phantom arg dropped */
        pass = (pass == 0);
    } while (pass);

    rasterizer_models_end();
}
