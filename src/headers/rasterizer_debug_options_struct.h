#pragma once
/* rasterizer_debug_options_struct — global rasterizer debug/config block. */

typedef struct rasterizer_debug_options_struct
{
    unsigned __int8 fps_accumulation;                          /* 0x00 */
    char            _pad01[1];                                 /* 0x01 */
    __int16         statistics_mode;                           /* 0x02 */
    __int16         drawing_mode;                              /* 0x04 */
    unsigned __int8 wireframe_enabled;                         /* 0x06 */
    unsigned __int8 debug_model_vertices_enabled;              /* 0x07 */
    __int16         debug_model_lod;                           /* 0x08 */
    unsigned __int8 debug_transparent_geometry_enabled;        /* 0x0A */
    unsigned __int8 debug_meter_shader_enabled;                /* 0x0B */
    unsigned __int8 draw_models;                               /* 0x0C */
    unsigned __int8 draw_model_transparent_geometry;           /* 0x0D */
    unsigned __int8 draw_first_person_weapon_first;            /* 0x0E */
    unsigned __int8 stencil_mask_enabled;                      /* 0x0F */
    unsigned __int8 draw_environment;                          /* 0x10 */
    unsigned __int8 draw_environment_lightmaps;                /* 0x11 */
    unsigned __int8 draw_environment_shadows;                  /* 0x12 */
    unsigned __int8 draw_environment_diffuse_lights;           /* 0x13 */
    unsigned __int8 draw_environment_textures;                 /* 0x14 */
    unsigned __int8 draw_environment_decals;                   /* 0x15 */
    unsigned __int8 draw_environment_specular_lights;          /* 0x16 */
    unsigned __int8 draw_environment_specular_lightmaps;       /* 0x17 */
    unsigned __int8 draw_environment_reflection_lightmap_masks;/* 0x18 */
    unsigned __int8 draw_environment_reflection_mirrors;       /* 0x19 */
    unsigned __int8 draw_environment_reflections;              /* 0x1A */
    unsigned __int8 draw_environment_transparent_geometry;     /* 0x1B */
    unsigned __int8 draw_environment_fog;                      /* 0x1C */
    unsigned __int8 draw_environment_fog_screen;               /* 0x1D */
    unsigned __int8 draw_water;                                /* 0x1E */
    unsigned __int8 draw_lens_flares;                          /* 0x1F */
    unsigned __int8 draw_dynamic_unlit_geometry;               /* 0x20 */
    unsigned __int8 draw_dynamic_lit_geometry;                 /* 0x21 */
    unsigned __int8 draw_dynamic_screen_geometry;              /* 0x22 */
    unsigned __int8 draw_hud_motion_sensor;                    /* 0x23 */
    unsigned __int8 draw_detail_objects;                       /* 0x24 */
    unsigned __int8 draw_debug_geometry;                       /* 0x25 */
    unsigned __int8 debug_geometry_multipass;                  /* 0x26 */
    unsigned __int8 fog_atmospheric_enabled;                   /* 0x27 */
    unsigned __int8 fog_planar_enabled;                        /* 0x28 */
    unsigned __int8 bump_mapping_enabled;                      /* 0x29 */
    char            _pad2A[2];                                 /* 0x2A */
    float           lightmap_ambient;                          /* 0x2C */
    __int16         _lightmap_mode;                            /* 0x30 */
    __int16         pad3;                                      /* 0x32 — debug flag; ==69 enables verbose scoreboard-selection logging */
    unsigned __int8 lightmap_incident_radiosity_enabled;       /* 0x34 */
    unsigned __int8 lightmap_filtering_enabled;                /* 0x35 */
    char            _pad36[2];                                 /* 0x36 */
    float           model_lighting_ambient;                    /* 0x38 */
    unsigned __int8 environment_alpha_testing_enabled;         /* 0x3C */
    unsigned __int8 environment_specular_mask_enabled;         /* 0x3D */
    unsigned __int8 shadow_convolution_enabled;                /* 0x3E */
    unsigned __int8 shadow_debug_enabled;                      /* 0x3F */
    unsigned __int8 water_mipmapping_enabled;                  /* 0x40 */
    unsigned __int8 active_camouflage_enabled;                 /* 0x41 */
    unsigned __int8 active_camouflage_multipass_enabled;       /* 0x42 */
    unsigned __int8 plasma_energy_enabled;                     /* 0x43 */
    unsigned __int8 lens_flare_occlusion_enabled;              /* 0x44 */
    unsigned __int8 lens_flare_occlusion_debug;                /* 0x45 */
    unsigned __int8 lens_flare_sun_glow_enabled;               /* 0x46 */
    unsigned __int8 screen_flash_enabled;                      /* 0x47 */
    unsigned __int8 screen_effects_enabled;                    /* 0x48 */
    unsigned __int8 DXTC_noise_enabled;                        /* 0x49 */
    unsigned __int8 soft_filter_enabled;                       /* 0x4A */
    unsigned __int8 secondary_render_target_debug_enabled;     /* 0x4B */
    unsigned __int8 profile_log_enabled;                       /* 0x4C */
    char            _pad4D[3];                                 /* 0x4D */
    float           detail_object_screen_facing_offset_multiplier; /* 0x50 */
    float           zbias;                                     /* 0x54 */
    float           zoffset;                                   /* 0x58 */
    unsigned __int8 force_all_player_views_to_default_player;  /* 0x5C */
    unsigned __int8 safe_frame_bounds_adjust_enabled;          /* 0x5D */
    __int16         freeze_flying_camera;                      /* 0x5E */
    unsigned __int8 zsprite_enabled;                           /* 0x60 */
    unsigned __int8 filthy_decal_fog_hack_enabled;             /* 0x61 */
    unsigned __int8 smart_states_enabled;                      /* 0x62 */
    unsigned __int8 splitscreen_VB_optimization_enabled;       /* 0x63 */
    unsigned __int8 profile_print_locks;                       /* 0x64 */
    char            _pad65[3];                                 /* 0x65 */
    float           profile_objectlock_time;                   /* 0x68 */
    __int16         rasterizer_effects_level;                  /* 0x6C */
    __int16         rasterizer_texture_default_mip_level;      /* 0x6E */
    __int16         rasterizer_model_quality_level;            /* 0x70 */
    char            _pad72[2];                                 /* 0x72 */
    float           pad3_scale;                                /* 0x74 */
    float           f[6];                                      /* 0x78 */
} rasterizer_debug_options_struct;                             /* 144 bytes */

extern rasterizer_debug_options_struct rasterizer_debug_options;
