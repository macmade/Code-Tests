/*******************************************************************************
 * Copyright (c) 2010, Jean-David Gadina <macmade@eosgarden.com>
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
#include <stdlib.h>

#define MAX_LEN 16
#define BUF_LEN 64

typedef struct _symbol
{
    uint16_t     code;
    unsigned int bits;
    char         c;
}
symbol;

unsigned int read( char buf[] );
unsigned int read( char buf[] )
{
    static int i = 0;
    
    if( i > 2 )
    {
        return 0;
    }
    else if( i == 0 )
    {
        buf[ 0 ]  = 1;
        buf[ 1 ]  = 0;
        buf[ 2 ]  = 2;
        buf[ 3 ]  = 0;
        buf[ 4 ]  = 0;
        buf[ 5 ]  = 2;
        buf[ 6 ]  = 0;
        buf[ 7 ]  = 1;
        buf[ 8 ]  = 0;
        buf[ 9 ]  = 1;
        buf[ 10 ] = 1;
        buf[ 11 ] = 1;
        buf[ 12 ] = 0;
        buf[ 13 ] = 1;
        buf[ 14 ] = 1;
        buf[ 15 ] = 1;
    }
    else if( i == 1 )
    {
        buf[ 0 ]  = 0;
        buf[ 1 ]  = 1;
        buf[ 2 ]  = 1;
        buf[ 3 ]  = 2;
        buf[ 4 ]  = 0;
        buf[ 5 ]  = 1;
        buf[ 6 ]  = 0;
        buf[ 7 ]  = 2;
        buf[ 8 ]  = 1;
        buf[ 9 ]  = 1;
        buf[ 10 ] = 0;
        buf[ 11 ] = 2;
        buf[ 12 ] = 0;
        buf[ 13 ] = 0;
        buf[ 14 ] = 0;
        buf[ 15 ] = 1;
    }
    else
    {
        buf[ 0 ] = 0;
        buf[ 1 ] = 1;
        
        i++;
        
        return 2;
    }
    
    i++;
    
    return 16;
}

int main( void )
{
    symbol ss[ 3 ] =
    {
        { 1, 1,  0 },
        { 2, 7,  1 },
        { 3, 16, 2 }
    };
    
    char         c;
    char         r_buf[ 16 ];
    char         s_buf[ MAX_LEN ];
    uint16_t     w_buf[ BUF_LEN ];
    uint16_t *   w;
    unsigned int n_bits;
    unsigned int length;
    unsigned int i;
    unsigned int j;
    symbol       s;
    
    printf( "\n" );
    
    j      = 0;
    n_bits = 0;
    w      = ( uint16_t * )( &( s_buf[ 0 ] ) );
    
    while( ( length = read( r_buf ) ) )
    {
        printf( "*** Getting data (%u)\n", length );
        
        i = 0;
        
        write_codes:
        
        for( ; i < length; i++ )
        {
            if( n_bits >= MAX_LEN )
            {
                break;
            }
            
            c = r_buf[ i ];
            s = ss[ ( int )c ];
            
            
            *( w ) |= s.code << ( ( MAX_LEN - n_bits ) - s.bits );
            
            printf(
                "        - Processing symbol: %u - %u"
                " / "
                "Buffer: %010u"
                " / "
                "Left shift: %03i\n", ( unsigned int )c, s.code, *( w ), ( MAX_LEN - n_bits ) - s.bits
            );
            
            n_bits += s.bits;
        }
        
        if( n_bits >= MAX_LEN )
        {
            n_bits     = n_bits - MAX_LEN;
            w_buf[ j ] = *( w );
            
            j++;
            
            printf( "--- Storing buffer: %u\n", *( w ) );
            printf( "        - Bits left: %u\n", n_bits );
            
            if( j == BUF_LEN / MAX_LEN )
            {
                printf( "*** Writing data\n" );
                
                for( j = 0; j < BUF_LEN; j++ )
                {
                    w_buf[ j ] = 0;
                }
                
                j = 0;
            }
            
            *( w ) = s.code << ( MAX_LEN - n_bits );
            
            
            printf( "        - New buffer: %u\n", *( w ) );
        }
        
        if( i < length )
        {
            goto write_codes;
        }
    }
    
    if( n_bits > 0 )
    {
        printf( "*** Writing remaining data\n" );
    }
    
    printf( "\n" );
    
    return EXIT_SUCCESS;
}
