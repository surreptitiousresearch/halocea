/* shader_table @ 0x84174998 (.data, 3936 bytes)
 * DB applied_types: rasterizer_dx9_shader shader_table[123];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x0000000D
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x82110824 -> "env_lm_normal"
 *   [ 1] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x0000000D
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x82110810 -> "env_lm_no_lightmap"
 *   [ 2] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x0000000D
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x821107F8 -> "env_lm_no_illumination"
 *   [ 3] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x0000000D
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x821107D4 -> "env_lm_no_illumination_no_lightmap"
 *   [ 4] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x00000009
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x821107C0 -> "env_diffuse_lights"
 *   [ 5] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x00000017
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x8211079C -> "env_tex_normal_bias_mul_bias_mul"
 *   [ 6] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x00000017
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x82110780 -> "env_tex_normal_bias_mul_mul"
 *   [ 7] +0x00 effect                     = 0x00000000
 *        +0x04 vshader9                   = 0x00000017
 *        +0x08 texture                    = 0x00000000000000000000000000000000
 *        +0x18 constants                  = 0x00000000
 *        +0x1C filename                   = 0x8211075C -> "env_tex_normal_bias_mul_bias_add"
 *   ... 115 further elements elided; full hex in .sweep/data_image.tsv
 * The remaining 115 elements are carried by the initializer below, each preceded by its
 * rasterizer_dx9_shader_index slot name.
 *
 * Element type and count are PROVEN independently of the (agreeing) applied_types decl:
 * rasterizer_shader_select @0x836A1E18 forms the object address directly (`lis r9,
 * shader_table@ha` / `addi r27, r9, shader_table@l` - no pointer load), sets its cursor to
 * `addi r29, r27, 0x1C`, passes `lwz r4, 0(r29)` as the `%s` argument of hcex_output_dbg - so
 * +0x1C is `char *filename` - and advances `addi r29, r29, 0x20`. Stride 32 ==
 * sizeof(rasterizer_dx9_shader) and 3936 / 32 == 123 elements exactly; every re-sourced consumer
 * (rasterizer_dx9_shaders_load_from_ascii, _dispose, _unload_effects, rasterizer_shader_select)
 * bounds its loop at 123.
 * Slot order is confirmed a second time by the DB enum rasterizer_dx9_shader_index
 * ($70114256E1A9771FAD92C7E0FE4A9FE8, 123 values + NUMBER_OF_SHADERS): each image filename is the
 * abbreviated spelling of its slot's `_dxshader_` enumerator (`env_lm_normal` for
 * _dxshader_environment_lightmap_normal). Each element below is labelled with its slot name.
 *
 * `vshader9` holds a vsf_table slot index. No re-sourced consumer reads the field yet, so the
 * evidence is the image itself: all 123 words are in [0, 64] or are -1, the vertex-shader table
 * has exactly 65 entries, and every value names the vertex shader its pixel shader needs
 * (env_lm_* -> _vs_environment_lightmap, screen_multitex_* -> _vs_screen2, active_camouflage_draw
 * -> _vs_model_active_camouflage). -1 is the eight slots with no vertex shader of their own. The
 * `_vs_` enumerators below come from rasterizer_vertex_shader_index.h and are byte-identical to
 * the image words.
 *
 * `effect`, `texture[4]` and `constants` are zero in all 123 slots - they are filled at runtime by
 * rasterizer_dx9_shaders_load_from_ascii / _init_effect (ID3DXEffect_GetParameterByName) and
 * rasterizer_dx9_shaders_initialize (dlMalloc), and released by rasterizer_dx9_shaders_dispose.
 * Exactly 625 of the 3936 image bytes are non-zero: the 123 `filename` pointers plus the non-zero
 * `vshader9` words.
 * DEVIATION: the previous definition was uninitialised (`rasterizer_dx9_shader shader_table[123];`)
 * and described as data-bss zero-fill. The .data record is not zero - it carries every filename
 * pointer and every vertex-shader binding, so a tentative definition handed the engine 123 null
 * effect names (D:\shaders\fx\<null>.fx) and vertex-shader slot 0 (_vs_convolution) wherever the
 * image says -1 or another slot. Caught 2026-08-06 by data_provenance.py --verify against the
 * .data record.
 */
