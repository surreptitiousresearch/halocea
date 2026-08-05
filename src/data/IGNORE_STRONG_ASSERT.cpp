/* IGNORE_STRONG_ASSERT @0x841DB148 (.data) — the Saber global that disables the strong-assert
 * crash path: every `if (!IGNORE_STRONG_ASSERT) STRONG_ASSERT_DUMMY::Crash(...)` guard reads it.
 *
 * DB name `?IGNORE_STRONG_ASSERT@@3HA`, the one symbol of this family the binary actually exports.
 * The mangling is decisive on all three questions the corpus's declarations disagreed about:
 *   `3`  -> a variable (not a function), `H` -> int, `A` -> plain, non-const.
 * So the `bool` / `const bool` / `const char` spellings the tree carried were all wrong, and the
 * definition must have C++ linkage (a C TU cannot emit `?IGNORE_STRONG_ASSERT@@3HA`) — hence a
 * .cpp here rather than the usual src/data/<sym>.c. Segment .data (perm 6, writable), bytes at
 * 0x841DB148 are 00 00 00 00.
 */

int IGNORE_STRONG_ASSERT = 0;
