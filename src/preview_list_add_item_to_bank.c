/* preview_list_add_item_to_bank @0x837807B8 — append a new item to a preview list bank (16-byte records:
 * name_ptr +0, data_ptr +4, list_index +8, is_default +12 — per preview_list_clear_default_item.c). Copies
 * `name` into a fresh allocation, and if `data`/`data_len` are given, copies that payload into a second
 * fresh allocation too. Sets preview_list_has_default when the new item is marked default. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"


extern unsigned int ustrlen(const wchar_t *string);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void preview_list_add_item_to_bank(int bank, const wchar_t *name, int list_index, void *data,
        unsigned int data_len, char is_default)
{
    int item_index = dynamic_array_add_element(&preview_list_array_bank[bank]);
    if ( item_index == -1 )
        return;

    unsigned int name_length = ustrlen(name);
    bank_item_s *item = &((bank_item_s *)preview_list_array_bank[bank].elements)[item_index];

    item->ptr = 0;
    unsigned __int16 *name_copy = dlMalloc(2 * (name_length + 1),
        "D:\\Projects\\code\\HCEX\\sources\\interface\\ui_widget_game_data_input_functions.c", 0x1076u);
    item->list_index = list_index;
    item->name = name_copy;
    item->is_default = is_default;
    if ( is_default )
        preview_list_has_default = 1;
    ustrcpy(name_copy, name);

    if ( data && data_len )
    {
        void *data_copy = dlMalloc(data_len,
            "D:\\Projects\\code\\HCEX\\sources\\interface\\ui_widget_game_data_input_functions.c", 0x1083u);
        item->ptr = data_copy;
        memcpy(data_copy, data, data_len);
    }
}
