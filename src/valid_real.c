/* valid_real @0x836DE230 — true when the value is not a NaN. The disassembly passes the float
 * argument straight to the CRT _isnan and returns the logical negation of its result. */

extern int _isnan(double value);

int valid_real(float value)
{
    return _isnan(value) == 0;
}
