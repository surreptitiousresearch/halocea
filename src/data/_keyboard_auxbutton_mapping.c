/* keyboard_auxbutton_mapping @ 0x84177F60 — data-init __int16 array.
 *
 * Recovered from binary bytes (big-endian): 0010 0000 000D  -> {16, 0, 13}.
 * Consumers (input_abstraction_debounce_auxbutton_key.c, input_abstraction_update.c) iterate
 * `for (m = keyboard_auxbutton_mapping; m < &keyboard_auxbutton_mapping_end; ++m)`, i.e. exactly 3 __int16 elements
 * (0x84177F60..0x84177F66); the DB applied-type also models it as __int16[3]. The trailing 0x0000 at
 * +6 is the separate one-past-end sentinel symbol keyboard_auxbutton_mapping_end (out of scope here).
 *
 * NOTE: ledger symbol is `_keyboard_auxbutton_mapping` (IDA leading-underscore display name) and lists
 * size 8; the true source symbol consumers link against is `keyboard_auxbutton_mapping` with size 6
 * (int16[3]) — the extra 2 bytes belong to keyboard_auxbutton_mapping_end. Defined under the consumer-facing name. */

short keyboard_auxbutton_mapping[3] = { 16, 0, 13 };
