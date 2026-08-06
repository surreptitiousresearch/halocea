/* base_seat_labels @ 0x841761B8 (.data, 24 bytes)
 * DB applied_types: const char *base_seat_labels[6];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82113340 -> "asleep"
 *   +0x0004 = 0x820DC020 -> "alert"
 *   +0x0008 = 0x82113338 -> "stand"
 *   +0x000C = 0x82113330 -> "crouch"
 *   +0x0010 = 0x82113328 -> "flee"
 *   +0x0014 = 0x82113320 -> "flaming"
 * data-init const char *[6]. Pointers recovered from the
 * binary (big-endian); string targets read from .rdata.
 */
const char *base_seat_labels[6] =
{
    "asleep",  /* 0 */
    "alert",   /* 1 */
    "stand",   /* 2 */
    "crouch",  /* 3 */
    "flee",    /* 4 */
    "flaming", /* 5 */
};
