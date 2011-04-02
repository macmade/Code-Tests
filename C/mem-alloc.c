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
#include <unistd.h>

#ifndef __bool_true_false_are_defined
    #define bool                            _Bool
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

typedef unsigned long int mem_fence;

#define MEM_FENCE_VAL       0x3CC3L
#define MEM_FENCE_VAL_SIZE  sizeof( mem_fence )

struct mem_block_hdr
{
    mem_fence   fence1;
    bool        available;
    size_t      size;
    mem_fence   fence2;
};

static void       * MEM_START_ADDR;
static void       * MEM_END_ADDR;
static bool         MEM_INIT = false;

void   mem_init( void );
void   mem_free( void * ptr );
void * mem_alloc( size_t size );

#define malloc( x )     mem_alloc( x )
#define free( x )       mem_free( x )

int main( void )
{
    char       * data1;
    char       * data2;
    unsigned int x;
    unsigned int i;
    
    x = 256;
    
    data1 = malloc( x * sizeof( char ) );
    data2 = malloc( x * sizeof( char ) );
    
    free( data1 );
    
    data1 = malloc( x * sizeof( char ) );
    
    free( data1 );
    
    for( i = 0; i < x; i++ )
    {
        data2[ i ] = ( i % 93 ) + 33;
    }
    
    printf( "----------\n" );
    
    for( i = 0; i < x; i++ )
    {
        printf( "%c", data2[ i ] );
    }
    
    printf( "\n" );
    
    printf( "----------\n" );
    
    free( data2 );
    
    return EXIT_SUCCESS;
}

void mem_init( void )
{
    MEM_END_ADDR   = sbrk( 0 );
    MEM_START_ADDR = MEM_END_ADDR;
    MEM_INIT       = true;
    
    printf( "*** INIT\n" );
}

void mem_free( void * ptr )
{
    struct mem_block_hdr * hdr1;
    struct mem_block_hdr * hdr2;
    
    hdr1 = ( struct mem_block_hdr * )( ( char * )ptr - sizeof( struct mem_block_hdr ) );
    hdr2 = ( struct mem_block_hdr * )( ( char * )ptr + hdr1->size );
    
    hdr1->available = true;
    hdr2->available = true;
    
    printf( "*** FREE:  %i - %p\n", ( unsigned int )hdr1->size, ptr );
    
    return;
}

void * mem_alloc( size_t size )
{
    size_t                 allocSize;
    void                 * addr;
    void                 * mem;
    struct mem_block_hdr * hdr1;
    struct mem_block_hdr * hdr2;
    
    if( MEM_INIT == false )
    {
        mem_init();
    }
    
    allocSize = size + ( 2 * sizeof( struct mem_block_hdr ) );
    mem       = 0;
    addr      = MEM_START_ADDR;
    
    while( addr != MEM_END_ADDR )
    {
        hdr1 = ( struct mem_block_hdr * )addr;
        
        if( hdr1->available == true )
        {
            if( hdr1->size >= size )
            {
                hdr2             = ( struct mem_block_hdr * )( ( char * )addr + size + sizeof( struct mem_block_hdr ) );
                hdr1->available  = false;
                hdr2->available  = false;
                mem              = addr;
                
                break;
            }
            
            if( hdr1->size > size )
            {
                
            }
        }
        
        addr = ( char * )addr + allocSize;
    }
    
    if( !mem )
    {
        sbrk( allocSize );
        
        printf( "*** SBRK\n" );
        
        mem                = MEM_END_ADDR;
        MEM_END_ADDR       = ( char * )MEM_END_ADDR + allocSize;
        hdr1               = ( struct mem_block_hdr * )mem;
        hdr2               = ( struct mem_block_hdr * )( ( char * )mem + hdr1->size + sizeof( struct mem_block_hdr ) );
        hdr1->available    = false;
        hdr2->available    = false;
        hdr1->size         = size;
        hdr2->size         = size;
        hdr1->fence1       = MEM_FENCE_VAL;
        hdr2->fence1       = MEM_FENCE_VAL;
        hdr1->fence2       = MEM_FENCE_VAL;
        hdr2->fence2       = MEM_FENCE_VAL;
    }
    
    mem = ( char * )mem + sizeof( struct mem_block_hdr );
    
    printf( "*** ALLOC: %i - %p\n", ( unsigned int )size, mem );
    
    return mem;
}
