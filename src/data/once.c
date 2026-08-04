/* @0x84184CC4. Consumers declare `extern unsigned __int8 once;` (run-once guard).
 * DB applied type is __int16 and the ledger marks data-init, but the defining byte is 0x00
 * (the following 0x01 belongs to a separate adjacent global); a zero uchar is the correct value. */
unsigned __int8 once;
