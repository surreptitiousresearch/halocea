/* noMemLeftInDl @ 0x841CB164 (.data, 4 bytes)
 * DB applied_types: int noMemLeftInDl;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * .data zero-fill. DB symbol ?noMemLeftInDl@@3HA (int).
 * Set by dlDumpNoMemLeft before it forwards to apDumpNoMemLeft, suppressing the vid-mem dump on that path.
 */
int noMemLeftInDl;
