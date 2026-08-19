/* spcl_ensure_top_item_visible @0x83780AC8 — recompute a spinner/scroll-list widget's top-visible-item
 * index so the currently selected item stays in view. The visible row count comes from the widget
 * definition's item-count field (+992), reduced by 1 if the list's first child's first grandchild is a
 * text-box widget (a header row eating a visible slot), and further reduced to 3 (or 1, if the definition's
 * flag bit +336&8 is set or the list is nearly full) fewer than that to leave scroll headroom, clamped to
 * the item count. If the selection has scrolled out of the current visible window, the top index jumps to
 * show it (clamped so the window doesn't run past the end of the list, or to 0 if nothing is selected);
 * otherwise the existing top index is kept (reset to 0 if it was never set, i.e. -1). Returns the resulting
 * top index. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_list_flags.h"
#include "headers/blam_data_globals.h"


int spcl_ensure_top_item_visible(widget_instance *widget)
{
    widget_instance *children = widget->children;
    int list_item_top_index = widget->parameters.list_parameters.list_item_top_index;
    /* deref: TAG_INSTANCE lands on base_address, the definition pointer must be read through it
     * (disasm 0x83780AEC lwz r10,0x14(r6) / lwz r8,0x3E0(r10)) — was missing the deref */
    ui_widget_definition *definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    int visible_count = definition->child_widget_references.count;

    uint8_t has_header_row = children && children->children && children->children->widget_type == _ui_widget_type_spinner_list;
    if ( has_header_row )
        --visible_count;

    uint8_t near_full = (definition->list_flags & (1u << _list_single_preview_box_no_scroll)) != 0
        || widget->parameters.list_parameters.number_of_items <= visible_count - 1;

    int number_of_items = widget->parameters.list_parameters.number_of_items;
    int window = visible_count - (near_full ? 1 : 3);
    if ( window > number_of_items )
        window = number_of_items;

    int selected = widget->parameters.list_parameters.selected_list_item_index;

    if ( selected < list_item_top_index || selected >= window + list_item_top_index )
    {
        if ( selected >= 0 )
        {
            int max_top = number_of_items - window;
            if ( selected >= max_top )
                selected = max_top;
            widget->parameters.list_parameters.list_item_top_index = selected;
            return (int16_t)selected;
        }
        else
        {
            widget->parameters.list_parameters.list_item_top_index = 0;
            return 0;
        }
    }
    else if ( list_item_top_index == -1 )
    {
        widget->parameters.list_parameters.list_item_top_index = 0;
        return 0;
    }
    else
    {
        return widget->parameters.list_parameters.list_item_top_index;
    }
}
