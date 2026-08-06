/* widget_type_definitions @ 0x8417DB98 (.data, 200 bytes)
 * DB applied_types: widget_type_definition widget_type_definitions[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 group_tag                  = 0x666C6167
 *        +0x04 needs_lighting             = 0x01
 *        +0x08 initialize                 = 0x8380DE18 -> flags_initialize
 *        +0x0C initialize_for_new_map     = 0x8380DE50 -> flags_initialize_for_new_map
 *        +0x10 dispose_from_old_map       = 0x8380DE60 -> flags_dispose_from_old_map
 *        +0x14 dispose                    = 0x8380DE70 -> flags_dispose
 *        +0x18 new_proc                   = 0x8380F408 -> flag_new
 *        +0x1C delete_proc                = 0x8380DFB8 -> flag_delete
 *        +0x20 update                     = 0x8380F698 -> flags_update
 *        +0x24 render_proc                = 0x8380F5E8 -> flag_render
 *   [ 1] +0x00 group_tag                  = 0x616E7421
 *        +0x04 needs_lighting             = 0x00
 *        +0x08 initialize                 = 0x8380D2C8 -> antennas_initialize
 *        +0x0C initialize_for_new_map     = 0x8380D300 -> antennas_initialize_for_new_map
 *        +0x10 dispose_from_old_map       = 0x8380D310 -> antennas_dispose_from_old_map
 *        +0x14 dispose                    = 0x8380D320 -> antennas_dispose
 *        +0x18 new_proc                   = 0x8380D340 -> antenna_new
 *        +0x1C delete_proc                = 0x8380D5B8 -> antenna_delete
 *        +0x20 update                     = 0x8380DD38 -> antennas_update
 *        +0x24 render_proc                = 0x8380DC68 -> antenna_render
 *   [ 2] +0x00 group_tag                  = 0x676C7721
 *        +0x04 needs_lighting             = 0x00
 *        +0x08 initialize                 = 0x8380C288 -> glow_initialize
 *        +0x0C initialize_for_new_map     = 0x8380B410 -> glow_initialize_for_new_map
 *        +0x10 dispose_from_old_map       = 0x8380B460 -> glow_dispose_from_old_map
 *        +0x14 dispose                    = 0x8380B408 -> glow_dispose
 *        +0x18 new_proc                   = 0x8380B4B0 -> glow_new
 *        +0x1C delete_proc                = 0x8380B590 -> glow_delete
 *        +0x20 update                     = 0x00000000
 *        +0x24 render_proc                = 0x8380D240 -> glow_submit
 *   [ 3] +0x00 group_tag                  = 0x6D677332
 *        +0x04 needs_lighting             = 0x00
 *        +0x08 initialize                 = 0x8380AA40 -> light_volumes_initialize
 *        +0x0C initialize_for_new_map     = 0x8380AA80 -> light_volumes_initialize_for_new_map
 *        +0x10 dispose_from_old_map       = 0x8380AA98 -> light_volumes_dispose_from_old_map
 *        +0x14 dispose                    = 0x8380AA78 -> light_volumes_dispose
 *        +0x18 new_proc                   = 0x8380AAB0 -> light_volume_new
 *        +0x1C delete_proc                = 0x8380AAF8 -> light_volume_delete
 *        +0x20 update                     = 0x00000000
 *        +0x24 render_proc                = 0x8380B150 -> light_volume_submit
 *   ... 1 further elements elided; full hex in .sweep/data_image.tsv
 * data-init widget_type_definition[5] (200 bytes,
 * 40 bytes each). One descriptor per object widget type; group_tag + needs_lighting +
 * 8 procedure pointers reconstructed from the binary (big-endian, all named funcs).
 */
#include <stdint.h>
#include "../headers/widget_type_definition.h"

