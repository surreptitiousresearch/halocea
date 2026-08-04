/* set_profile_name @0x83732AFC — sets a text widget's buffer to "<label> <name>", where <label> is string
 * index 7 of the "common_button_captions" string list (reallocated to fit; the buffer is left null on
 * allocation failure). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/stack_memory_pool.h"
#include "headers/blam_data_globals.h"

#include "headers/stack_memory_pool.h"
extern int tag_loaded(uint32_t group_tag, const char *name);
extern void *pool_resize_pointer(stack_memory_pool *pool, void *p, unsigned int new_size);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

void set_profile_name(widget_instance *text_widget, const wchar_t *name)
{
    int captions_tag = tag_loaded(0x75737472u, "ui\\shell\\strings\\common_button_captions");

    unsigned __int16 *text = pool_resize_pointer(
        widget_memory_pool, (char *)text_widget->parameters.text_box_parameters.text, 0x80u);

    text_widget->parameters.text_box_parameters.text = text;
    if ( text )
    {
        const wchar_t *label = unicode_string_list_get_string(captions_tag, 7);
        usnprintf(text_widget->parameters.text_box_parameters.text, 0x3Fu, L"%s %s", label, name);
        text_widget->parameters.text_box_parameters.text[63] = 0;
    }
}
