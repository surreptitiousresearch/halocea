/* cache_root_directory @ 0x84176DE8 — .data data-init.
 * DB type char[256]; bytes begin 64 3A 5C 00 ("d:\") then zero-fill.
 * NOTE: several consumers inline-extern this as `char *cache_root_directory`, but the
 * binary storage is a 256-byte char array (the leading bytes are the ASCII path "d:\",
 * not a pointer). The array form is the ABI-correct definition. */
char cache_root_directory[256] = "d:\\";
