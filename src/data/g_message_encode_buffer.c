/* g_message_encode_buffer @ 0x846DA300 — data-bss scratch buffer.
 * Consumers declare it `char g_message_encode_buffer[]` (unsized). Actual extent to the
 * next named global (widget_data @ 0x846DB300) is exactly 4096 bytes, matching the ledger.
 * NOTE: the DB applied-type reads `unsigned __int8[4095]` (off-by-one); 4096 is the true span. */
char g_message_encode_buffer[4096];
