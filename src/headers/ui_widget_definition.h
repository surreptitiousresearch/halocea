#pragma once
/* ui_widget_definition — the 'DeLa' tag definition backing a widget_instance (ui_widget.c).
 * Layout taken verbatim from the database. */

#include "tag_block.h"
#include "tag_reference.h"
#include "rectangle2d.h"
#include "real_argb_color.h"

typedef struct ui_widget_definition
{
    __int16         type;                              /* 0x000 */
    __int16         controller_index;                  /* 0x002 */
    char            name[32];                          /* 0x004 */
    rectangle2d     bounds;                            /* 0x024 */
    unsigned int    flags;                             /* 0x02C */
    int             millisecond_to_auto_close;         /* 0x030 */
    int             millisecond_auto_close_fade_time;  /* 0x034 */
    tag_reference   background_bitmap;                 /* 0x038 */
    tag_block       game_data_input_references;        /* 0x048 */
    tag_block       event_handler_references;          /* 0x054 */
    tag_block       search_and_replace_references;     /* 0x060 */
    int             unused[32];                        /* 0x06C */
    tag_reference   text_label_unicode_string_list_tag;/* 0x0EC */
    tag_reference   font_tag;                          /* 0x0FC */
    real_argb_color text_color;                        /* 0x10C */
    __int16         justification;                     /* 0x11C */
    __int16         text_box_flags;                    /* 0x11E */
    tag_block       unused_tag_block;                  /* 0x120 */
    __int16         pad1;                              /* 0x12C */
    __int16         string_list_index;                 /* 0x12E */
    __int16         hoffset;                           /* 0x130 */
    __int16         voffset;                           /* 0x132 */
    int             widget_type_text_box_unused[6];    /* 0x134 */
    __int16         pad2;                              /* 0x14C */
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned int    list_flags;                        /* 0x150 */
    tag_reference   list_header_bitmap;                /* 0x154 */
    tag_reference   list_footer_bitmap;                /* 0x164 */
    rectangle2d     list_header_bounds;                /* 0x174 */
    rectangle2d     list_footer_bounds;                /* 0x17C */
    int             widget_type_spinner_list_unused[8];/* 0x184 */
    tag_reference   extended_description_widget_tag;   /* 0x1A4 */
    int             widget_type_column_list_unused[8]; /* 0x1B4 */
    int             widget_type_game_model_unused[64]; /* 0x1D4 */
    tag_block       conditional_widget_references;     /* 0x2D4 */
    int             widget_type_movie_unused[32];      /* 0x2E0 */
    int             widget_type_custom_unused[32];     /* 0x360 */
    tag_block       child_widget_references;           /* 0x3E0 */
} ui_widget_definition;                                /* 1004 bytes */
