#pragma once
/* hud_messaging_parameters_definition — HUD messaging tag block (hud.c).
 * DB types_members-confirmed (hud_messaging_update.c consumer). */

#include "hud_absolute_placement_definition.h"
#include "hud_placement_definition.h"
#include "tag_reference.h"
#include "tag_block.h"
#include "real_argb_color.h"
#include "hud_color_definition.h"

typedef struct hud_messaging_parameters_definition
{
    hud_absolute_placement_definition absolute_placement; /* 0x00 (0) */
    hud_placement_definition          placement;          /* 0x24 (36) */
    tag_reference   single_player_font;        /* 0x48 (72) — cinematic subtitle font */
    tag_reference   multi_player_font;         /* 0x58 (88) */
    float           up_time;                   /* 0x68 (104) */
    float           fade_time;                 /* 0x6C (108) */
    real_argb_color state_color;               /* 0x70 (112) — hud_get_text_color's return */
    real_argb_color text_color;                /* 0x80 (128) */
    float           spacing;                   /* 0x90 (144) */
    tag_reference   hud_item_messages;         /* 0x94 (148) */
    tag_reference   messaging_icons;           /* 0xA4 (164) — .index is the messaging-icon bitmap group */
    tag_reference   alternate_icon_text;       /* 0xB4 (180) */
    tag_block       button_icons;              /* 0xC4 (196) — 16-byte icon_hud_element_definition entries */
    hud_color_definition color;                /* 0xD0 (208) */
    tag_reference   hud_messages;              /* 0xF0 (240) */
    hud_color_definition objective_color;      /* 0x100 (256) */
} hud_messaging_parameters_definition;        /* 288 bytes */
