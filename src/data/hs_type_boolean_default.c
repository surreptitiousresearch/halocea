/* hs_type_boolean_default @ 0x82118F0C (.rdata, 1 bytes)
 * DB applied_types: const unsigned __int8 hs_type_boolean_default;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 00 (1 trailing byte)
 * .rdata read-only constant. DB symbol name is the leading-underscore
 * form _hs_type_boolean_default (the original file-scope spelling); consumers use the plain name, the same
 * convention src/data/hs_type_string_default.c already established. Access width: lbz (disasm 0x8368DA20); image byte 00.
 * hs_global_reconcile_read substitutes it when an external HaloScript global's backing pointer is null.
 */
const unsigned char hs_type_boolean_default = 0;
