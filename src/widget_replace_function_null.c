/* widget_replace_function_null @0x83803FA8 — default "no replacement text" widget callback; always returns
 * the shared empty wide string (see single_preview_column_list_get_string.c for the same fallback string).
 * `widget` is unused (confirmed via disasm: only its own address computation runs, r3 is never read). */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/widget_instance.h"

extern const wchar_t empty_wide_string; /* .short 0 — empty wide string */

uint16_t * widget_replace_function_null(widget_instance *widget)
{
    return (uint16_t *)&empty_wide_string;
}
