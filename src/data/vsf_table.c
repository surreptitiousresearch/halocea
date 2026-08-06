/* vsf_table @ 0x84177D58 (.data, 520 bytes)
 * DB applied_types: vertex_shader_function vsf_table[65];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821153C4 -> "convolution"
 *   [ 1] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82042208 -> "debug"
 *   [ 2] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82036BD0 -> "decal"
 *   [ 3] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821153B0 -> "detail_object_type0"
 *   [ 4] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x8211539C -> "detail_object_type1"
 *   [ 5] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x8207EDC4 -> "effect"
 *   [ 6] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115388 -> "effect_multitexture"
 *   [ 7] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115368 -> "effect_multitexture_screenspace"
 *   [ 8] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115358 -> "effect_zsprite"
 *   [ 9] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x8211533C -> "environment_diffuse_light"
 *   [10] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x8211531C -> "environment_diffuse_light_ff"
 *   [11] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x8211530C -> "environment_fog"
 *   [12] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821152F4 -> "environment_fog_screen"
 *   [13] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821152DC -> "environment_lightmap"
 *   [14] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821152BC -> "environment_reflection_bumped"
 *   [15] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x821152A0 -> "environment_reflection_flat"
 *   [16] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115278 -> "environment_reflection_lightmap_mask"
 *   [17] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115258 -> "environment_reflection_mirror"
 *   [18] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115234 -> "environment_reflection_radiosity"
 *   [19] +0x00 shader                     = 0x00000000
 *        +0x04 filename                   = 0x82115220 -> "environment_shadow"
 *   ... 45 further elements elided; full hex in .sweep/data_image.tsv
 * Element type and count are PROVEN independently of the (agreeing) applied_types decl:
 * rasterizer_dx9_shaders_vshader9_load_vso_files @0x83723720 forms the object address directly
 * (`lis r11, vsf_table@ha` / `addi r29, r11, vsf_table@l` - no pointer load), sets its cursor to
 * `addi r31, r29, 4`, tests `lwz r11, 0(r31)` for null, and advances `addi r31, r31, 8` until
 * `r29 + 0x20C`. Stride 8 == sizeof(vertex_shader_function); the cursor sits at +4, so the string
 * pointer is the SECOND word of the element, not the first; 0x20C - 4 == 520 == 65 * 8.
 * rasterizer_dx9_shaders_vshader9_unload_vso_file @0x837237A0 confirms the stride from the other
 * direction with `slwi r30, r3, 3` (index * 8).
 * Slot order is confirmed a second time by the DB enum rasterizer_vertex_shader_index
 * ($48C73337F1F6FA5002623CD6EBB0FE21, 65 values + NUMBER_OF_VERTEX_SHADERS): every image filename
 * is its slot's `_vs_` enumerator name. Those names are carried as trailing comments.
 *
 * `shader` is zero in all 65 slots - it is filled at runtime by
 * rasterizer_dx9_shaders_vshader9_load_vso_file (D3DDevice_CreateVertexShader) and cleared by
 * rasterizer_dx9_shaders_vshader9_dispose. Exactly 260 of the 520 image bytes are non-zero: the
 * 65 four-byte `filename` pointers and nothing else.
 * DEVIATION: the previous definition was uninitialised (`vertex_shader_function vsf_table[65];`)
 * and described as data-bss. The .data record is not zero-fill - it carries all 65 .rdata filename
 * pointers, so a tentative definition handed the loader a table of null names and every vso file
 * would have been skipped by the `if (vsf_table[i].filename)` guard in _load_vso_files. Caught
 * 2026-08-06 by data_provenance.py --verify against the .data record.
 */
#include "../headers/vertex_shader_function.h"