#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
extern int antenna_new(int antenna_definition_index);
extern int flag_new(int flag_definition_index);
extern int glow_new(int glow_definition_index);
extern int light_volume_new(int definition_index);
extern int lightning_new(int definition_index);
extern void antenna_delete(int antenna_index);
extern void antenna_render(int object_index, int antenna_index, const render_lighting *lighting, const render_animation *animation);
extern void antennas_dispose(void);
extern void antennas_dispose_from_old_map(void);
extern void antennas_initialize(void);
extern void antennas_initialize_for_new_map(void);
extern void antennas_update(float dt);
extern void flag_delete(int flag_index);
extern void flag_render(int object_index, int flag_index, const render_lighting *lighting, const render_animation *animation);
extern void flags_dispose(void);
extern void flags_dispose_from_old_map(void);
extern void flags_initialize(void);
extern void flags_initialize_for_new_map(void);
extern void flags_update(float dt);
extern void glow_delete(int glow_index);
extern void glow_dispose(void);
extern void glow_dispose_from_old_map(void);
extern void glow_initialize(void);
extern void glow_initialize_for_new_map(void);
extern void glow_submit(int object_index, int widget_index, const render_lighting *lighting, const render_animation *animation);
extern void light_volume_delete(int light_volume_index);
extern void light_volume_submit(int object_index, int widget_index, const render_lighting *lighting, const render_animation *animation);
extern void light_volumes_dispose(void);
extern void light_volumes_dispose_from_old_map(void);
extern void light_volumes_initialize(void);
extern void light_volumes_initialize_for_new_map(void);
extern void lightning_delete(int lightning_index);
extern void lightning_submit(int object_index, int widget_index, const render_lighting *lighting, const render_animation *animation);
extern void lightnings_dispose(void);
extern void lightnings_dispose_from_old_map(void);
extern void lightnings_initialize(void);
extern void lightnings_initialize_for_new_map(void);

widget_type_definition widget_type_definitions[5] =
{
    {
        0x666C6167u, /* group_tag 'flag' */
        1,           /* needs_lighting */
        { 0, 0, 0 },  /* _pad05 */
        flags_initialize, /* initialize */
        flags_initialize_for_new_map, /* initialize_for_new_map */
        flags_dispose_from_old_map, /* dispose_from_old_map */
        flags_dispose, /* dispose */
        flag_new, /* new_proc */
        flag_delete, /* delete_proc */
        flags_update, /* update */
        flag_render, /* render_proc */
    },
    {
        0x616E7421u, /* group_tag 'ant!' */
        0,           /* needs_lighting */
        { 0, 0, 0 },  /* _pad05 */
        antennas_initialize, /* initialize */
        antennas_initialize_for_new_map, /* initialize_for_new_map */
        antennas_dispose_from_old_map, /* dispose_from_old_map */
        antennas_dispose, /* dispose */
        antenna_new, /* new_proc */
        antenna_delete, /* delete_proc */
        antennas_update, /* update */
        antenna_render, /* render_proc */
    },
    {
        0x676C7721u, /* group_tag 'glw!' */
        0,           /* needs_lighting */
        { 0, 0, 0 },  /* _pad05 */
        glow_initialize, /* initialize */
        glow_initialize_for_new_map, /* initialize_for_new_map */
        glow_dispose_from_old_map, /* dispose_from_old_map */
        glow_dispose, /* dispose */
        glow_new, /* new_proc */
        glow_delete, /* delete_proc */
        0, /* update */
        glow_submit, /* render_proc */
    },
    {
        0x6D677332u, /* group_tag 'mgs2' */
        0,           /* needs_lighting */
        { 0, 0, 0 },  /* _pad05 */
        light_volumes_initialize, /* initialize */
        light_volumes_initialize_for_new_map, /* initialize_for_new_map */
        light_volumes_dispose_from_old_map, /* dispose_from_old_map */
        light_volumes_dispose, /* dispose */
        light_volume_new, /* new_proc */
        light_volume_delete, /* delete_proc */
        0, /* update */
        light_volume_submit, /* render_proc */
    },
    {
        0x656C6563u, /* group_tag 'elec' */
        0,           /* needs_lighting */
        { 0, 0, 0 },  /* _pad05 */
        lightnings_initialize, /* initialize */
        lightnings_initialize_for_new_map, /* initialize_for_new_map */
        lightnings_dispose_from_old_map, /* dispose_from_old_map */
        lightnings_dispose, /* dispose */
        lightning_new, /* new_proc */
        lightning_delete, /* delete_proc */
        0, /* update */
        lightning_submit, /* render_proc */
    },
};
