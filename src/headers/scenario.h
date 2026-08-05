#pragma once
/* scenario — scenario tag (scenario.c). Layout reproduced verbatim from the database `scenario` type.
 * Unnamed "*_unused" gaps are kept as sized arrays so member offsets stay exact. */

#include <stdint.h>
#include "tag_data.h"
#include "tag_block.h"
#include "tag_reference.h"

typedef struct scenario
{
    tag_reference ugly_structure_bsp;                  /* 0x000 */
    tag_reference unloved_globals;                     /* 0x010 */
    tag_reference bad_sky;                             /* 0x020 */
    tag_block     sky_references;                      /* 0x030 */
    int16_t       type;                                /* 0x03C */
    uint16_t flags;                            /* 0x03E */
    tag_block     scenario_references;                 /* 0x040 */
    float         local_north;                         /* 0x04C */
    unsigned int  header_unused[5];                    /* 0x050 */
    int           reference_unused[34];                /* 0x064 */
    tag_block     predicted_ui_resources;              /* 0x0EC */
    tag_block     functions;                           /* 0x0F8 */
    tag_data      editor_scenario_data;                /* 0x104 */
    tag_block     comments;                            /* 0x118 */
    int           user_edit_unused[56];                /* 0x124 */
    tag_block     object_names;                        /* 0x204 */
    tag_block     scenery;                             /* 0x210 */
    tag_block     scenery_palette;                     /* 0x21C */
    tag_block     bipeds;                              /* 0x228 */
    tag_block     biped_palette;                       /* 0x234 */
    tag_block     vehicles;                            /* 0x240 */
    tag_block     vehicle_palette;                     /* 0x24C */
    tag_block     equipment;                           /* 0x258 */
    tag_block     equipment_palette;                   /* 0x264 */
    tag_block     weapons;                             /* 0x270 */
    tag_block     weapon_palette;                      /* 0x27C */
    tag_block     device_groups;                       /* 0x288 */
    tag_block     machines;                            /* 0x294 */
    tag_block     machine_palette;                     /* 0x2A0 */
    tag_block     controls;                            /* 0x2AC */
    tag_block     control_palette;                     /* 0x2B8 */
    tag_block     light_fixtures;                      /* 0x2C4 */
    tag_block     light_fixtures_palette;              /* 0x2D0 */
    tag_block     sound_scenery;                       /* 0x2DC */
    tag_block     sound_scenery_palette;               /* 0x2E8 */
    tag_block     unused_blocks[7];                    /* 0x2F4 */
    tag_block     starting_profiles;                   /* 0x348 */
    tag_block     players;                             /* 0x354 */
    tag_block     trigger_volumes;                     /* 0x360 */
    tag_block     recorded_animations;                 /* 0x36C */
    tag_block     netgame_flags;                       /* 0x378 */
    tag_block     netgame_equipment;                   /* 0x384 */
    tag_block     scenario_starting_equipment;         /* 0x390 */
    tag_block     bsp_switch_trigger_volumes;          /* 0x39C */
    tag_block     decals;                              /* 0x3A8 */
    tag_block     decal_palette;                       /* 0x3B4 */
    tag_block     detail_object_collection_palette;    /* 0x3C0 */
    int           render_unused[21];                   /* 0x3CC */
    tag_block     ai_actor_palette;                    /* 0x420 */
    tag_block     ai_encounters;                       /* 0x42C */
    tag_block     ai_command_lists;                    /* 0x438 */
    tag_block     ai_animation_references;             /* 0x444 */
    tag_block     ai_script_references;                /* 0x450 */
    tag_block     ai_recording_references;             /* 0x45C */
    tag_block     ai_conversations;                    /* 0x468 */
    tag_data      hs_syntax_data;                      /* 0x474 */
    tag_data      hs_string_constants;                 /* 0x488 */
    tag_block     hs_scripts;                          /* 0x49C */
    tag_block     hs_globals;                          /* 0x4A8 */
    tag_block     hs_references;                       /* 0x4B4 */
    tag_block     hs_source_files;                     /* 0x4C0 */
    int           scripting_unused[6];                 /* 0x4CC */
    tag_block     cutscene_flags;                      /* 0x4E4 */
    tag_block     cutscene_camera_points;              /* 0x4F0 */
    tag_block     cutscene_chapter_titles;             /* 0x4FC */
    int           rapidly_dwindling_unused_space[27];  /* 0x508 */
    tag_reference custom_object_names;                 /* 0x574 */
    tag_reference ingame_help_text;                    /* 0x584 */
    tag_reference hud_messages;                        /* 0x594 */
    tag_block     structure_bsp_references;            /* 0x5A4 */
} scenario;

extern scenario *global_scenario;
extern int global_scenario_index;
