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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

/*******************************************************************************
 * 0x0000: | main [ @start:0x0000 ]
 * 0x0000: | ----> | foo [ @start:0x0000 ]
 * 0x0000: | <---- | foo [ @start:0x0000 ]
 * 0x0000: | ----> | bar [ @start:0x0000 ]
 * 0x0000: |       | ----> | foobar [ @start:0x0000 ]
 * 0x0000: |       |       | ----> | foobaz [ @start:0x0000 ]
 * 0x0000: |       |       | <---- | foobaz [ @start:0x0000 ]
 * 0x0000: |       | <---- | foobar [ @start:0x0000 ]
 * 0x0000: | <---- | bar [ @start:0x0000 ]
 * 0x0000: | main [ @start:0x0000 ]
*******************************************************************************/

static unsigned int __call_level = 0;

void __cyg_profile_func_enter( void * func_addr, void * call_site ) __attribute__( ( no_instrument_function ) );
void __cyg_profile_func_enter( void * func_addr, void * call_site )
{
    char         buf[ 1024 ];
    Dl_info      func_info;
    Dl_info      caller_info;
    const char * caller_name;
    const char * func_name;
    unsigned int i;
    
    func_name   = ( dladdr( func_addr, &func_info ) )   ? func_info.dli_sname   : "<unknown>";
    caller_name = ( dladdr( call_site, &caller_info ) ) ? caller_info.dli_sname : "<unknown>";
    buf[ 0 ]    = 0;
    
    if( __call_level > 1 )
    {
        for( i = 0; i < __call_level; i++ )
        {
            strcat( buf, "      | " );
        }
    }
    
    if( __call_level > 0 )
    {
        strcat( buf, "----> | " );
    }
    
    printf
    (
        "0x%010lX: | %s%s [ @%s:0x%010lX ]\n",
        ( uintptr_t )func_addr,
        buf,
        func_name,
        caller_name,
        ( uintptr_t )call_site
    );
    
    __call_level++;
}

void __cyg_profile_func_exit( void * func_addr, void * call_site ) __attribute__( ( no_instrument_function ) );
void __cyg_profile_func_exit( void * func_addr, void * call_site )
{
    ( void )func_addr;
    ( void )call_site;
    
    __call_level--;
}

void foo( void );
void bar( void );
void foobar( void );
void foobaz( void );

void foo( void )
{}

void bar( void )
{
    foobar();
}

void foobar( void )
{
    foobaz();
}

void foobaz( void )
{}

int main( void )
{
    foo();
    bar();
    
    return 0;
}