vertex_shader_function vsf_table[65] =
{
    { 0, "convolution"                           },   /* 0  _vs_convolution */
    { 0, "debug"                                 },   /* 1  _vs_debug */
    { 0, "decal"                                 },   /* 2  _vs_decal */
    { 0, "detail_object_type0"                   },   /* 3  _vs_detail_object_type0 */
    { 0, "detail_object_type1"                   },   /* 4  _vs_detail_object_type1 */
    { 0, "effect"                                },   /* 5  _vs_effect */
    { 0, "effect_multitexture"                   },   /* 6  _vs_effect_multitexture */
    { 0, "effect_multitexture_screenspace"       },   /* 7  _vs_effect_multitexture_screenspace */
    { 0, "effect_zsprite"                        },   /* 8  _vs_effect_zsprite */
    { 0, "environment_diffuse_light"             },   /* 9  _vs_environment_diffuse_light */
    { 0, "environment_diffuse_light_ff"          },   /* 10 _vs_environment_diffuse_light_ff */
    { 0, "environment_fog"                       },   /* 11 _vs_environment_fog */
    { 0, "environment_fog_screen"                },   /* 12 _vs_environment_fog_screen */
    { 0, "environment_lightmap"                  },   /* 13 _vs_environment_lightmap */
    { 0, "environment_reflection_bumped"         },   /* 14 _vs_environment_reflection_bumped */
    { 0, "environment_reflection_flat"           },   /* 15 _vs_environment_reflection_flat */
    { 0, "environment_reflection_lightmap_mask"  },   /* 16 _vs_environment_reflection_lightmap_mask */
    { 0, "environment_reflection_mirror"         },   /* 17 _vs_environment_reflection_mirror */
    { 0, "environment_reflection_radiosity"      },   /* 18 _vs_environment_reflection_radiosity */
    { 0, "environment_shadow"                    },   /* 19 _vs_environment_shadow */
    { 0, "environment_specular_light"            },   /* 20 _vs_environment_specular_light */
    { 0, "environment_specular_spot_light"       },   /* 21 _vs_environment_specular_spot_light */
    { 0, "environment_specular_lightmap"         },   /* 22 _vs_environment_specular_lightmap */
    { 0, "environment_texture"                   },   /* 23 _vs_environment_texture */
    { 0, "lens_flare"                            },   /* 24 _vs_lens_flare */
    { 0, "model_fogged"                          },   /* 25 _vs_model_fogged */
    { 0, "model"                                 },   /* 26 _vs_model */
    { 0, "model_ff"                              },   /* 27 _vs_model_skin_ff */
    { 0, "model_fast"                            },   /* 28 _vs_model_fast */
    { 0, "model_scenery"                         },   /* 29 _vs_model_scenery */
    { 0, "model_active_camouflage"               },   /* 30 _vs_model_active_camouflage */
    { 0, "model_active_camouflage_ff"            },   /* 31 _vs_model_active_camouflage_ff */
    { 0, "model_fog_screen"                      },   /* 32 _vs_model_fog_screen */
    { 0, "model_shadow"                          },   /* 33 _vs_model_shadow */
    { 0, "model_zbuffer"                         },   /* 34 _vs_model_zbuffer */
    { 0, "screen"                                },   /* 35 _vs_screen */
    { 0, "screen2"                               },   /* 36 _vs_screen2 */
    { 0, "transparent_generic"                   },   /* 37 _vs_transparent_generic */
    { 0, "transparent_generic_lit_m"             },   /* 38 _vs_transparent_generic_lit_m */
    { 0, "transparent_generic_m"                 },   /* 39 _vs_transparent_generic_m */
    { 0, "transparent_generic_object_centered"   },   /* 40 _vs_transparent_generic_object_centered */
    { 0, "transparent_generic_object_centered_m" },   /* 41 _vs_transparent_generic_object_centered_m */
    { 0, "transparent_generic_reflection"        },   /* 42 _vs_transparent_generic_reflection */
    { 0, "transparent_generic_reflection_m"      },   /* 43 _vs_transparent_generic_reflection_m */
    { 0, "transparent_generic_screenspace"       },   /* 44 _vs_transparent_generic_screenspace */
    { 0, "transparent_generic_screenspace_m"     },   /* 45 _vs_transparent_generic_screenspace_m */
    { 0, "transparent_generic_viewer_centered"   },   /* 46 _vs_transparent_generic_viewer_centered */
    { 0, "transparent_generic_viewer_centered_m" },   /* 47 _vs_transparent_generic_viewer_centered_m */
    { 0, "transparent_glass_diffuse_light"       },   /* 48 _vs_transparent_glass_diffuse_light */
    { 0, "transparent_glass_diffuse_light_m"     },   /* 49 _vs_transparent_glass_diffuse_light_m */
    { 0, "transparent_glass_reflection_bumped"   },   /* 50 _vs_transparent_glass_reflection_bumped */
    { 0, "transparent_glass_reflection_bumped_m" },   /* 51 _vs_transparent_glass_reflection_bumped_m */
    { 0, "transparent_glass_reflection_flat"     },   /* 52 _vs_transparent_glass_reflection_flat */
    { 0, "transparent_glass_reflection_flat_m"   },   /* 53 _vs_transparent_glass_reflection_flat_m */
    { 0, "transparent_glass_reflection_mirror"   },   /* 54 _vs_transparent_glass_reflection_mirror */
    { 0, "transparent_glass_tint"                },   /* 55 _vs_transparent_glass_tint */
    { 0, "transparent_glass_tint_m"              },   /* 56 _vs_transparent_glass_tint_m */
    { 0, "transparent_meter"                     },   /* 57 _vs_transparent_meter */
    { 0, "transparent_meter_m"                   },   /* 58 _vs_transparent_meter_m */
    { 0, "transparent_plasma_m"                  },   /* 59 _vs_transparent_plasma_m */
    { 0, "transparent_water_opacity"             },   /* 60 _vs_transparent_water_opacity */
    { 0, "transparent_water_opacity_m"           },   /* 61 _vs_transparent_water_opacity_m */
    { 0, "transparent_water_reflection"          },   /* 62 _vs_transparent_water_reflection */
    { 0, "transparent_water_reflection_m"        },   /* 63 _vs_transparent_water_reflection_m */
    { 0, "widget"                                },   /* 64 _vs_widget */
};
