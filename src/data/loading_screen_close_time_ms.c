/* loading_screen_close_time_ms @ 0x8417ED24 (.data, 4 bytes)
 * DB applied_types: unsigned int loading_screen_close_time_ms;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xFFFFFFFF
 * .data data-init.
 * DB: unsigned int; bytes FF FF FF FF = 0xFFFFFFFF (NONE / uninitialized-time sentinel).
 */
unsigned int loading_screen_close_time_ms = 0xFFFFFFFFu;
