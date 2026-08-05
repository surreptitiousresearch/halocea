#pragma once

#include <stdint.h>
#include "_animation_data.h"
#include "_text_box_parameters.h"
#include "_list_parameters.h"

typedef struct event_record event_record;       /* forward decl */

/* DB anonymous union _BC910FEC01C0C4B9F28A575600A72A5B — widget parameters
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */
typedef union _BC910FEC01C0C4B9F28A575600A72A5B
{
    _text_box_parameters text_box_parameters; /* 0x00 */
    _list_parameters     list_parameters;     /* 0x00 */
} _BC910FEC01C0C4B9F28A575600A72A5B;                                            /* 28 bytes */
typedef union _BC910FEC01C0C4B9F28A575600A72A5B widget_parameters_union;

typedef struct widget_instance
{
    int                    ui_widget_tag_index;                  /* 0x00 */
    char                  *name;                                 /* 0x04 */
    int16_t                local_player_index;                   /* 0x08 */
    int16_t                horizontal_offset;                    /* 0x0A */
    int16_t                vertical_offset;                      /* 0x0C */
    int16_t                widget_type;                          /* 0x0E */
    uint8_t        visible;                              /* 0x10 */
    uint8_t        render_regardless_of_controller_index;/* 0x11 */
    uint8_t        never_receive_events;                 /* 0x12 */
    uint8_t        pause_game_time;                      /* 0x13 */
    uint8_t        delete_recursion_lock;                /* 0x14 */
    uint8_t        widget_is_error_dialog;               /* 0x15 */
    uint8_t        close_if_local_player_controller_present; /* 0x16 */
    uint8_t        _pad17[1];                            /* 0x17 */
    unsigned int           creation_time;                        /* 0x18 */
    unsigned int           milliseconds_to_auto_close;           /* 0x1C */
    unsigned int           auto_close_fade_time;                 /* 0x20 */
    float                  alpha_modifier;                       /* 0x24 */
    struct widget_instance *previous;                            /* 0x28 */
    struct widget_instance *next;                                /* 0x2C */
    struct widget_instance *parent;                              /* 0x30 */
    struct widget_instance *children;                            /* 0x34 */
    struct widget_instance *focused_child;                       /* 0x38 */
    union _BC910FEC01C0C4B9F28A575600A72A5B parameters;          /* 0x3C (28 bytes) */
    _animation_data        animation_data;                       /* 0x58 (8 bytes) */
} widget_instance;                                               /* 96 bytes */
