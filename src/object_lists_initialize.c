/* object_lists_initialize @ 0x83775418 — object-list header + reference pools */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern int sprintf_0(char *string, const char *format, ...);

void object_lists_initialize(void)
{
    char name[264];
    /* disasm: r3 at blr is the second game_state_data_new residue (stored to object_list_data);
     * 0 consumers, no explicit return computation => void */
    object_list_header_data = game_state_data_new("object list header", 48, 12);
    sprintf_0(name, "%s reference", "list object");
    object_list_data = game_state_data_new(name, 128, 12);
}
