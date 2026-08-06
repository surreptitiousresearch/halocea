/* magic_number @ 0x8417EEC4 (.data, 2 bytes)
 * DB applied_types: unsigned __int16 magic_number;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 000c (2 trailing bytes)
 * .data data-init.
 * DB type __int16 (2 bytes); ledger size 4 overcounts (following 2 bytes belong to a
 * separate global). Bytes 00 0C (big-endian) = 0x000C = 12.
 */
short magic_number = 12;
