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

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <limits.h>
#import <execinfo.h>
#import <signal.h>

#define ENABLE_ZOMBIES    1
#define ZOMBIES_SIGTRAP   1

static const char * const __assocKeyClass = "ZombieAssocKeyClass";
static const char * const __assocKeyBT    = "ZombieAssocKeyBT";

static char * __backtrace();
static char * __backtrace()
{
    int              i;
    int              num;
    void          ** trace;
    char          ** symbols;
    char           * bt;
    unsigned int     pad;
    
    trace = ( void ** )malloc( 100 * sizeof( void * ) );
    
    if( trace == NULL )
    {
        return "???";
    }
    
    num     = backtrace( trace, 100 );
    symbols = backtrace_symbols( trace, num );
    
    if( symbols == NULL )
    {
        free( trace );
        
        return "???";
    }
    
    bt = NULL;
    
    for( i = 2; i < num; i++ )
    {
        pad = ( i == 2 ) ? 0 : 16;
        bt  = ( bt == NULL ) ? ( char * )calloc( sizeof( char ), strlen( symbols[ i ] ) + 2 + pad ) : ( char * )realloc( bt, strlen( bt ) + strlen( symbols[ i ]  ) + 2 + pad );
        
        if( bt == NULL )
        {
            break;
        }
        
        if( i == 2 )
        {
            strcat( bt, symbols[ i ] );
        }
        else
        {
            strcat( bt, "                " );
            strcat( bt, symbols[ i ] );
        }
        
        strcat( bt, "\n" );
    }
    
    return ( bt == NULL ) ? "???" : bt;
}

@interface Zombie < NSObject >
{}

@end

@interface Zombie( Private )

- ( void )logCall: ( SEL )sel;

@end

@implementation Zombie( Private )

- ( void )logCall: ( SEL )sel
{
    Class  cls;
    char * bt1;
    char * bt2;
    
    cls = objc_getAssociatedObject( self, __assocKeyClass );
    bt1 = ( char * )objc_getAssociatedObject( self, __assocKeyBT );
    bt2 = __backtrace();
    
    NSLog
    (
        @"DEBUG - [ %@ %@ ]: message sent to a deallocated instance\n"
        @"\n"
        @"Backtrace:      %s\n"
        @"Deallocated in: %s\n",
        NSStringFromClass( cls ),
        NSStringFromSelector( sel ),
        ( bt2 != nil ) ? bt2 : "???",
        ( bt1 != nil ) ? bt1 : "???"
    );
    
    #if defined( ZOMBIES_SIGTRAP ) && ZOMBIES_SIGTRAP == 1
    
    raise( SIGTRAP );
    
    #endif
}

@end

@implementation Zombie

- ( Class )class
{
    Class cls;
    
    cls = objc_getAssociatedObject( self, __assocKeyClass );
    
    if( cls != NULL )
    {
        [ self logCall: _cmd ];
        
        return cls;
    }
    
    return objc_getClass( "Zombie" );
}

- ( Class )superclass
{
    Class cls;
    
    cls = objc_getAssociatedObject( self, __assocKeyClass );
    
    if( cls != NULL )
    {
        [ self logCall: _cmd ];
        
        return class_getSuperclass( cls );
    }
    
    return NULL;
}

- ( BOOL )isEqual: ( id )object
{
    [ self logCall: _cmd ];
    
    return ( BOOL )( self == object );
}

- ( NSUInteger )hash
{
    [ self logCall: _cmd ];
    
    return ( NSUInteger )self;
}

- ( id )self
{
    [ self logCall: _cmd ];
    
    return self;
}

- ( id )retain
{
    [ self logCall: _cmd ];
    
    return self;
}

- ( oneway void )release
{
    [ self logCall: _cmd ];
}

- ( id )autorelease
{
    [ self logCall: _cmd ];
    
    return self;
}

- ( NSUInteger )retainCount
{
    [ self logCall: _cmd ];
    
    return UINT_MAX;
}

- ( BOOL )isKindOfClass: ( Class )cls
{
    ( void )cls;
    
    [ self logCall: _cmd ];
    
    return NO;
}

- ( BOOL )isMemberOfClass: ( Class )cls
{
    ( void )cls;
    
    [ self logCall: _cmd ];
    
    return NO;
}

- ( BOOL )respondsToSelector: ( SEL )sel
{
    ( void )sel;
    
    [ self logCall: _cmd ];
    
    return NO;
}

- ( BOOL )conformsToProtocol: ( Protocol * )protocol
{
    ( void )protocol;
    
    [ self logCall: _cmd ];
    
    return NO;
}

- ( NSString * )description
{
    [ self logCall: _cmd ];
    
    return nil;
}

- ( id )performSelector: ( SEL )sel
{
    ( void )sel;
    
    [ self logCall: _cmd ];
    
    return nil;
}

- ( id )performSelector: ( SEL )sel withObject: ( id )obj
{
    ( void )sel;
    ( void )obj;
    
    [ self logCall: _cmd ];
    
    return nil;
}

- ( id )performSelector: ( SEL )sel withObject: ( id )obj1 withObject: ( id )obj2
{
    ( void )sel;
    ( void )obj1;
    ( void )obj2;
    
    [ self logCall: _cmd ];
    
    return nil;
}

- ( NSZone * )zone
{
    [ self logCall: _cmd ];
    
    return nil;
}

- ( BOOL )isProxy
{
    [ self logCall: _cmd ];
    
    return NO;
}

- ( void )doesNotRecognizeSelector: ( SEL )sel
{
    ( void )sel;
}

- ( NSMethodSignature * )methodSignatureForSelector: ( SEL )sel
{
    Class cls;
    
    cls = objc_getAssociatedObject( self, __assocKeyClass );
    
    return [ cls instanceMethodSignatureForSelector: sel ];
}

- ( void )forwardInvocation: ( NSInvocation * )invocation
{
    [ self logCall: [ invocation selector ] ];
}

@end

#if defined( ENABLE_ZOMBIES ) && ENABLE_ZOMBIES == 1

static void __init( void ) __attribute__( ( constructor ) );
static void __init( void )
{
    
    Class  cls;
    Method m1;
    Method m2;
    
    cls = [ NSObject class ];
    m1  = class_getInstanceMethod( cls, @selector( dealloc ) );
    m2  = class_getInstanceMethod( cls, @selector( zombie_dealloc ) );
    
    method_exchangeImplementations( m1, m2 );
}

#endif

@interface NSObject( Zombie )

- ( void )zombie_dealloc;

@end

@implementation NSObject( Zombie )

- ( void )zombie_dealloc
{
    objc_setAssociatedObject( self, __assocKeyClass, self->isa, OBJC_ASSOCIATION_ASSIGN );
    objc_setAssociatedObject( self, __assocKeyBT, ( id )( __backtrace() ), OBJC_ASSOCIATION_ASSIGN );
    
    self->isa = [ Zombie class ];
}

@end

int main( void )
{
    NSMutableArray * a;
    
    @autoreleasepool
    {
        a = [ [ NSMutableArray alloc ] initWithCapacity: 10 ];
        
        [ a release ];
        [ a release ];
        [ a lastObject ];
    }
    
    return EXIT_SUCCESS;
}
