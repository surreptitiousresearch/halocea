/* hcex_precache_resources @0x836842B0 — HCEX startup precache pass: load the bitmap groups referenced by the
 * game-globals rasterizer config, precache decal bitmaps, predict every object tag definition, then per local
 * player precache the predicted resources of the BSP cluster the camera is in and the first-person weapon
 * assets. Each phase is bracketed by a named PIX profiling event.
 *
 * The rasterizer_data block's referenced bitmap-group indices are read as a raw int array (each
 * tag_reference is 16 bytes / 4 ints with its index at +12, so indices land at [3],[7],[11],... and a later
 * group at [46],[50],[54]); reproduced as the observed literal offsets. The DB gives no prototype; the
 * decompiler's `return hcex_pix_end_event(...)` tail calls and the arguments passed to hcex_pix_end_event are
 * spurious register leftovers (the event-end marker takes no argument) — rendered as a plain void function. */

#include <stdint.h>
#include "../headers/game_globals_definition.h"
#include "../headers/game_globals_rasterizer_data.h"
#include "../headers/structure_bsp.h"
#include "../headers/structure_cluster.h"
#include "../headers/observer_result.h"
#include "../headers/location.h"
#include "../headers/tag_iterator.h"
#include "../headers/tag_block.h"

#include "headers/tag_block.h"
extern "C" struct game_globals *global_game_globals;
extern "C" structure_bsp *global_structure_bsp; // non-const to match blam_data_globals.h

extern "C" void hcex_pix_begin_event(const char *fmt, ...); /* DEVIATION: variadic (std r4..r10 save-area prologue @0x823CED10) */
extern "C" void hcex_pix_end_event(void);
extern "C" void hcex_load_bitmap_group(int bitmap_group_index);
extern "C" void hcex_decals_precache_bitmaps(void);
extern "C" void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern "C" int tag_iterator_next(tag_iterator *iterator);
extern "C" void object_definition_predict(int definition_index);
extern "C" int16_t local_player_count(void);
extern "C" const observer_result *observer_get_camera(int16_t local_player_index);
extern "C" void scenario_location_from_point(location *location, const real_point3d *point);
extern "C" void predicted_resources_precache(const tag_block *predicted_resources);
extern "C" void first_person_weapon_predict(int16_t local_player_index);

extern "C" void hcex_precache_resources(void)
{
    if ( global_game_globals->rasterizer_data.count )
    {
        game_globals_rasterizer_data *rasterizer_data = (game_globals_rasterizer_data *)global_game_globals->rasterizer_data.address;
        if ( rasterizer_data )
        {
            hcex_pix_begin_event("precache_rasterizer");
            hcex_load_bitmap_group(rasterizer_data->distance_attenuation.index);
            hcex_load_bitmap_group(rasterizer_data->vector_normalization.index);
            hcex_load_bitmap_group(rasterizer_data->atmospheric_fog_density.index);
            hcex_load_bitmap_group(rasterizer_data->planar_fog_density.index);
            hcex_load_bitmap_group(rasterizer_data->linear_corner_fade.index);
            hcex_load_bitmap_group(rasterizer_data->active_camouflage_distortion.index);
            hcex_load_bitmap_group(rasterizer_data->glow.index);
            hcex_load_bitmap_group(rasterizer_data->default_textures[0].index);
            hcex_load_bitmap_group(rasterizer_data->default_textures[1].index);
            hcex_load_bitmap_group(rasterizer_data->default_textures[2].index);
            hcex_pix_end_event();
        }
    }

    hcex_pix_begin_event("precache_decals");
    hcex_decals_precache_bitmaps();
    hcex_pix_end_event();

    hcex_pix_begin_event("precache_obj");
    tag_iterator object_iterator;
    tag_iterator_new(&object_iterator, 0x6F626A65u); /* 'obje' */
    for ( int definition_index = tag_iterator_next(&object_iterator); definition_index != -1;
          definition_index = tag_iterator_next(&object_iterator) )
        object_definition_predict(definition_index);
    hcex_pix_end_event();

    hcex_pix_begin_event("precache_lm");
    for ( int local_player = 0; local_player < local_player_count(); ++local_player )
    {
        const observer_result *camera = observer_get_camera(local_player);
        location camera_location;
        scenario_location_from_point(&camera_location, &camera->position);
        if ( camera_location.cluster_index != -1 )
            predicted_resources_precache(&((structure_cluster *)global_structure_bsp->clusters.address)[
                camera_location.cluster_index].predicted_resources);
    }
    hcex_pix_end_event();

    hcex_pix_begin_event("precache_fp");
    for ( int local_player = 0; local_player < local_player_count(); ++local_player )
        first_person_weapon_predict(local_player);
    hcex_pix_end_event();
}
