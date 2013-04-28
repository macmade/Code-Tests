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

/*!
 * @file        fibonascii.c
 * @copyright   Jean-David Gadina - www.xs-labs.com
 * @abstract    Main executable
 */

/* System includes */
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Local includes */
#include "fibonascii.h"

/*!
 * @abstract        main
 * @description     Program's entry point
 * @param           argc    The number of command line arguments
 * @param           argv    An array with the command line arguments
 * @result          The process exit status
 */
int main( void )
{
    unsigned int  i;
    int           flt_rounds;
    char          flt_rounds_str[ 256 ];
    char          size_str[ 256 ];
    char        * unit;
    char        * unit_name;
    unsigned long fibo_int[ 256 ];
    long double   fibo_dbl[ 256 ];
    long double   size;
    long double   cur;
    bool          overflow;
    
    flt_rounds = FLT_ROUNDS;
    overflow   = false;
    
    memset( flt_rounds_str, 0, 256 );
    memset( size_str,       0, 256 );
    
    switch( flt_rounds )
    {
        case -1: sprintf( flt_rounds_str, "%18i - Indeterminable",          flt_rounds ); break;
        case  0: sprintf( flt_rounds_str, "%21i - Toward zero",             flt_rounds ); break;
        case  1: sprintf( flt_rounds_str, "%22i - To nearest",              flt_rounds ); break;
        case  2: sprintf( flt_rounds_str, "%8i - Toward positive infinity", flt_rounds ); break;
        case  3: sprintf( flt_rounds_str, "%8i - Toward negative infinity", flt_rounds ); break;
        default: sprintf( flt_rounds_str, "%10i - Implementation defined",  flt_rounds ); break;
    }
    
    printf
    (
        "\n"
        "--------------------------------------------------------------------------------\n"
        "|                                                                              |\n"
        "|                                - FIBONASCII -                                |\n"
        "|                                                                              |\n"
        "--------------------------------------------------------------------------------\n"
        #if defined( __ppc__)
        "| Processor type:                                                          PPC |\n"
        #elif defined( __ppc64__ )
        "| Processor type:                                                       PPC-64 |\n"
        #elif defined( __i386__ )
        "| Processor type:                                                         i386 |\n"
        #elif defined( __x86_64__ )
        "| Processor type:                                                       x86-64 |\n"
        #elif defined( __arm__ )
        "| Processor type:                                                          ARM |\n"
        #else
        "| Processor type:                                                    <unknown> |\n"
        #endif
        #ifdef __LP64__
        "| LP64:                                                                    Yes |\n"
        #else
        "| LP64:                                                                     No |\n"
        #endif
        "--------------------------------------------------------------------------------\n"
        #ifdef LONG_BIT
        "| Unsigned long integer size:          %34i bits |\n"
        #else
        "| Unsigned long integer bits:                                        <unknown> |\n"
        #endif
        "| Unsigned long integer maximum value: %39lu |\n"
        "--------------------------------------------------------------------------------\n"
        "| Radix of floating-point representations: %35i |\n"
        "| Floating-point rounding mode:            %35s |\n"
        "--------------------------------------------------------------------------------\n"
        "| Long double precision:                 %37i |\n"
        "| Long double epsilon:                   %.31Le |\n"
        "| Long double mantissa:                  %37i |\n"
        "| Long double maximum value:             %.29Le |\n"
        "| Long double largest exponent:          %37i |\n"
        "| Long double minimum normalized number: %.29Le |\n"
        "| Long double smallest exponent:         %37i |\n"
        "--------------------------------------------------------------------------------\n"
        "| Dec: | Hex: | ASCII:  | Fibonacci number:          | Size:       | Unit:     |\n"
        "--------------------------------------------------------------------------------\n",
        #ifdef LONG_BIT
        LONG_BIT,
        #endif
        ULONG_MAX,
        FLT_RADIX,
        flt_rounds_str,
        LDBL_DIG,
        LDBL_EPSILON,
        LDBL_MANT_DIG,
        LDBL_MAX,
        LDBL_MAX_EXP,
        LDBL_MIN,
        LDBL_MIN_EXP
    );
    
    for( i = 0; i < 256; i++ )
    {
        if( i > 1 )
        {
            if( overflow == false && ULONG_MAX - fibo_int[ i - 2 ] < fibo_int[ i - 1 ] )
            {

                printf
                (
                    "--------------------------------------------------------------------------------\n"
                    "|                       - UNSIGNED LONG LIMIT REACHED -                        |\n"
                    "--------------------------------------------------------------------------------\n"
                    "| Dec: | Hex: | ASCII:  | Fibonacci number:          | Size:       | Unit:     |\n"
                    "--------------------------------------------------------------------------------\n"
                );
                
                overflow = true;
                
                fibo_dbl[ i - 2 ] = ( long double )fibo_int[ i - 2 ];
                fibo_dbl[ i - 1 ] = ( long double )fibo_int[ i - 1 ];
            }
            
            fibo_dbl[ i ] = fibo_dbl[ i - 2 ] + fibo_dbl[ i - 1 ];
            fibo_int[ i ] = fibo_int[ i - 1 ] + fibo_int[ i - 2 ];
        }
        else
        {
            fibo_int[ i ] = 1;
        }
        
        cur = ( overflow == true ) ? fibo_dbl[ i ] : ( long double )fibo_int[ i ];
        
        if( fibo_int[ i ] < SIZE_MAX_BYTE )
        {
            unit      = " B";
            unit_name = "Byte";
            size      = cur;
            
            sprintf( size_str, "%8.*Lf", 0, size );
        }
        else if( cur < SIZE_MAX_KILOBYTE )
        {
            unit      = "KB";
            unit_name = "KiloByte";
            size      = BYTE_TO_KB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_MEGABYTE )
        {
            unit      = "MB";
            unit_name = "MegaByte";
            size      = BYTE_TO_MB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_GIGABYTE )
        {
            unit      = "GB";
            unit_name = "GigaByte";
            size      = BYTE_TO_GB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_TERABYTE )
        {
            unit      = "TB";
            unit_name = "TeraByte";
            size      = BYTE_TO_TB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_PETABYTE )
        {
            unit      = "PB";
            unit_name = "PetaByte";
            size      = BYTE_TO_PB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_EXABYTE )
        {
            unit      = "EB";
            unit_name = "ExaByte";
            size      = BYTE_TO_EB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else if( cur < SIZE_MAX_ZETTABYTE )
        {
            unit      = "ZB";
            unit_name = "ZettaByte";
            size      = BYTE_TO_ZB( cur );
            
            sprintf( size_str, "%8.*Lf", 2, size );
        }
        else
        {
            unit      = "YB";
            unit_name = "YottaByte";
            size      = BYTE_TO_YB( cur );
            
            if( cur < SIZE_MAX_YOTTABYTE )
            {
                sprintf( size_str, "%8.*Lf", 2, size );
            }
            else
            {
                sprintf( size_str, "%8.2Le", size );
            }
        }
        
        if( overflow == true )
        {
            if( isprint( i ) && i != 0x20 )
            {
                printf( "| %-3i  | 0x%02X |    %c    | %26.20Le | %s %s | %9s |\n", i, i, i, fibo_dbl[ i ], size_str, unit, unit_name );
            }
            else
            {
                printf( "| %-3i  | 0x%02X |   ...   | %26.20Le | %s %s | %9s |\n", i, i, fibo_dbl[ i ], size_str, unit, unit_name );
            }
        }
        else
        {
            if( isprint( i ) && i != 0x20 )
            {
                printf( "| %03i  | 0x%02X |    %c    | %26lu | %s %s | %9s |\n", i, i, i, fibo_int[ i ], size_str, unit, unit_name );
            }
            else
            {
                printf( "| %03i  | 0x%02X |   ...   | %26lu | %s %s | %9s |\n", i, i, fibo_int[ i ], size_str, unit, unit_name );
            }
        }
    }
    
    overflow = false;
    
    for( i = 0; i < 255; i++ )
    {
        if( size + BYTE_TO_YB( fibo_dbl[ i ] ) < size )
        {
            overflow = true;
            break;
        }
        
        size += BYTE_TO_YB( fibo_dbl[ i ] );
    }
    
    if( overflow == true )
    {
        sprintf( size_str, "Not computable" );
    }
    else
    {
        sprintf
        (
            size_str,
            "%*.*Le %s | %s",
            46 - ( int )strlen( unit_name ),
            40 - ( int )strlen( unit_name ),
            size,
            unit,
            unit_name
        );
    }
    
    printf
    (
        "--------------------------------------------------------------------------------\n"
        "| Total size needed:    | %52s |\n"
        "--------------------------------------------------------------------------------\n"
        "\n",
        size_str
    );
    
    return EXIT_SUCCESS;
}
