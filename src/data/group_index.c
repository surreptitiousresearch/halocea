/* group_index @ 0x8446841C - data-bss zero-fill; DB applied-type `__int16 group_index;`. DB type is 2 bytes; ledger size 52 is the next-symbol gap heuristic (overcount) - DB authoritative.
 * P4 sweep flagged +0x4 (lwzx/stwx): that is a SEPARATE word array (DB dword_84468420) indexed off the
 * transparent_geometry_group_count anchor (0x84468458), not an extension of this __int16. group_index is
 * accessed only as a halfword (lhz/sth) on its own anchor. Type __int16 is correct. */

__int16 group_index;
