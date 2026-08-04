/* notEnoughMem @ 0x842CDAE0 -- third-order .data. One-shot latch set when the physical memory
 * pool is exhausted (vidAllocPhysMem), after which further physical allocations fail. Zero-init. */
int notEnoughMem = 0;
