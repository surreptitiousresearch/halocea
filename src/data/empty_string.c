/* empty_string @ 0x8200155A (.rdata, 2 bytes)
 * DB applied_types: char byte_8200155A[2];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 00 00
 * the shared "" string literal.
 * Not an exported symbol in the original: it is the anonymous `""` the compiler emitted once and
 * the linker pooled, sitting in the alignment gap between the "…\ds\ds_string.hpp" filename string
 * (whose NUL is 0x82001559) and the 4-aligned float pool at 0x8200155C. Its address is taken by
 * 7,452 .text sites across 6,341 functions — every STRONG_ASSERT "info" argument, plus Blam's
 * empty marker-name / empty tag-name sentinels — so the reconstruction needs one shared name for it.
 * Extent: IDA types 0x8200155A as a single byte (`byte_8200155A`); 0x8200155B is unreferenced
 * alignment padding. Value: 0x00. Segment .rdata (perm 4, read-only) -> const.
 * Declared `extern const char empty_string[];` in C and `extern "C" const char empty_string[];` in
 * C++ (the C++ TUs must not mangle it, or they would reference a different symbol than this one).
 * Same object as the corpus's former `dsStrongAssertMessage` / `dsEmptyCStr` / the invented
 * `g_conStartMsgReliable`, and as `STRONG_ASSERT_INFO_STUB` in the excluded src/ws/ai tree.
 * /
 */
const char empty_string[1] = "";
