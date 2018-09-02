#ifndef FIXED_POINT_CALCULATIONS_H
#define FIXED_POINT_CALCULATIONS_H
#include <cmath>

struct range {int min; int max;};
// SIGNint_N.K (SIGN: u for unsigned or blank for signed, N: length of data, K: length of fractional part, L: length of decimal part)
struct fxp_info{unsigned int n; unsigned int k; unsigned int l; bool value_signed; double min; double max; double precision; double fxp_value; double fxp_value_dec; double error_abs; double error_rel; bool out_of_size;};

range fxp_getRange(double input_value, bool input_signed);
fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size);

#endif // FIXED_POINT_CALCULATIONS_H