#include "../headers/rasterizer_dx9_shader_table.h"
#include "../headers/rasterizer_vertex_shader_index.h"

/* effect, vshader9, texture[4], constants, filename */
rasterizer_dx9_shader shader_table[123] =
{
    /* 0   _dxshader_environment_lightmap_normal */
    { 0, _vs_environment_lightmap                , { 0 }, 0, "env_lm_normal"                         },
    /* 1   _dxshader_environment_lightmap_no_lightmap */
    { 0, _vs_environment_lightmap                , { 0 }, 0, "env_lm_no_lightmap"                    },
    /* 2   _dxshader_environment_lightmap_no_illumination */
    { 0, _vs_environment_lightmap                , { 0 }, 0, "env_lm_no_illumination"                },
    /* 3   _dxshader_environment_lightmap_no_illumination_no_lightmap */
    { 0, _vs_environment_lightmap                , { 0 }, 0, "env_lm_no_illumination_no_lightmap"    },
    /* 4   _dxshader_environment_diffuse_lights */
    { 0, _vs_environment_diffuse_light           , { 0 }, 0, "env_diffuse_lights"                    },
    /* 5   _dxshader_env_tex_normal_biased_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_mul_bias_mul"      },
    /* 6   _dxshader_env_tex_normal_biased_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_mul_mul"           },
    /* 7   _dxshader_env_tex_normal_biased_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_mul_bias_add"      },
    /* 8   _dxshader_env_tex_normal_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_mul_bias_mul"           },
    /* 9   _dxshader_env_tex_normal_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_mul_mul"                },
    /* 10  _dxshader_env_tex_normal_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_mul_bias_add"           },
    /* 11  _dxshader_env_tex_normal_biased_add_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_add_bias_mul"      },
    /* 12  _dxshader_env_tex_normal_biased_add_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_add_mul"           },
    /* 13  _dxshader_env_tex_normal_biased_add_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_normal_bias_add_bias_add"      },
    /* 14  _dxshader_env_tex_blended_biased_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_mul_bias_mul"     },
    /* 15  _dxshader_env_tex_blended_biased_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_mul_mul"          },
    /* 16  _dxshader_env_tex_blended_biased_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_mul_bias_add"     },
    /* 17  _dxshader_env_tex_blended_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_mul_bias_mul"          },
    /* 18  _dxshader_env_tex_blended_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_mul_mul"               },
    /* 19  _dxshader_env_tex_blended_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_mul_bias_add"          },
    /* 20  _dxshader_env_tex_blended_biased_add_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_add_bias_mul"     },
    /* 21  _dxshader_env_tex_blended_biased_add_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_add_mul"          },
    /* 22  _dxshader_env_tex_blended_biased_add_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_blended_bias_add_bias_add"     },
    /* 23  _dxshader_env_tex_specular_mask_biased_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_mul_bias_mul"   },
    /* 24  _dxshader_env_tex_specular_mask_biased_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_mul_mul"        },
    /* 25  _dxshader_env_tex_specular_mask_biased_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_mul_bias_add"   },
    /* 26  _dxshader_env_tex_specular_mask_multiply_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_mul_bias_mul"        },
    /* 27  _dxshader_env_tex_specular_mask_multiply_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_mul_mul"             },
    /* 28  _dxshader_env_tex_specular_mask_multiply_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_mul_bias_add"        },
    /* 29  _dxshader_env_tex_specular_mask_biased_add_biased_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_add_bias_mul"   },
    /* 30  _dxshader_env_tex_specular_mask_biased_add_multiply */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_add_mul"        },
    /* 31  _dxshader_env_tex_specular_mask_biased_add_biased_add */
    { 0, _vs_environment_texture                 , { 0 }, 0, "env_tex_spec_mask_bias_add_bias_add"   },
    /* 32  _dxshader_environment_reflection_bumped */
    { 0, _vs_environment_reflection_bumped       , { 0 }, 0, "env_reflection_bumped"                 },
    /* 33  _dxshader_environment_reflection_flat */
    { 0, _vs_environment_reflection_flat         , { 0 }, 0, "env_reflection_flat"                   },
    /* 34  _dxshader_environment_reflection_flat_specular */
    { 0, _vs_environment_reflection_flat         , { 0 }, 0, "env_reflection_flat_specular"          },
    /* 35  _dxshader_environment_reflection_radiosity */
    { 0, _vs_environment_reflection_radiosity    , { 0 }, 0, "env_reflection_radiosity"              },
    /* 36  _dxshader_environment_reflection_lightmap_mask */
    { 0, _vs_environment_reflection_lightmap_mask, { 0 }, 0, "env_reflection_lm_mask"                },
    /* 37  _dxshader_environment_reflection_mirror_bumped */
    { 0, _vs_environment_reflection_mirror       , { 0 }, 0, "env_reflection_mirror_bumped"          },
    /* 38  _dxshader_environment_reflection_mirror_flat */
    { 0, _vs_environment_reflection_mirror       , { 0 }, 0, "env_reflection_mirror_flat"            },
    /* 39  _dxshader_environment_reflection_mirror_flat_specular */
    { 0, _vs_environment_reflection_mirror       , { 0 }, 0, "env_reflection_mirror_flat_specular"   },
    /* 40  _dxshader_environment_specular_light_flat */
    { 0, _vs_environment_specular_light          , { 0 }, 0, "env_spec_light_flat"                   },
    /* 41  _dxshader_environment_specular_light_bumped */
    { 0, _vs_environment_specular_light          , { 0 }, 0, "env_spec_light_bumped"                 },
    /* 42  _dxshader_environment_specular_lightmap_flat */
    { 0, _vs_environment_specular_lightmap       , { 0 }, 0, "env_spec_lm_flat"                      },
    /* 43  _dxshader_environment_specular_lightmap_bumped */
    { 0, _vs_environment_specular_lightmap       , { 0 }, 0, "env_spec_lm_bumped"                    },
    /* 44  _dxshader_transparent_plasma */
    { 0, _vs_transparent_plasma_m                , { 0 }, 0, "transparent_plasma"                    },
    /* 45  _dxshader_shadow_convolve */
    { 0, _vs_convolution                         , { 0 }, 0, "shadow_convolve"                       },
    /* 46  _dxshader_model_shadow */
    { 0, _vs_model_shadow                        , { 0 }, 0, "model_shadow"                          },
    /* 47  _dxshader_environment_shadow */
    { 0, _vs_environment_shadow                  , { 0 }, 0, "env_shadow"                            },
    /* 48  _dxshader_widget_sprite */
    { 0, _vs_lens_flare                          , { 0 }, 0, "widget_sprite"                         },
    /* 49  _dxshader_screen_normal */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_normal"                         },
    /* 50  _dxshader_screen_multitexture_add_add */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_add_add"               },
    /* 51  _dxshader_screen_multitexture_add_dot */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_add_dot"               },
    /* 52  _dxshader_screen_multitexture_add_multiply */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_add_mul"               },
    /* 53  _dxshader_screen_multitexture_add_multiply2x */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_add_mul2x"             },
    /* 54  _dxshader_screen_multitexture_add_subtract */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_add_subtract"          },
    /* 55  _dxshader_screen_multitexture_dot_add */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_dot_add"               },
    /* 56  _dxshader_screen_multitexture_dot_dot */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_dot_dot"               },
    /* 57  _dxshader_screen_multitexture_dot_multiply */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_dot_mul"               },
    /* 58  _dxshader_screen_multitexture_dot_multiply2x */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_dot_mul2x"             },
    /* 59  _dxshader_screen_multitexture_dot_subtract */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_dot_subtract"          },
    /* 60  _dxshader_screen_multitexture_multiply_add */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul_add"               },
    /* 61  _dxshader_screen_multitexture_multiply_dot */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul_dot"               },
    /* 62  _dxshader_screen_multitexture_multiply_multiply */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul_mul"               },
    /* 63  _dxshader_screen_multitexture_multiply_multiply2x */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul_mul2x"             },
    /* 64  _dxshader_screen_multitexture_multiply_subtract */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul_subtract"          },
    /* 65  _dxshader_screen_multitexture_multiply2x_add */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul2x_add"             },
    /* 66  _dxshader_screen_multitexture_multiply2x_dot */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul2x_dot"             },
    /* 67  _dxshader_screen_multitexture_multiply2x_multiply */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul2x_mul"             },
    /* 68  _dxshader_screen_multitexture_multiply2x_multiply2x */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul2x_mul2x"           },
    /* 69  _dxshader_screen_multitexture_multiply2x_subtract */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_mul2x_subtract"        },
    /* 70  _dxshader_screen_multitexture_subtract_add */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_sub_add"               },
    /* 71  _dxshader_screen_multitexture_subtract_dot */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_sub_dot"               },
    /* 72  _dxshader_screen_multitexture_subtract_multiply */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_sub_mul"               },
    /* 73  _dxshader_screen_multitexture_subtract_multiply2x */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_sub_mul2x"             },
    /* 74  _dxshader_screen_multitexture_subtract_subtract */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_multitex_sub_subtract"          },
    /* 75  _dxshader_screen_meter */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_meter"                          },
    /* 76  _dxshader_sun_glow_convolve */
    { 0, _vs_convolution                         , { 0 }, 0, "sun_glow_convolve"                     },
    /* 77  _dxshader_sun_glow_draw */
    { 0, _vs_lens_flare                          , { 0 }, 0, "sun_glow_draw"                         },
    /* 78  _dxshader_effect_multitexture_nonlinear_tint */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint"                },
    /* 79  _dxshader_effect_multitexture_nonlinear_tint_add */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint_add"            },
    /* 80  _dxshader_effect_multitexture_nonlinear_tint_alpha_blend */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint_alpha_blend"    },
    /* 81  _dxshader_effect_multitexture_nonlinear_tint_double_multiply */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint_double_mul"     },
    /* 82  _dxshader_effect_multitexture_nonlinear_tint_multiply */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint_mul"            },
    /* 83  _dxshader_effect_multitexture_nonlinear_tint_multiply_add */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_nlin_tint_mul_add"        },
    /* 84  _dxshader_effect_multitexture_normal_tint */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint"              },
    /* 85  _dxshader_effect_multitexture_normal_tint_add */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint_add"          },
    /* 86  _dxshader_effect_multitexture_normal_tint_alpha_blend */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint_alpha_blend"  },
    /* 87  _dxshader_effect_multitexture_normal_tint_double_multiply */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint_double_mul"   },
    /* 88  _dxshader_effect_multitexture_normal_tint_multiply */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint_mul"          },
    /* 89  _dxshader_effect_multitexture_normal_tint_multiply_add */
    { 0, _vs_effect_multitexture                 , { 0 }, 0, "eff_multitex_normal_tint_mul_add"      },
    /* 90  _dxshader_effect_nonlinear_tint */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint"                         },
    /* 91  _dxshader_effect_nonlinear_tint_add */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint_add"                     },
    /* 92  _dxshader_effect_nonlinear_tint_alpha_blend */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint_alpha_blend"             },
    /* 93  _dxshader_effect_nonlinear_tint_double_multiply */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint_double_mul"              },
    /* 94  _dxshader_effect_nonlinear_tint_multiply */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint_mul"                     },
    /* 95  _dxshader_effect_nonlinear_tint_multiply_add */
    { 0, _vs_effect                              , { 0 }, 0, "eff_nlin_tint_mul_add"                 },
    /* 96  _dxshader_effect_normal_tint */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint"                       },
    /* 97  _dxshader_effect_normal_tint_add */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint_add"                   },
    /* 98  _dxshader_effect_normal_tint_alpha_blend */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint_alpha_blend"           },
    /* 99  _dxshader_effect_normal_tint_double_multiply */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint_double_mul"            },
    /* 100 _dxshader_effect_normal_tint_multiply */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint_mul"                   },
    /* 101 _dxshader_effect_normal_tint_multiply_add */
    { 0, _vs_effect                              , { 0 }, 0, "eff_normal_tint_mul_add"               },
    /* 102 _dxshader_transparent_water_opacity */
    { 0, _vs_transparent_water_opacity           , { 0 }, 0, "transparent_water_opacity"             },
    /* 103 _dxshader_transparent_water_reflection */
    { 0, _vs_transparent_water_reflection        , { 0 }, 0, "transparent_water_reflection"          },
    /* 104 _dxshader_transparent_water_bumpmap_convolution */
    { 0, _vs_convolution                         , { 0 }, 0, "transparent_water_bumpmap_convolution" },
    /* 105 _dxshader_active_camouflage_draw */
    { 0, _vs_model_active_camouflage             , { 0 }, 0, "active_camouflage_draw"                },
    /* 106 _dxshader_transparent_glass_reflection_bumped */
    { 0, _vs_environment_reflection_bumped       , { 0 }, 0, "transparent_glass_reflection_bumped"   },
    /* 107 _dxshader_transparent_glass_reflection_flat */
    { 0, _vs_environment_reflection_flat         , { 0 }, 0, "transparent_glass_reflection_flat"     },
    /* 108 _dxshader_transparent_glass_reflection_mirror */
    { 0, _vs_environment_reflection_mirror       , { 0 }, 0, "transparent_glass_reflection_mirror"   },
    /* 109 _dxshader_transparent_glass_diffuse */
    { 0, _vs_transparent_glass_diffuse_light     , { 0 }, 0, "transparent_glass_diffuse"             },
    /* 110 _dxshader_transparent_generic */
    { 0, _vs_environment_texture                 , { 0 }, 0, "transparent_generic"                   },
    /* 111 _dxshader_transparent_meter */
    { 0, _vs_transparent_meter                   , { 0 }, 0, "transparent_meter"                     },
    /* 112 _dxshader_environment_fog */
    { 0, _vs_environment_fog                     , { 0 }, 0, "env_fog"                               },
    /* 113 _dxshader_screen_fog */
    { 0, -1                                      , { 0 }, 0, "screen_fog"                            },
    /* 114 _dxshader_screen_effect */
    { 0, _vs_convolution                         , { 0 }, 0, "screen_effect"                         },
    /* 115 _dxshader_screen_flash */
    { 0, -1                                      , { 0 }, 0, "screen_flash"                          },
    /* 116 _dxshader_model_environment */
    { 0, -1                                      , { 0 }, 0, "model_environment"                     },
    /* 117 _dxshader_model_self_illumination */
    { 0, -1                                      , { 0 }, 0, "model_mask_self_illumination"          },
    /* 118 _dxshader_model_change_color */
    { 0, -1                                      , { 0 }, 0, "model_mask_change_color"               },
    /* 119 _dxshader_model_multipurpose */
    { 0, -1                                      , { 0 }, 0, "model_mask_multipurpose"               },
    /* 120 _dxshader_model_reflection */
    { 0, -1                                      , { 0 }, 0, "model_mask_reflection"                 },
    /* 121 _dxshader_model_normal */
    { 0, -1                                      , { 0 }, 0, "model_mask_none"                       },
    /* 122 _dxshader_dyn_geom */
    { 0, _vs_screen2                             , { 0 }, 0, "screen_clr_const"                      },
};
