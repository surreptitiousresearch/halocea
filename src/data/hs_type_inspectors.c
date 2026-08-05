/* hs_type_inspectors @ 0x84172120, 196 bytes (data-init; ledger mislabeled data-bss).
   HS script type -> value-inspector dispatch table (sparse). Targets recovered
   from binary relocations. */

#include <stdint.h>

extern void hs_inspect_boolean(int16_t type, int value, char *buffer);
extern void hs_inspect_real(int16_t type, int value, char *buffer);
extern void hs_inspect_short_integer(int16_t type, int value, char *buffer);
extern void hs_inspect_long_integer(int16_t type, int value, char *buffer);
extern void hs_inspect_string(int16_t type, int value, char *buffer);   /* value word is a text pointer */
extern void hs_inspect_enum(int16_t type, int value, char *buffer);

void (__fastcall *hs_type_inspectors[49])(int16_t value_type, int value, char *buffer) = {
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
