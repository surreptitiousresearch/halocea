/* hs_type_inspectors @ 0x84172120 (.data, 196 bytes)
 * DB applied_types: void (__fastcall *hs_type_inspectors[49])(__int16, int, char *);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x8368C948 -> hs_inspect_boolean
 *   +0x0018 = 0x8368C998 -> hs_inspect_real
 *   +0x001C = 0x8368C9D8 -> hs_inspect_short_integer
 *   +0x0020 = 0x8368CA10 -> hs_inspect_long_integer
 *   +0x0024 = 0x8368CA28 -> hs_inspect_string
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x00000000
 *   +0x0038 = 0x00000000
 *   +0x003C = 0x00000000
 *   +0x0040 = 0x00000000
 *   +0x0044 = 0x00000000
 *   +0x0048 = 0x00000000
 *   +0x004C = 0x00000000
 *   +0x0050 = 0x00000000
 *   +0x0054 = 0x00000000
 *   +0x0058 = 0x00000000
 *   +0x005C = 0x00000000
 *   +0x0060 = 0x00000000
 *   +0x0064 = 0x00000000
 *   +0x0068 = 0x00000000
 *   +0x006C = 0x00000000
 *   +0x0070 = 0x00000000
 *   +0x0074 = 0x00000000
 *   +0x0078 = 0x00000000
 *   +0x007C = 0x00000000
 *   +0x0080 = 0x8368CA40 -> hs_inspect_enum
 *   +0x0084 = 0x8368CA40 -> hs_inspect_enum
 *   +0x0088 = 0x8368CA40 -> hs_inspect_enum
 *   +0x008C = 0x8368CA40 -> hs_inspect_enum
 *   +0x0090 = 0x8368CA40 -> hs_inspect_enum
 *   +0x0094 = 0x00000000
 *   +0x0098 = 0x00000000
 *   +0x009C = 0x00000000
 *   ... 36 further bytes elided; full hex in .sweep/data_image.tsv
 * 196 bytes (data-init; ledger mislabeled data-bss).
 *    HS script type -> value-inspector dispatch table (sparse). Targets recovered
 *    from binary relocations.
 */
#include <stdint.h>

extern void hs_inspect_boolean(int16_t type, int value, char *buffer);
extern void hs_inspect_real(int16_t type, int value, char *buffer);
extern void hs_inspect_short_integer(int16_t type, int value, char *buffer);
extern void hs_inspect_long_integer(int16_t type, int value, char *buffer);
extern void hs_inspect_string(int16_t type, int value, char *buffer);   /* value word is a text pointer */
extern void hs_inspect_enum(int16_t type, int value, char *buffer);

void (*hs_type_inspectors[49])(int16_t value_type, int value, char *buffer) = {
    [5] = hs_inspect_boolean,
    [6] = hs_inspect_real,
    [7] = hs_inspect_short_integer,
    [8] = hs_inspect_long_integer,
    [9] = hs_inspect_string,
    [32] = hs_inspect_enum,
    [33] = hs_inspect_enum,
    [34] = hs_inspect_enum,
    [35] = hs_inspect_enum,
    [36] = hs_inspect_enum,
};
