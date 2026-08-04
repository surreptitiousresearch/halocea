/* set_textbox_to_build_number @0x8377E620 — allocate the widget's text-box buffer (128 bytes) if it doesn't
 * have one, then copy the build number string into it (truncated to 63 characters). */

#include <stdint.h>
#include <string.h>
#include "headers/widget_instance.h"
#include "headers/blam_data_globals.h"

extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void set_textbox_to_build_number(widget_instance *widget)
{
    if ( !widget->parameters.text_box_parameters.text )
    {
        unsigned __int16 *text = ui_widget_realloc(nullptr, 0x80u);

        widget->parameters.text_box_parameters.text = text;
        if ( text )
            memset(text, 0, 0x80u);
    }

    unsigned __int16 *text = widget->parameters.text_box_parameters.text;

    if ( text )
    {
        ustrncpy(text, build_number_string, 0x3F);
        widget->parameters.text_box_parameters.text[63] = 0;
    }
}
