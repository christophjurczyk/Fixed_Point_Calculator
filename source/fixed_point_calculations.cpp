#include "fixed_point_calculations.h"

range fxp_getRange(double input_value, bool input_signed){
    range result;

    // max value
    result.max = static_cast<int>(abs(round(input_value)));
    // min value
    if(input_signed)
    {
        // signed
        result.min = -result.max;
    }else{
        // unsigned
        result.min = 0;
    }


    return result;
}

fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size){
    fxp_info result;

    result.n = size;
    if(range_min < 0)
    {
        result.value_signed = true;
    }else{
        result.value_signed = false;
    }

    // find max absolute value of range
    int range_max_abs_value;
    if(result.value_signed){
        // signed
        if(range_max > abs(range_min)){
            range_max_abs_value = range_max;
        }else{
            range_max_abs_value = abs(range_min);
        }

    }else{
        // unsigned -> max absoulte value is range_max
        range_max_abs_value = range_max;
    }

    // length of decimal part
    result.l = static_cast<unsigned int>(ceil(log2(range_max_abs_value)) + 1);
    // length of fractional part
    if(result.l < result.n)
    {
         result.k = result.n - result.l;
    }else{
        result.k = 0;
    }
    // minimum value
    if(result.value_signed){
        result.min = -pow(2,result.n - 1 - result.k);
    }else{
        result.min = 0;
    }
    // maximum value    
    result.max = pow(2,result.n - 1 - result.k) - pow(2,-static_cast<int>(result.k));
    // precision
    result.precision = pow(2,-static_cast<int>(result.k));
    // range check of input value
    if((input_value < result.min) || (input_value > result.max + result.precision)){
        result.out_of_size = true;
        return result;
    }else{
        result.out_of_size = false;
    }
    // fixed point value in decimal
    if(input_value == (result.max + result.precision)){
        // workaround for positiv boundary number
           result.fxp_value_dec = pow(2,result.n - 1) - 1;
    }else if(input_value >= 0){
            result.fxp_value_dec = round(input_value/result.precision);
    }else if(input_value < 0){
            result.fxp_value_dec = pow(2,result.n - 1) + round((pow(2,result.l-1) + input_value)/result.precision);
    }
    // value
    if(result.value_signed){
        if(result.fxp_value_dec > pow(2,result.n - 1)-1)
        {
            // negative number
            result.fxp_value = -pow(2,result.n - 1 - result.k) + (result.fxp_value_dec - pow(2,result.n -1))*result.precision;
        }else{
            // positive number
            result.fxp_value = result.precision * result.fxp_value_dec;
        }
    }else{
        result.fxp_value = result.precision * result.fxp_value_dec;
    }

    // absolute error
    result.error_abs = input_value - result.fxp_value;
    // relative error
    result.error_rel = 100 - (input_value/result.fxp_value) * 100;

    return result;
}
