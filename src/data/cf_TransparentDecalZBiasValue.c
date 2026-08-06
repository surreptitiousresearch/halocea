/* cf_TransparentDecalZBiasValue @ 0x84174978 (.data, 4 bytes)
 * DB applied_types: float cf_TransparentDecalZBiasValue;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xB6A7C5AC
 * third-order .data (surfaced by data_closure.py).
 * D3D transparent-decal constant depth-bias. Bit pattern 0xB6A7C5AC == -5.0e-6f; the consumer
 * (rasterizer_dx9_set_transparent_decal_zbias) reads it via *(const unsigned int*) to feed the
 * depth-bias render state, so the exact bits matter.
 */
float cf_TransparentDecalZBiasValue = -5.0e-6f;
