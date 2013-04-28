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
 * @header          fibonascii.h
 * @copyright       Jean-David Gadina - www.xs-labs.com
 * @abstract        Fibonascii main header file
 * @version         0.0.1
 * @availability    0.0.1
 * @updated         $Date$
 */

#ifndef __FIBONASCII_H__
#define __FIBONASCII_H__
#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/* Definition of the boolean data type if necessary */
#ifndef __bool_true_false_are_defined
    #define bool                            _Bool
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

/* Utility macros for GCC */
#if defined( __GNUC__ )
    
    /* Version checking */
    #if     defined( __GNUC_MINOR__ )       \
        &&  defined( __GNUC_PATCHLEVEL__ )  \
        && !defined( __GCC_VERSION__ )
    
    /*!
     * @define          __GCC_VERSION__
     * @abstract        GCC version number
     * @availability    N/A
     */
    #define __GCC_VERSION__ ( __GNUC__       * 10000    \
                            + __GNUC_MINOR__ * 100      \
                            + __GNUC_PATCHLEVEL__ )
    
    #endif

#endif

/* Size conversion macros */
#define BYTE_TO_KB( b ) (               b / ( long double )1000 )
#define BYTE_TO_MB( b ) ( (             b / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_GB( b ) ( ( (           b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_TB( b ) ( ( ( (         b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_PB( b ) ( ( ( ( (       b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_EB( b ) ( ( ( ( ( (     b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_ZB( b ) ( ( ( ( ( ( (   b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )
#define BYTE_TO_YB( b ) ( ( ( ( ( ( ( ( b / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 ) / ( long double )1000 )

/* Size limits macros */
#define SIZE_MAX_BYTE       1000.0f
#define SIZE_MAX_KILOBYTE   1000000.0f
#define SIZE_MAX_MEGABYTE   1000000000.0f
#define SIZE_MAX_GIGABYTE   1000000000000.0f
#define SIZE_MAX_TERABYTE   1000000000000000.0f
#define SIZE_MAX_PETABYTE   1000000000000000000.0f
#define SIZE_MAX_EXABYTE    1000000000000000000000.0f
#define SIZE_MAX_ZETTABYTE  1000000000000000000000000.0f
#define SIZE_MAX_YOTTABYTE  1000000000000000000000000000.0f

/* Stringification macros */
#define XSTR(s) STR(s)
#define  STR(s) #s

#ifdef __cplusplus
}
#endif

#endif /* __FIBONASCII_H__ */
