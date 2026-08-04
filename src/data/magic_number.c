/* magic_number @ 0x8417EEC4 — .data data-init.
 * DB type __int16 (2 bytes); ledger size 4 overcounts (following 2 bytes belong to a
 * separate global). Bytes 00 0C (big-endian) = 0x000C = 12. */
short magic_number = 12;
