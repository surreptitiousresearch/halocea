/* local_layer @ 0x8445D3B0 (.data, 2 bytes)
 * DB applied_types: __int16 local_layer;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0000 (2 trailing bytes)
 * data-bss. DB type __int16 (2); ledger gap 10 overcounts.
 * A5 sweep flagged word accesses at +0x4 (0x8445D3B4) and halfword at +0x8 (0x8445D3B8): those are
 * UNNAMED sibling rasterizer-local globals in the gap before local_filthy_decal_fog_hack_enabled
 * (0x8445D3BA), reached via a shared small-data anchor register. Sibling-block access, not overflow.
 * Type __int16 correct. 2026-07-31.
 */
short local_layer;
