/* base_weapon_label_get @0x836CC0C0 */
#include <stdint.h>
/* DEVIATION: disasm builds 1-element string table on the caller stack; only index 0 ("unarmed") is defined */
static const char *base_weapon_labels[1] = { "unarmed" };

const char * base_weapon_label_get(int16_t base_weapon_index)
{
    return base_weapon_labels[base_weapon_index];
}
