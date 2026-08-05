/* vidMemAllocated @ 0x842CDADC - .data zero-fill. DB symbol ?vidMemAllocated@@3HA.
 * Running total of physical bytes handed out by vidAllocPhysMem; subtracted from the quota in bytes
 * (disasm 0x827CDB40 `subf r31,r9,r7`), so an unsigned byte count. */

unsigned int vidMemAllocated;
