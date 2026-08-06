/* eol_characters @ 0x82122264 (.rdata, 2 bytes)
 * DB applied_types: const char eol_characters[2];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 0A 0D
 * { '\n', '\r' } (0x0A, 0x0D). Two entries: both consumers
 * walk it with a fixed 2-iteration loop (`cmpwi cr6, r11, 2` in skip_whitespace @0x83777978) and
 * read it with `lbzx` + `extsb` (signed char). +0x02..+0x03 are zero alignment padding inside the
 * 4-byte slot (hs_type_primitive_parsers follows at +0x04).
 */
const char eol_characters[2] = { '\n', '\r' };
