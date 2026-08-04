/* credits_post_render @0x83698080 — per-frame update+draw for the scrolling credits widget. Advances every
 * live credit line upward by a time-scaled amount (0.08 px/ms since the last update), deleting lines that
 * scroll off the top; then, while the bottom-most line has risen above the y=430 spawn threshold, spawns new
 * lines from the widget's unicode string list (cyclically from index 0 in attract mode, or sequentially in
 * end-game mode). When end-game mode exhausts the string list and the list empties, it flips to state 2 and
 * returns to the previous widget. Finally it draws every live line — yellow for "header" lines (type 1),
 * light blue otherwise — clipped to the (50,0,430,640) rectangle. Always returns 1 (handled).
 *
 * The credit-line records (dynamic_array, 20-byte stride) are typed (string pointer @0, string index @4,
 * type @8, top y @12, bottom y @16). The goto-based spawn dispatch is reflowed: the non-end-game path
 * enters the spawn loop body once unconditionally (string 0), modeled with a `first` flag. The widget tag
 * definition supplies the string-list tag index (@248) and the font index (@264). */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/text_justification.h"
#include "headers/credits_line_s.h"
#include "headers/event_record.h"
#include "headers/rectangle2d.h"
#include "headers/dynamic_array.h"
#include "headers/real_argb_color.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_definition.h"
#include "headers/credits_endgame_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern uint32_t system_milliseconds(void);
extern void dynamic_array_delete_element(dynamic_array *array, int index);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern int credits_item_new(const wchar_t *string, int string_index, int y0);
extern void widget_instance_go_back_to_previous(widget_instance *widget);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

unsigned __int8 credits_post_render(widget_instance *widget, event_record *event, unsigned __int8 *widget_deleted)
{
    ui_widget_definition *widget_definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    int string_list_tag = widget_definition->text_label_unicode_string_list_tag.index;
    int font_index = widget_definition->font_tag.index;
    int string_count = *TAG_GET(int, string_list_tag);

    unsigned int now = system_milliseconds();
    int scroll = (int)((float)(now - credits_last_update_time) * 0.079999998f);

    int count = credits_list.count;
    int bottom_y = 430;
    int last_string_index = -1;

    if ( scroll != 0 )
    {
        credits_last_update_time = now;

        bool skip_spawn = false;
        if ( count > 0 )
        {
            int i = 0;
            do
            {
                credits_line_s *entry = &((credits_line_s *)credits_list.elements)[i];
                last_string_index = entry->string_index;
                int y2 = entry->y1 - scroll;
                entry->y0 -= scroll;
                entry->y1 = y2;
                bottom_y = y2;
                if ( y2 < 50 )
                {
                    dynamic_array_delete_element(&credits_list, i);
                    --i;
                }
                count = credits_list.count;
                ++i;
            }
            while ( i < count );

            if ( bottom_y > 430 )
                skip_spawn = true;
        }

        if ( !skip_spawn )
        {
            int string_index = credits_endgame_mode ? last_string_index + 1 : 0;
            bool first = !credits_endgame_mode;
            bool filled = false;
            while ( first || string_index < string_count )
            {
                first = false;
                const wchar_t *string = unicode_string_list_get_string(string_list_tag, string_index);
                bottom_y += credits_item_new(string, string_index, bottom_y);
                if ( bottom_y > 430 )
                {
                    filled = true;
                    break;
                }
                ++string_index;
            }
            count = credits_list.count;
            if ( !filled && count <= 0 )
            {
                credits_endgame_mode = CREDITS_ENDGAME_FINISHED;
                widget_instance_go_back_to_previous(widget);
                return 1;
            }
        }
    }

    if ( count <= 0 )
        return 1;

    rectangle2d clip;
    clip.__s1.y0 = 50;
    clip.__s1.x0 = 0;
    clip.__s1.y1 = 430;
    clip.__s1.x1 = 640;

    int i = 0;
    do
    {
        credits_line_s *entry = &((credits_line_s *)credits_list.elements)[i];
        rectangle2d bounds;
        bounds.__s1.y0 = entry->y0;
        bounds.__s1.x0 = 0;
        bounds.__s1.y1 = entry->y1;
        bounds.__s1.x1 = 640;
        const real_argb_color *color = (entry->formatting == 1)
            ? global_real_argb_yellow
            : global_real_argb_lightblue;
        draw_string_set_draw_mode(font_index, -1, _text_justification_center, 0, color);
        rasterizer_draw_unicode_string(&bounds, &clip, nullptr, 0, entry->string);
        ++i;
    }
    while ( i < credits_list.count );

    return 1;
}
