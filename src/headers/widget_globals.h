#pragma once
/* _widget_globals @ widget_globals — UI widget system state (ui_widget.c) */

typedef struct thread thread;

#include <stdint.h>
#include "real_argb_color.h"
#include "_text_box_parameters.h"
#include "_list_parameters.h"
#include "_animation_data.h"
#include "widget_instance.h"
#include "widget_stack_node.h"
#include "_deferred_error.h"
#include "_deferred_dashboard_error.h"
#include "_deferred_cinematic_error.h"

/* DB anonymous union: types_members _BC910FEC01C0C4B9F28A575600A72A5B
 * (widget_instance.parameters). Structural twin of widget_instance.h's widget_parameters_union. */
typedef union widget_parameters
{
    _text_box_parameters text_box_parameters;
    _list_parameters     list_parameters;
} widget_parameters;                     /* 28 bytes */

typedef struct _widget_globals
{
    widget_instance          *active_widgets[2];          /* 0x00 */
    widget_stack_node        *widget_stack[2];            /* 0x08 */
    unsigned int              current_system_milliseconds;/* 0x10 */
    int                       pause_disabled_ticks;       /* 0x14 */
    int16_t                   main_menu_deferred_error_code;/* 0x18 */
    int16_t                   pause_game_time_count;       /* 0x1A */
    float                     fade_to_black;               /* 0x1C */
    _deferred_error           deferred_error[2];           /* 0x20 (12 bytes) */
    _deferred_dashboard_error deferred_dashboard_error;    /* 0x2C */
    _deferred_cinematic_error deferred_cinematic_error[2]; /* 0x30 */
    thread                   *initialization_thread;       /* 0x38 */
    int16_t                   filesystem_check_result;      /* 0x3C */
    unsigned char             initialized;                  /* 0x3E */
    unsigned char             dont_load_children_recursive; /* 0x3F */
    unsigned char             debug_show_path;              /* 0x40 */
    unsigned char             processing_inhibited;         /* 0x41 */
    unsigned char             main_menu_music_active;       /* 0x42 */
    unsigned char             sound_paused;                 /* 0x43 */
} _widget_globals;                                          /* 68 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern _widget_globals widget_globals;
extern unsigned char we_are_at_the_main_menu;

#ifdef __cplusplus
}
#endif

#include "stack_memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern stack_memory_pool *widget_memory_pool;

#ifdef __cplusplus
}
#endif
