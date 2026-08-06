/* notEnoughMem @ 0x842CDAE0 (.data, 4 bytes)
 * DB applied_types: int notEnoughMem;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * third-order .data. One-shot latch set when the physical memory
 * pool is exhausted (vidAllocPhysMem), after which further physical allocations fail. Zero-init.
 */
int notEnoughMem = 0;
