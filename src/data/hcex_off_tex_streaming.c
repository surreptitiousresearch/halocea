/* hcex_off_tex_streaming @ 0x841768F4 — .data data-init. DB applied type is int;
 * bytes 00 00 00 01 (big-endian) = 1. (Some consumers inline-extern this as
 * unsigned char, which on this big-endian target would read the 0x00 MSB — a
 * pre-existing consumer discrepancy; DB type/int value is authoritative.) */
int hcex_off_tex_streaming = 1;
