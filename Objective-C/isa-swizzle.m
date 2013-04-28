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

@interface NSObject( ISASwizzle )

- ( void )setClass: ( Class )cls;
- ( Class )originalClass;
- ( void )restoreClass;
- ( void )restoreOriginalClass;
- ( BOOL )hasCustomClass;

@end

static NSString * const __assocKey              = @"ISASwizzle_NSObject_SetClass";
static NSString * const __assocKeyOriginalClass = @"ISASwizzle_NSObject_OriginalClass";
static NSString * const __assocKeyClasses       = @"ISASwizzle_NSObject_Classes";

@interface NSObject( ISASwizzle_Private )

- ( void )ddna_dealloc;

@end

@implementation NSObject( ISASwizzle_Private )

- ( void )ddna_dealloc
{
    objc_setAssociatedObject( self, __assocKey, nil, OBJC_ASSOCIATION_ASSIGN );
    
    [ self ddna_dealloc ];
}

@end

static void __init( void ) __attribute__( ( constructor ) );
static void __init( void )
{
    Class  cls;
    Method m1;
    Method m2;
    
    cls = [ NSObject class ];
    m1  = class_getInstanceMethod( cls, @selector( dealloc ) );
    m2  = class_getInstanceMethod( cls, @selector( ddna_dealloc ) );
    
    method_exchangeImplementations( m1, m2 );
}

@implementation NSObject( ISASwizzle )

- ( void )setClass: ( Class )cls
{
    NSMutableDictionary * infos;
    NSMutableArray      * classes;
    Class                 originalClass;
    
    infos = objc_getAssociatedObject( self, __assocKey );
    
    if( infos == nil )
    {
        infos = [ NSMutableDictionary dictionaryWithObjectsAndKeys: [ self class ], __assocKeyOriginalClass, [ NSMutableArray arrayWithCapacity: 10 ], __assocKeyClasses, nil ];
        
        [ infos setObject: [ self class ] forKey: __assocKeyOriginalClass ];
        
        objc_setAssociatedObject( self, __assocKey, infos, OBJC_ASSOCIATION_RETAIN );
    }
    
    classes       = [ infos objectForKey: __assocKeyClasses ];
    originalClass = [ infos objectForKey: __assocKeyOriginalClass ];
    
    if( classes.count > 0 || self->isa != originalClass )
    {
        [ classes addObject: self->isa ];
    }
    
    self->isa = cls;
}

- ( Class )originalClass
{
    NSMutableDictionary * infos;
    Class                 originalClass;
    
    infos = objc_getAssociatedObject( self, __assocKey );
    
    if( infos == nil )
    {
        return self->isa;
    }
    
    originalClass = [ infos objectForKey: __assocKeyOriginalClass ];
    
    if( originalClass == nil )
    {
        return self->isa;
    }
    
    return originalClass;
}

- ( void )restoreClass
{
    NSMutableDictionary * infos;
    NSMutableArray      * classes;
    Class                 originalClass;
    
    infos = objc_getAssociatedObject( self, __assocKey );
    
    if( infos == nil )
    {
        return;
    }
    
    classes       = [ infos objectForKey: __assocKeyClasses ];
    originalClass = [ infos objectForKey: __assocKeyOriginalClass ];
    
    if( classes.count == 0 )
    {
        self->isa = originalClass;
    }
    else if( classes.count == 0 )
    {
        self->isa = originalClass;
    }
    else
    {
        self->isa = [ classes lastObject ];
        
        [ classes removeLastObject ];
    }
}

- ( void )restoreOriginalClass
{
    NSMutableDictionary * infos;
    NSMutableArray      * classes;
    Class                 originalClass;
    
    infos = objc_getAssociatedObject( self, __assocKey );
    
    if( infos == nil )
    {
        return;
    }
    
    classes       = [ infos objectForKey: __assocKeyClasses ];
    originalClass = [ infos objectForKey: __assocKeyOriginalClass ];
    
    [ classes removeAllObjects ];
    
    self->isa = originalClass;
}

- ( BOOL )hasCustomClass
{
    NSMutableDictionary * infos;
    NSMutableArray      * classes;
    Class                 originalClass;
    
    infos = objc_getAssociatedObject( self, __assocKey );
    
    if( infos == nil )
    {
        return NO;
    }
    
    classes       = [ infos objectForKey: __assocKeyClasses ];
    originalClass = [ infos objectForKey: __assocKeyOriginalClass ];
    
    if( classes.count > 0 && [ classes lastObject ] != originalClass )
    {
        return YES;
    }
    else if( self->isa != originalClass )
    {
        return YES;
    }
    
    return NO;
}

@end

/****************************************************************************
 * TESTING
 ****************************************************************************/

@interface Foo: NSObject
{}

@end

@interface Bar: NSObject
{}

@end

@interface FooBar: NSObject
{}

@end

@implementation Foo

@end

@implementation Bar

@end

@implementation FooBar

@end

int main( void )
{
    NSString * s;
    
    @autoreleasepool
    {
        s = @"hello, world";
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
        
        [ s setClass: [ Foo class ] ];
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
        
        [ s setClass: [ Bar class ] ];
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
        
        [ s setClass: [ FooBar class ] ];
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
        
        @try
        {
            NSLog( @"Trying to call a no longer valid selector..." );
            
            [ s length ];
        }
        @catch( NSException * e )
        {
            NSLog( @"Caught expected exception: %@ - %@", [ e name ], [ e reason ] );
        }
        
        [ s restoreClass ];
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
        
        [ s restoreOriginalClass ];
        
        NSLog( @"Object is now: %@", NSStringFromClass( [ s class ] ) );
    }
    
    return EXIT_SUCCESS;
}
