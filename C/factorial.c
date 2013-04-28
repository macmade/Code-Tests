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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long double get_time( void );
long double get_time( void )
{
    struct timeval  t;
    struct timezone tzp;
    
    gettimeofday( &t, &tzp );
    
    return t.tv_sec + t.tv_usec * 1e-6;
}

unsigned long factorial_recursive( unsigned long n );
unsigned long factorial_recursive( unsigned long n )
{
    if( n == 0 )
    {
        return 1;
    }
    
    return n * factorial_recursive( n - 1 );
}

unsigned long factorial_while( unsigned long n );
unsigned long factorial_while( unsigned long n )
{
    unsigned long x = n;
    
    if( n == 0 )
    {
        return 1;
    }
    
    while( n > 1 )
    {
        x *= --n;
    }
    
    return x;
}

unsigned long factorial_asm( unsigned long n );
unsigned long factorial_asm( unsigned long n )
{
    unsigned long x;
    
    if( n == 0 )
    {
        return 1;
    }
    
    __asm__
    (
        "    movq %[n], %%rax \n\t"
        "    movq %[n], %%rcx \n\t"
        "    decq       %%rcx \n\t"
        "FACTORIAL:"
        "    mulq %%rcx \n\t"
        "    decq %%rcx \n\t"
        "    jnz FACTORIAL \n\t"
        "    movq %%rax, %[x]"
        
        : [ x ] "=m" ( x )
        : [ n ] "m"  ( n )
    );
    
    return x;
}

unsigned long factorial_for( unsigned long n );
unsigned long factorial_for( unsigned long n )
{
    unsigned long x = n;
    
    if( n == 0 )
    {
        return 1;
    }
    
    for( ; n > 1 ; )
    {
        x = --n * x;
    }
    
    return x;
}

int main( void )
{
    unsigned long i;
    unsigned long a;
    unsigned long b;
    unsigned long c;
    unsigned long d;
    long double a_time;
    long double b_time;
    long double c_time;
    long double d_time;
    long double t1;
    long double t2;
    
    get_time();
    
    a_time = 0;
    b_time = 0;
    c_time = 0;
    d_time = 0;
    
    for( i = 0; i < 10000000; i++ )
    {    
        t1     = get_time();
        a      = factorial_recursive( 50 );
        t2     = get_time();
        a_time += ( t2 - t1 );
    }
    
    for( i = 0; i < 10000000; i++ )
    {
        t1     = get_time();
        b      = factorial_while( 50 );
        t2     = get_time();
        b_time += ( t2 - t1 );
    }
    
    for( i = 0; i < 10000000; i++ )
    {
        t1     = get_time();
        c      = factorial_asm( 50 );
        t2     = get_time();
        c_time += ( t2 - t1 );
    }
    
    for( i = 0; i < 10000000; i++ )
    {
        t1     = get_time();
        d      = factorial_for( 50 );
        t2     = get_time();
        d_time += ( t2 - t1 );
    }
    
    printf
    (
        "\n"
        "---------------\n"
        "Evaluating 50!:\n"
        "---------------\n"
        "Recursive: %lu - %.20Lf\n"
        "While:     %lu - %.20Lf\n"
        "Assembler: %lu - %.20Lf\n"
        "For:       %lu - %.20Lf\n"
        "\n",
        a,
        a_time,
        b,
        b_time,
        c,
        c_time,
        d,
        d_time
    );
    
    return EXIT_SUCCESS;
}
