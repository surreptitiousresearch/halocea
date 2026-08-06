/* typecasting_procedures @ 0x841721E8 (.data, 9604 bytes)
 * DB applied_types: int (__fastcall *typecasting_procedures[49][49])(int);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
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
 *   +0x0080 = 0x00000000
 *   +0x0084 = 0x00000000
 *   +0x0088 = 0x00000000
 *   +0x008C = 0x00000000
 *   +0x0090 = 0x00000000
 *   +0x0094 = 0x00000000
 *   +0x0098 = 0x00000000
 *   +0x009C = 0x00000000
 *   ... 9444 further bytes elided; full hex in .sweep/data_image.tsv
 * 9604 bytes = int (*[49][49])(int).
 *    HS (HaloScript) type-cast dispatch matrix: [source_type][dest_type] -> conversion
 *    procedure, or NULL when no cast exists. Reconstructed from the raw image (big-endian);
 *    only 70 of the 2401 slots are non-NULL, referencing 13 distinct conversion functions,
 *    all resolved to named functions via the DB funcs table. Emitted as a designated-index
 *    initializer so the null majority stays implicit-zero and every reloc lands in its exact
 *    [row][col] cell.
 */
#include <stdint.h>

/* forward decls for the HS type-conversion procedures (defined elsewhere in the Blam lib).
   Every one is `int (int)`: a HaloScript value is an opaque 32-bit word, so the coercions that
   deal in reals type-pun through memory rather than using f1. Verified per function against the
   disassembly — e.g. hs_long_to_real ends `stfs f12, back_chain(r1)` / `lwz r3, back_chain(r1)`,
   and hs_real_to_short opens `stw r3, arg_14(r1)` / `lfs f0, arg_14(r1)`. This is why the DB
   applied type of the table is a uniform `int (*[49][49])(int)`. */
extern int hs_data_to_void(int n);
extern int hs_enum_to_real(int value);
extern int hs_long_to_boolean(int n);
extern int hs_long_to_real(int l);
extern int hs_long_to_short(int l);
extern int hs_object_name_to_object_list(int object_name_index);
extern int hs_object_to_object_list(int object_index);
extern int hs_real_to_long(int r);
extern int hs_real_to_short(int r);
extern int hs_short_to_boolean(int s);
extern int hs_short_to_real(int value);
extern int hs_string_to_boolean(int value);
extern int object_list_from_ai_reference(int ai_index);

int (*typecasting_procedures[49][49])(int) = {
    [4] = { [5] = hs_data_to_void, [6] = hs_data_to_void, [7] = hs_data_to_void, [8] = hs_data_to_void, [9] = hs_data_to_void, [10] = hs_data_to_void, [11] = hs_data_to_void, [12] = hs_data_to_void, [13] = hs_data_to_void, [14] = hs_data_to_void, [15] = hs_data_to_void, [16] = hs_data_to_void, [17] = hs_data_to_void, [18] = hs_data_to_void, [19] = hs_data_to_void, [20] = hs_data_to_void, [21] = hs_data_to_void, [22] = hs_data_to_void, [23] = hs_data_to_void, [24] = hs_data_to_void, [25] = hs_data_to_void, [26] = hs_data_to_void, [27] = hs_data_to_void, [28] = hs_data_to_void, [29] = hs_data_to_void, [30] = hs_data_to_void, [31] = hs_data_to_void, [32] = hs_data_to_void, [33] = hs_data_to_void, [34] = hs_data_to_void, [35] = hs_data_to_void, [36] = hs_data_to_void, [37] = hs_data_to_void, [38] = hs_data_to_void, [39] = hs_data_to_void, [40] = hs_data_to_void, [41] = hs_data_to_void, [42] = hs_data_to_void, [43] = hs_data_to_void, [44] = hs_data_to_void, [45] = hs_data_to_void, [46] = hs_data_to_void, [47] = hs_data_to_void, [48] = hs_data_to_void },
    [5] = { [6] = hs_long_to_boolean, [7] = hs_short_to_boolean, [8] = hs_long_to_boolean, [9] = hs_string_to_boolean },
    [6] = { [7] = hs_short_to_real, [8] = hs_long_to_real, [30] = hs_enum_to_real, [31] = hs_enum_to_real, [32] = hs_enum_to_real, [33] = hs_enum_to_real },
    [7] = { [6] = hs_real_to_short, [8] = hs_long_to_short },
    [8] = { [6] = hs_real_to_long, [8] = hs_long_to_short },
    [23] = { [16] = object_list_from_ai_reference, [34] = hs_object_to_object_list, [35] = hs_object_to_object_list, [36] = hs_object_to_object_list, [37] = hs_object_to_object_list, [38] = hs_object_to_object_list, [39] = hs_object_name_to_object_list, [40] = hs_object_name_to_object_list, [41] = hs_object_name_to_object_list, [42] = hs_object_name_to_object_list, [43] = hs_object_name_to_object_list, [44] = hs_object_name_to_object_list },
};
