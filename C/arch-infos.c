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

#include <stdio.h>

#define XSTR(s) STR(s)
#define  STR(s) #s

int main( void )
{
    printf
    (
        "\n"
        "System informations:\n"
        "\n"
        "- Architecture:       "
        #if defined ( __i386__ ) || defined( __x86_64__ )
            #if defined( __x86_64__ )
            "x86-64\n"
            #else
            "i386\n"
            #endif
        #elif defined ( __arm__ )
            "ARM\n"
        #elif defined ( __ppc__ ) || defined ( __ppc64__ )
            #if defined( __ppc64__ )
            "PPC-64\n"
            #else
            "PPC\n"
            #endif
        #else
            "N/A\n"
        #endif
        "- Standard-C:         "
        #ifdef __STDC__
        "yes\n"
        #else
        "no\n"
        #endif
        #ifdef __STDC_VERSION__
        "- Standard-C version: %li\n"
        #else
        "- Standard-C version: N/A\n"
        #endif
        #if defined( __STDC_HOSTED__ ) && __STDC_HOSTED__ >= 1
        "- Standard-C hosted:  yes\n"
        #else
        "- Standard-C hosted:  no\n"
        #endif
        "- Strict ANSI:        "
        #ifdef __STRICT_ANSI__
        "yes\n"
        #else
        "no\n"
        #endif
        "- LP64:               "
        #ifdef __LP64__
        "yes\n"
        #endif
        #if defined( __ASSEMBLER__ ) && __ASSEMBLER__ >= 1
        "- Assembler:          yes\n"
        #else
        "- Assembler:          no\n"
        #endif
        #if defined( __OBJC__ ) && __OBJC__ >= 1
        "- Objective-C:        yes\n"
        #else
        "- Objective-C:        no\n"
        #endif
        #ifdef __cplusplus
        "- C++:                yes\n"
        #else
        "- C++:                no\n"
        #endif
        "- GNU-C:              "
        #ifdef __GNUC__
        "yes\n"
        #else
        "no\n"
        #endif
        "- GNU Fortran:        "
        #ifdef __GFORTRAN__
        "yes\n"
        #else
        "no\n"
        #endif
        "- ELF:                "
        #ifdef __ELF___
        "yes\n"
        #else
        "no\n"
        #endif
        "- SSP:                "
        #ifdef __SSP__
        "yes\n"
        #else
        "no\n"
        #endif
        "- Register prefix:    "
        #ifdef __REGISTER_PREFIX__
        "[ %1s ]\n"
        #else
        "N/A\n"
        #endif
        "- User label prefix:  "
        #ifdef __USER_LABEL_PREFIX__
        "[ %1s ]\n"
        #else
        "N/A\n"
        #endif
        "\n"
         #ifdef __STDC_VERSION__
        , __STDC_VERSION__
        #endif
         #ifdef __REGISTER_PREFIX__
        , XSTR( __REGISTER_PREFIX__ )
        #endif
         #ifdef __USER_LABEL_PREFIX__
        , XSTR( __USER_LABEL_PREFIX__ )
        #endif
    );
    
    return 0;
}
