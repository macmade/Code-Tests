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

@interface NSObject( Abstract )

- ( void )registerAbstractMethod: ( NSString * )name;

@end

@implementation NSObject( Abstract )

- ( void )registerAbstractMethod: ( NSString * )name
{
    Class         cls;
    SEL           selector;
    Method        m;
    NSException * e;
    
    selector = NSSelectorFromString( name );
    cls      = [ self class ];
    m        = class_getInstanceMethod(  cls, selector );
    
    if( m == NULL )
    {
        e = [ NSException exceptionWithName: @"AbstractClassException" reason: [ NSString stringWithFormat: @"Class %@ must implement abstract method %@", NSStringFromClass( cls ), name ] userInfo: nil ];
        
        @throw e;
    }
}

@end

@interface TestAbstractClass: NSObject
{
@protected
    
    
    
@private
    
    id _TestAbstractClass_Reserved[ 5 ];
}

@end

@implementation TestAbstractClass

- ( id )init
{
    if( ( self = [ super init ] ) )
    {
        [ self registerAbstractMethod: @"doSomething" ];
    }
    
    return self;
}

@end

@interface TestClass: TestAbstractClass
{
@protected
    
    
    
@private
    
    id _TestClass_Reserved[ 5 ];
}

@end

@implementation TestClass



@end

int main( void )
{
    TestClass * test;
    
    test = [ [ TestClass alloc ] init ];
    
    [ test release ];
    
    return 0;
}
