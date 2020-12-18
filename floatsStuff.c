#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "stdlib.h"
#include "string.h"

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127


/*
    Declare a "typedef struct {} flt;" to contain data for a float
    The data should include:
        An integer for the sign
            (1 for positive, -1 for negative)
        An integer for the exponent value
            (should be bit value to integer minus BIAS or -126 for denormalized)
        A float to contain the value of the mantissa
            (Actual float value extracted from the binary value)
        An integer to contain the mode using the defines above
            (NORM, DNORM and SPEC)
    Example after processing: -15.375 = 1 10000010 11101100000000000000000
        sign = -1
        exp = 3
        man = 0.9218750000
        mode = NORM
*/
typedef struct flt{
    int sign;
    int exponent;
    float  mantissa;
    int mode;
} flt;

/*
    Write a function get_flt_bits_int to return an integer with the
    bits copied from a float.
    Example:
        for f = -15.375,
        the bits of int n = 11000001011101100000000000000000
    Look at the slides and code from the float lectures and use the
    indirection trick.  This can easily be done in one line of code.
    The function should accept a float and return an int.
*/

int get_flt_bits_int(float f){  //get the bits of a float as an int for bitwise manipulation.
    int n = *(int*)&f;   //INDIRECTION TECHNIQUE
    return n;
}
/*
    Write a function that returns the sign of a float as a char.
    You should call get_flt_bits_int to get the bits in an int
    and return '1' if the sign is negative else return '0'.  The
    function should accept a float and return a string.
*/

char get_flt_sign_char(float f){
    int n = get_flt_bits_int(f);

    if(n & INT_MIN){   //2<<31   leftmost bit 1
        return '1';   //negative
    }
    return '0';
}

/*
    Write a function that returns the sign of a float as an integer.
    You should call get_flt_bits_int to get the bits in an int
    and return -1 if the sign is negative else return 1.  The function
    should accept a float and return an int.
*/
int get_flt_sign_val(float f){
    int n = get_flt_bits_int(f);

    if(n & INT_MIN){
        return -1;
    }
    return 1;

}
/*
    Write a function to return a string containing the
    actual binary value of the exponent of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 1 10000010 11101100000000000000000
            the exponent bits are "10000010"
    The function should accept a float and return a string.
*/
char* get_flt_exp_str(float f){
    int n = get_flt_bits_int(f);

    char* exponent = (char*) malloc(9 * sizeof(char));
    if(exponent == NULL){
        printf(("Memory was not allocated for exponent array\n"));
        exit(0);
    }
//        https://www.geeksforgeeks.org/binary-representation-of-a-given-number/
    int i;
    for(i = 0; i< 8; i++){
       n<<= 1;  //so as to not count the sign bit
        exponent[i] = (n & INT_MIN)  ? '1' : '0';
    }
//same thing as right shifting it 23 times and then doing bitwise and with 1 like in lecture but would also be backwards
    exponent[i] = '\0';
    return exponent;

}


/*
    Write a function to return an integer containing the
    actual integer value of the exponent of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the exponent value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the actual value of the exponent is 3
    The function should accept a float and return an int.
*/
int get_flt_exp_val(float f){
    char* strExponent = get_flt_exp_str(f);

    int i, move = 0, w = 0;
    for(i = 7; i >= 0; i--){
        move += ((strExponent[w++] - '0') * pow(2, i) );

    }
    free(strExponent);
//    printf("%d\n", move);
    return move - BIAS;
}

/*
    Write a function to return an integer containing the
    mode of the exponent of a float.  You should call
    get_flt_exp_val to get the bits in an int and return
    the int with the mode value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the mode is NORM
    The function should accept a float and return an int.
*/

int get_flt_exp_mode(float f){
    int n = get_flt_exp_val(f);

    if(n == 128){   //in the lab outline says that sqrt(-1) and infin should be 255?
        return SPEC;
    }
    else if(n == -127 ){   //DNORM exponent is 1 - BIAS (bias = 127)
        return DNORM;
    }
    else{
        return NORM;
    }
}

/*
    Write a function to return a string containing the
    actual binary value of the mantissa of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are "11101100000000000000000"
    The function should accept a float and return a string.
*/


char*  get_flt_man_str(float f){
    int n = get_flt_bits_int(f);

    char* mantissa = (char*) malloc(24 * sizeof(char));
    if(mantissa == NULL){
        printf("Memory was not allocated for mantissa array.");
        exit(0);
    }

    n<<= 9;
    int i;
    for(i = 0; i< 23; i++){
        mantissa[i] = (n & INT_MIN) ? '1' : '0';
        n<<= 1;
    }
    mantissa[i] = '\0';
    return mantissa;

}

