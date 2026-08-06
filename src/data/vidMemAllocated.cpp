/* vidMemAllocated @ 0x842CDADC (.data, 4 bytes)
 * DB applied_types: int vidMemAllocated;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * .data zero-fill. DB symbol ?vidMemAllocated@@3HA — MANGLED, so this storage definition must have
 * C++ linkage (a C TU cannot emit `?vidMemAllocated@@3HA`); hence a .cpp here rather than the usual
 * src/data/<sym>.c, following src/data/IGNORE_STRONG_ASSERT.cpp. It is therefore absent from
 * blam_data_globals.h, which declares the C-linkage globals only.
 *
 * DEVIATION: was `unsigned int`, on the reasoning that it is "a byte count" and that
 * `subf r31,r9,r7` @0x827CDB40 subtracts it from the quota. Both oracles this file already quotes
 * say otherwise — applied_types states `int` and the mangling's `H` is int (`I` would be unsigned
 * int) — and `subf` is sign-agnostic, so it was never evidence either way. Under C++ the wrong
 * spelling is not cosmetic: `unsigned int` mangles to ?vidMemAllocated@@3IA, a different symbol.
 * Running total of physical bytes handed out by vidAllocPhysMem.
 */
int vidMemAllocated;
