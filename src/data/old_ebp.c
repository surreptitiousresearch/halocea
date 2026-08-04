/* old_ebp @ 0x844AB548 - data-bss pointer (4 bytes). DB type unsigned int*; ledger gap 8.
 * A5 sweep flagged word accesses at +0x4: that is the SEPARATE adjacent global walk_up_current_frame
 * (unsigned int @ 0x844AB54C, reconstructed in src/data/walk_up_current_frame.c) — a stack-walk sibling
 * reached via a shared anchor register. Sibling-global access, not overflow. Type correct. 2026-07-31. */
unsigned int *old_ebp;
