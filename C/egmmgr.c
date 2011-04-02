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
#include <string.h>

#define EG_MAX_AR_POOLS 100
#define EG_NUM_OBJECTS  100

typedef struct _EGMemoryObject
{
    unsigned int retain_count;
    size_t       size;
    void *       data;
}
EGMemoryObject;

typedef struct _EGAutoreleasePool
{
    size_t  size;
    size_t  num_objects;
    void ** objects;
}
EGAutoreleasePool;

#define DEBUG           1
#define DLOG( ... )     if( DEBUG ) printf( " - DEBUG: " ); printf( __VA_ARGS__ ); printf( "\n" )
#define FATAL( ... )    fprintf( stderr, "Fatal error: " ); fprintf( stderr, __VA_ARGS__ );  fprintf( stderr, "\n" ); exit( EXIT_FAILURE )

EGAutoreleasePool * __EGGetCurrentAutoreleasePool( void );
void                __EGAutoreleasePoolDrain( EGAutoreleasePool * ap );
EGMemoryObject    * __EGGetMemoryObject( void * ptr );

EGAutoreleasePool * EGAutoreleasePoolCreate( void );
void                EGAutoreleasePoolDestroy( EGAutoreleasePool * ap );
void                EGAutoreleasePoolDrain( void );

void * EGAlloc( size_t size );
void * EGRealloc( void * ptr, size_t size );
void   EGRetain( void * ptr );
void   EGRelease( void * ptr );
void * EGAutoAlloc( size_t size );
void   EGAutorelease( void * ptr );
void * EGCopy( void * ptr );

static EGAutoreleasePool * __eg_ar_pools[ EG_MAX_AR_POOLS ];
static unsigned int        __eg_ar_pools_num;

EGAutoreleasePool * __EGGetCurrentAutoreleasePool( void )
{
    return __eg_ar_pools[ __eg_ar_pools_num - 1 ];
}

void __EGAutoreleasePoolDrain( EGAutoreleasePool * ap )
{
    size_t i;
    
    for( i = 0; i < ap->num_objects; i++ )
    {
        EGRelease( ap->objects[ i ] );
    }
    
    ap->num_objects = 0;
}

EGMemoryObject * __EGGetMemoryObject( void * ptr )
{
    EGMemoryObject * o;
    char           * c;
    
    c  = ( char * )ptr;
    c -= sizeof( EGMemoryObject * );
    c -= sizeof( void * );
    o  = ( EGMemoryObject * )c;
    
    return o;
}

EGAutoreleasePool * EGAutoreleasePoolCreate( void )
{
    EGAutoreleasePool * ap;
    
    if( __eg_ar_pools_num == EG_MAX_AR_POOLS )
    {
        FATAL( "Maximum number of auto-release pools reached: %u", EG_MAX_AR_POOLS );
    }
    
    if( NULL == ( ap = ( EGAutoreleasePool * )calloc( sizeof( EGAutoreleasePool ), 1 ) ) )
    {
        FATAL( "Unable to allocate memory for the auto-release pool" );
    }
    
    if( NULL == ( ap->objects = ( void ** )calloc( sizeof( void * ), EG_NUM_OBJECTS ) ) )
    {
        free( ap );
        FATAL( "Unable to allocate memory for the auto-release pool" );
    }
    
    ap->size                             = EG_NUM_OBJECTS;
    ap->num_objects                      = 0;
    __eg_ar_pools[ __eg_ar_pools_num++ ] = ap;
    
    DLOG( "Creating new auto-release pool:      %p", ( void * )ap );
    
    return ap;
}

void EGAutoreleasePoolDestroy( EGAutoreleasePool * ap )
{
    __EGAutoreleasePoolDrain( ap );
    
    DLOG( "Destroying auto-release pool:        %p", ( void * )ap );
    
    free( ap );
    
    __eg_ar_pools_num--;
}

void EGAutoreleasePoolDrain( void )
{
    EGAutoreleasePool * ap;
    
    ap = __EGGetCurrentAutoreleasePool();
    
    DLOG( "Draining current auto-release pool:  %p", ( void * )ap );
    
    __EGAutoreleasePoolDrain( ap );
}

void * EGAlloc( size_t size )
{
    EGMemoryObject * o;
    char           * ptr;
    
    o = calloc( sizeof( EGMemoryObject ) + size, 1 );
    
    if( o == NULL )
    {
        return NULL;
    }
    
    o->retain_count = 1;
    o->size         = size;
    
    ptr  =  ( char * )o;
    ptr += ( sizeof( EGMemoryObject ) );
    
    DLOG( "Allocating object:                   %p", ptr );
    
    return ptr;
}

void * EGRealloc( void * ptr, size_t size )
{
    EGMemoryObject * o;
    void           * data;
    
    o    = __EGGetMemoryObject( ptr );
    data = realloc( o->data, size );
    
    if( data == NULL )
    {
        return NULL;
    }
    
    o->data = data;
    o->size = size;
    
    return o->data;
}

void EGRetain( void * ptr )
{
    EGMemoryObject * o;
    
    o = __EGGetMemoryObject( ptr );
    
    DLOG( "Retaining object:                    %p", ptr );
    
    o->retain_count++;
}

void EGRelease( void * ptr )
{
    EGMemoryObject * o;
    
    o = __EGGetMemoryObject( ptr );
    
    DLOG( "Releasing object:                    %p", ptr );
    
    o->retain_count--;
    
    if( o->retain_count == 0 )
    {
        DLOG( "Freeing object:                      %p", ptr );
        free( o );
    }
}

void * EGAutoAlloc( size_t size )
{
    void * ptr;
    
    ptr = EGAlloc( size );
    
    EGAutorelease( ptr );
    
    return ptr;
}

void EGAutorelease( void * ptr )
{
    EGAutoreleasePool * ap;
    
    ap = __EGGetCurrentAutoreleasePool();
    
    if( ap->num_objects == ap->size )
    {
        ap->objects = ( void ** )realloc( ap->objects, sizeof( void * ) * ( ap->size + EG_NUM_OBJECTS ) );
        
        if( ap->objects == NULL )
        {
            FATAL( "Unable to allocate memory for the auto-release pool" );
        }
        
        ap->size += EG_NUM_OBJECTS;
    }
    
    ap->objects[ ap->num_objects++ ] = ptr;
}

void * EGCopy( void * ptr )
{
    EGMemoryObject * o;
    void           * ptr2;
    
    o    = __EGGetMemoryObject( ptr );
    ptr2 = EGAlloc( o->size );
    
    memcpy( ptr2, o->data, o->size );
    
    DLOG( "Copying object:                      %p", ( void * )ptr );
    
    return ptr2;
}

int main( void )
{
    unsigned int         i;
    char              ** test;
    EGAutoreleasePool  * ap1;
    EGAutoreleasePool  * ap2;
    
    ap1   = EGAutoreleasePoolCreate();
    test  = EGAutoAlloc( 5 * sizeof( char * ) );
    ap2   = EGAutoreleasePoolCreate();
    
    for( i = 0; i < 5; i++ )
    {
        test[ i ] = EGAutoAlloc( 2 );
    }
    
    EGAutoreleasePoolDrain();
    EGAutoreleasePoolDestroy( ap2 );
    EGAutoreleasePoolDestroy( ap1 );
    
    return EXIT_SUCCESS;
}
