/* cf_TransparentDecalZBiasValue @ 0x84174978 -- third-order .data (surfaced by data_closure.py).
 * D3D transparent-decal constant depth-bias. Bit pattern 0xB6A7C5AC == -5.0e-6f; the consumer
 * (rasterizer_dx9_set_transparent_decal_zbias) reads it via *(const unsigned int*) to feed the
 * depth-bias render state, so the exact bits matter. */
float cf_TransparentDecalZBiasValue = -5.0e-6f;
