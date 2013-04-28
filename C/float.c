/*******************************************************************************
 * Copyright (c) 2010, Jean-David Gadina - www.xs-labs.com
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  -   Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *  -   Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  -   Neither the name of 'Jean-David Gadina' nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef __bool_true_false_are_defined
    #define bool                            _Bool
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

/*!
 * @function    binary_to_float
 * @abstract    Converts a integer to its float representation
 * @description This function converts a 32 bits integer to a single precision
                floating point number, as specified by the IEEE Standard for
                Floating-Point Arithmetic (IEEE 754). This standard can be
                found at the folowing address:
                http://ieeexplore.ieee.org/servlet/opac?punumber=4610933
 * @param       binary  The integer to convert to a floating point value
 * @result      The floating point number
 * @copyright   Jean-David Gadina - www.xs-labs.com
 */
float binary_to_float( unsigned long binary );
float binary_to_float( unsigned long binary )
{
    /* Storage for the return value */
    float floatValue = 0;
    
    /* Counter */
    signed int i = 0;
    
    /* Gets the sign field */
    /* Bit 0, left to right */
    bool sign = binary >> 31;
    
    /* Gets the exponent field */
    /* Bits 1 to 8, left to right */
    unsigned char exponent = ( ( binary >> 23 ) & 0xFF );
    
    /* Gets the mantissa field */
    /* Bits 9 to 32, left to right */
    unsigned long mantissa = ( binary & 0x7FFFFF );
    
    /* Checks the values of the exponent and the mantissa fields to handle */
    /* special numbers */
    if( exponent == 0 && mantissa == 0 )
    {
        /* Zero - No need for a computation even if it can be considered */
        /* as a denormalized number */
        return 0;
    }
    else if( exponent == 255 && mantissa == 0 )
    {
        /* Infinity */
        return 0;
    }
    else if( exponent == 255 && mantissa != 0 )
    {
        /* Not a number */
        return 0;
    }
    else if( exponent == 0 && mantissa != 0 )
    {
        /* Donormalized number - Exponent is fixed to -126 */
        exponent = -126;
    }
    else
    {
        /* Computes the real exponent */
        exponent = exponent - 127;
        
        /* Adds the implicit bit to the mantissa */
        mantissa = mantissa | 0x800000;
    }
    
    /* Process the 24 bits of the mantissa */
    for( i = 0; i > -24; i-- )
    {
        /* Checks if the current bit is set */
        if( mantissa & ( 1 << ( i + 23 ) ) ) {
            
            /* Adds the value for the current bit */
            /* This is done by computing two raised to the power of the */
            /* exponent plus the bit position (negative if it's after the */
            /* implicit bit, as we are using scientific notation) */
            floatValue += ( float )pow( 2, i + exponent );
        }
    }
    
    /* Returns the final float value */
    return ( sign == false ) ? floatValue : -floatValue;
}

/*!
 * @abstract        main
 * @description     Program's entry point
 * @param           argc    The number of command line arguments
 * @param           argv    An array with the command line arguments
 * @result          The process exit status
 */
int main( int argc, char *argv[] )
{
    ( void )argc;
    ( void )argv;
    printf( "%f\n", binary_to_float( 0x40B80000 ) );
    return EXIT_SUCCESS;
}
