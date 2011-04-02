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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void quicksort( int * values, int left, int right );
void quicksort( int * values, int left, int right )
{
    int i;
    int j;
    int x;
    int y;
    
    i = left;
    j = right;
    x = values[ ( left + right ) / 2 ];
    
    do
    {
        while( ( values[ i ] < x ) && ( i < right ) )
        {
           i++;
        }
        while( ( x < values[ j ] ) && ( j > left ) )
        {
            j--;
        }
        
        if( i <= j )
        {
            y           = values[ i ];
            values[ i ] = values[ j ];
            values[ j ] = y;
            
            i++;
            j--;
        }
    }
    while( i <= j );
    
    if( i < right )
    {
        quicksort( values, i, right );
    }
    
    if( left < j )
    {
        quicksort( values, left, j );
    }
}


int main( void )
{
    int a[ 10 ];
    int i;
    
    printf( "\nUnsorted:\n" );
    
    for( i = 0; i < 10; i++ )
    {
        a[ i ] = rand();
        
        printf( "%i ", a[ i ] );
    }
    
    printf( "\n\nSorted:\n" );
    
    quicksort( a, 0, 9 );
    
    for( i = 0; i < 10; i++ )
    {
        printf( "%i ", a[ i ] );
    }
    
    printf( "\n\n" );
    
    return EXIT_SUCCESS;
}
