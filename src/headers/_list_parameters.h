#pragma once
/* _list_parameters — UI list widget parameters (ui_widget.c) */

struct widget_instance;

typedef struct _list_parameters
{
    __int16                 selected_list_item_index; /* 0x00 */
    __int16                 list_item_top_index;      /* 0x02 */
    __int16                 current_list_item_index;  /* 0x04 */
    __int16                 last_list_tab_direction;  /* 0x06 */
    void                   *list_items;               /* 0x08 */
    unsigned __int16        number_of_items;          /* 0x0C */
    unsigned char _pad0[2]; /* db-verified padding */
    struct widget_instance *extended_description;     /* 0x10 */
    unsigned __int16       *item_text;                /* 0x14 */
    __int16                 spin_activated;           /* 0x18 */
    char                    _pad1A[2];                /* 0x1A */
} _list_parameters;                                   /* 28 bytes */
