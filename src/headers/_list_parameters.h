#pragma once
#include <stdint.h>
/* _list_parameters — UI list widget parameters (ui_widget.c) */

struct widget_instance;

typedef struct _list_parameters
{
    int16_t                 selected_list_item_index; /* 0x00 */
    int16_t                 list_item_top_index;      /* 0x02 */
    int16_t                 current_list_item_index;  /* 0x04 */
    int16_t                 last_list_tab_direction;  /* 0x06 */
    void                   *list_items;               /* 0x08 */
    uint16_t        number_of_items;          /* 0x0C */
    unsigned char _pad0[2]; /* db-verified padding */
    struct widget_instance *extended_description;     /* 0x10 */
    uint16_t       *item_text;                /* 0x14 */
    int16_t                 spin_activated;           /* 0x18 */
    char                    _pad1A[2];                /* 0x1A */
} _list_parameters;                                   /* 28 bytes */
