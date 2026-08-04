#pragma once
/* _text_box_parameters — UI text-box widget parameters (ui_widget.c) */

#include "real_argb_color.h"

typedef struct _text_box_parameters
{
    unsigned __int16 *text;              /* 0x00 */
    __int16           string_list_index; /* 0x04 */
    char              _pad6[2];           /* 0x06 */
    real_argb_color   text_color;        /* 0x08 */
    unsigned __int8   flashing;          /* 0x18 */
    char              _pad19[3];         /* 0x19 */
} _text_box_parameters;                  /* 28 bytes */