/*
    Write a function to return a float containing the
    actual float value of the mantissa of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the mantissa value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are 11101100000000000000000
            the actual value of the mantissa is 0.9218750000
    The function should accept a float and return an int.
*/
float get_flt_man_val(float f){
    char* strMantissa = get_flt_man_str(f);

    int i;  float val = 0;
    for(i = 0; i  < 23 ; i++){
        val += (strMantissa[i] -'0') * pow(2, -1 * (i +1));
    }
    free(strMantissa);
    return val;

}

/*
    Write a function to return a string containing the
    actual binary value of a float in a char array.  You
    should call get_flt_sign_char, get_flt_exp_str and
    get_flt_man_str to get the bits in an char and two
    strings and return the concatenated string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            The sign is '1'
            the exponent is "10000010"
            and the mantissa bits are "11101100000000000000000"
            The string should be formatted as:
                "1 10000010 11101100000000000000000" to clearly
                separate the 3 parts.
    The function should accept a float and return a string.
*/
char* get_flt_bits_str(float f){

    char sign = get_flt_sign_char(f);
    char* exp = get_flt_exp_str(f);
    char* man = get_flt_man_str(f);

    char* binValue = (char*) malloc(35 * sizeof(char));

    binValue[0] = sign;
    binValue[1] = ' ';
    strcat(strcat(strcat(strcat(binValue, " "), exp), " "), man);

    return binValue;
}

/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/

 struct flt get_flt_val_flt(float f){
    flt flts;
    flts.sign = get_flt_sign_val(f);
    flts.exponent = get_flt_exp_val(f);
    flts.mantissa =  get_flt_man_val(f);

    flts.mode = get_flt_exp_mode(f);

    if(flts.mode == DNORM){
        flts.exponent = -126;
    }
    return flts;



}


/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/
void printFltStructure(flt Flt){  //check
    printf("sign = %d ", Flt.sign);
    printf("exp = %d ", Flt.exponent);
    printf("man = %f ", Flt.mantissa);

    if(Flt.mode == NORM) printf("Normalized\n");
    if(Flt.mode == DNORM) printf("Denormalized\n");
    if (Flt.mode == SPEC) printf("Specialized\n");
}

/*
    Write a function to get the actual float value back
    out of a flt struct.
    Hints:
        The float value produced will depend on the mode.
        To set a float to infinity use the math library constant INFINITY
        To set a float to not-a-number use the math library constant NAN
        Check the slides and text for conditions for NORN, DNORM and SPEC
        You need to return (sign) * M * 2^e
*/
float get_flt_bits_val(flt flts){

   if(flts.mode == DNORM){

   }
   else if(flts.mode == NORM){
//       printf("%d\n", flts.sign);
//       printf("%f\n", flts.mantissa);
//       printf("%f\n", pow(2, flts.exponent));

       return flts.sign * (1+ flts.mantissa )* pow(2, flts.exponent);

   }
   else if(flts.mode == SPEC){
       if(flts.mantissa != 0) return NAN;

       else if(flts.sign == -1){
           return -INFINITY;
       }
       else if(flts.sign == 1){
           return INFINITY;
       }
   }


}




/*
    Write a main function that calls an prints results for
    each function when completed.
    get_flt_sign_char
    get_flt_sign_val

    get_flt_exp_str
    get_flt_exp_val

    get_flt_man_str
    get_flt_man_val

    get_flt_bits_str

    get_flt_val_flt
    print_flt

    get_flt_bits_val
*/
int main(){

    float n = -15.375;    //e = 3 or 130?
//    float n = sqrt(-1);    //e = 128 or 255   is 128
//    float n = INFINITY;   //e = 128 or 255
//    float n = -INFINITY;    //e = 128 or 255
//    float n = .0000002;
//    float n = 0.0;          //e = -127

//    printf("f: %d\n",  get_flt_bits_int(n));  // 15.375-> hex = 41760000-> decimal =  1098252288


//----
    printf("sig: %c\n",get_flt_sign_char( n) );  //prints sig: 1
//https://stackoverflow.com/questions/3779202/checking-whether-a-number-is-positive-or-negative-using-bitwise-operators/3779452
//explains the n & INT_MIN

    printf("s: %d\n",get_flt_sign_val(n) );  //prints s: 1

    char* exponent = get_flt_exp_str(n);
    printf("exp: %s\n",exponent);
    free(exponent);

    printf("e: %d\n", get_flt_exp_val(n));


//    printf("mode: %d\n", get_flt_exp_mode(n));

    char* mantissa = get_flt_man_str(n);
    printf("man: %s\n", mantissa);  //  man = 11101100000000000000000
    free(mantissa);

    printf("m: %f\n", get_flt_man_val(n));


    char* bits = get_flt_bits_str(n);  //01000001011101100000000000000000
    printf("bits: %s\n", bits);
    free(bits);


    struct flt flts = get_flt_val_flt(n);
    printFltStructure(flts);




    printf("ff = %f", get_flt_bits_val(flts));










    return 0;
}
