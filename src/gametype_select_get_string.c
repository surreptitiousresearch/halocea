/* gametype_select_get_string @0x837814D0 — tail-call forwarder (`b single_preview_column_list_get_string`;
 * confirmed via disasm, not a bare blr). The gametype-select UI list reuses the generic single-preview-
 * column list-box's string-getter verbatim. single_preview_column_list_get_string itself is not yet
 * decompiled — declared extern with its own DB prototype and called through with the same arguments. */

#include <stdint.h>

extern uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items);

uint8_t gametype_select_get_string(uint16_t *out, int index, void *list_items)
{
    return single_preview_column_list_get_string(out, index, list_items);
}
