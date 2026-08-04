/* checking_for_updates_closing @ 0x8441E816 -- third-order .data. One-shot latch: set while the
 * "checking for updates" dialog is closing so its per-frame update early-outs. Zero-initialised. */
unsigned char checking_for_updates_closing = 0;
