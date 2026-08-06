/* checking_for_updates_closing @ 0x8441E816 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 checking_for_updates_closing;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 00 (1 trailing byte)
 * third-order .data. One-shot latch: set while the
 * "checking for updates" dialog is closing so its per-frame update early-outs. Zero-initialised.
 */
unsigned char checking_for_updates_closing = 0;
