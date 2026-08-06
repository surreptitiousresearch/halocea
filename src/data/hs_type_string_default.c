/* hs_type_string_default @ 0x84178040 (.data, 4 bytes)
 * DB applied_types: const char *hs_type_string_default;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8200155A -> "" (empty string at target)
 * .data data-init. DB applied type const char *;
 * points at the shared empty-string .rdata constant (0x8200155A) = "".
 * (Ledger col-1 name is _hs_type_string_default; consumers use the plain name.
 *  A consumer inline-externs it as int — DB pointer type is authoritative.)
 */
const char *hs_type_string_default = "";
