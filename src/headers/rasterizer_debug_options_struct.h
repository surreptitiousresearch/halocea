#pragma once
#include <stdint.h>
/* rasterizer_debug_options_struct — global rasterizer debug/config block. */

typedef struct rasterizer_debug_options_struct
{
    uint8_t fps_accumulation;                          /* 0x00 */
    char            _pad01[1];                                 /* 0x01 */
    int16_t         statistics_mode;                           /* 0x02 */
    int16_t         drawing_mode;                              /* 0x04 */
    uint8_t wireframe_enabled;                         /* 0x06 */
    uint8_t debug_model_vertices_enabled;              /* 0x07 */
    int16_t         debug_model_lod;                           /* 0x08 */
    uint8_t debug_transparent_geometry_enabled;        /* 0x0A */
    uint8_t debug_meter_shader_enabled;                /* 0x0B */
    uint8_t draw_models;                               /* 0x0C */
    uint8_t draw_model_transparent_geometry;           /* 0x0D */
    uint8_t draw_first_person_weapon_first;            /* 0x0E */
    uint8_t stencil_mask_enabled;                      /* 0x0F */
    uint8_t draw_environment;                          /* 0x10 */
    uint8_t draw_environment_lightmaps;                /* 0x11 */
    uint8_t draw_environment_shadows;                  /* 0x12 */
    uint8_t draw_environment_diffuse_lights;           /* 0x13 */
    uint8_t draw_environment_textures;                 /* 0x14 */
    uint8_t draw_environment_decals;                   /* 0x15 */
    uint8_t draw_environment_specular_lights;          /* 0x16 */
    uint8_t draw_environment_specular_lightmaps;       /* 0x17 */
    uint8_t draw_environment_reflection_lightmap_masks;/* 0x18 */
    uint8_t draw_environment_reflection_mirrors;       /* 0x19 */
    uint8_t draw_environment_reflections;              /* 0x1A */
    uint8_t draw_environment_transparent_geometry;     /* 0x1B */
    uint8_t draw_environment_fog;                      /* 0x1C */
    uint8_t draw_environment_fog_screen;               /* 0x1D */
    uint8_t draw_water;                                /* 0x1E */
    uint8_t draw_lens_flares;                          /* 0x1F */
    uint8_t draw_dynamic_unlit_geometry;               /* 0x20 */
    uint8_t draw_dynamic_lit_geometry;                 /* 0x21 */
    uint8_t draw_dynamic_screen_geometry;              /* 0x22 */
    uint8_t draw_hud_motion_sensor;                    /* 0x23 */
    uint8_t draw_detail_objects;                       /* 0x24 */
    uint8_t draw_debug_geometry;                       /* 0x25 */
    uint8_t debug_geometry_multipass;                  /* 0x26 */
    uint8_t fog_atmospheric_enabled;                   /* 0x27 */
    uint8_t fog_planar_enabled;                        /* 0x28 */
    uint8_t bump_mapping_enabled;                      /* 0x29 */
    char            _pad2A[2];                                 /* 0x2A */
    float           lightmap_ambient;                          /* 0x2C */
    int16_t         _lightmap_mode;                            /* 0x30 */
    int16_t         pad3;                                      /* 0x32 — debug flag; ==69 enables verbose scoreboard-selection logging */
    uint8_t lightmap_incident_radiosity_enabled;       /* 0x34 */
    uint8_t lightmap_filtering_enabled;                /* 0x35 */
    char            _pad36[2];                                 /* 0x36 */
    float           model_lighting_ambient;                    /* 0x38 */
    uint8_t environment_alpha_testing_enabled;         /* 0x3C */
    uint8_t environment_specular_mask_enabled;         /* 0x3D */
    uint8_t shadow_convolution_enabled;                /* 0x3E */
    uint8_t shadow_debug_enabled;                      /* 0x3F */
    uint8_t water_mipmapping_enabled;                  /* 0x40 */
    uint8_t active_camouflage_enabled;                 /* 0x41 */
    uint8_t active_camouflage_multipass_enabled;       /* 0x42 */
    uint8_t plasma_energy_enabled;                     /* 0x43 */
    uint8_t lens_flare_occlusion_enabled;              /* 0x44 */
    uint8_t lens_flare_occlusion_debug;                /* 0x45 */
    uint8_t lens_flare_sun_glow_enabled;               /* 0x46 */
    uint8_t screen_flash_enabled;                      /* 0x47 */
    uint8_t screen_effects_enabled;                    /* 0x48 */
    uint8_t DXTC_noise_enabled;                        /* 0x49 */
    uint8_t soft_filter_enabled;                       /* 0x4A */
    uint8_t secondary_render_target_debug_enabled;     /* 0x4B */
    uint8_t profile_log_enabled;                       /* 0x4C */
    char            _pad4D[3];                                 /* 0x4D */
    float           detail_object_screen_facing_offset_multiplier; /* 0x50 */
    float           zbias;                                     /* 0x54 */
    float           zoffset;                                   /* 0x58 */
    uint8_t force_all_player_views_to_default_player;  /* 0x5C */
    uint8_t safe_frame_bounds_adjust_enabled;          /* 0x5D */
    int16_t         freeze_flying_camera;                      /* 0x5E */
    uint8_t zsprite_enabled;                           /* 0x60 */
    uint8_t filthy_decal_fog_hack_enabled;             /* 0x61 */
    uint8_t smart_states_enabled;                      /* 0x62 */
    uint8_t splitscreen_VB_optimization_enabled;       /* 0x63 */
    uint8_t profile_print_locks;                       /* 0x64 */
    char            _pad65[3];                                 /* 0x65 */
    float           profile_objectlock_time;                   /* 0x68 */
    int16_t         rasterizer_effects_level;                  /* 0x6C */
    int16_t         rasterizer_texture_default_mip_level;      /* 0x6E */
    int16_t         rasterizer_model_quality_level;            /* 0x70 */
    char            _pad72[2];                                 /* 0x72 */
    float           pad3_scale;                                /* 0x74 */
    float           f[6];                                      /* 0x78 */
} rasterizer_debug_options_struct;                             /* 144 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern rasterizer_debug_options_struct rasterizer_debug_options;

#ifdef __cplusplus
}
#endif
