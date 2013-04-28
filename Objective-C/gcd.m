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

/* #define NSTHREAD_USES_GCD   1 */

@implementation NSThread( GCD )

#if defined( NSTHREAD_USES_GCD ) && NSTHREAD_USES_GCD == 1

+ ( void )detachNewThreadSelector: ( SEL )selector toTarget: ( id )target withObject: ( id )argument
{
    dispatch_async
    (
        dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 ),
        ^
        {
            [ target performSelector: selector withObject: argument ];
        }
    );
}

#endif

@end

@interface Foo: NSObject
{
@private
    
    id _Test_Reserved[ 5 ];
}

- ( void )log: ( NSString * )str;

@end

@implementation Foo

- ( void )log: ( NSString * )str
{
    printf( "%s ", [ str UTF8String ] );
    fflush( stdout );
}

@end

int main( void )
{
    NSAutoreleasePool * ap;
    Foo               * foo;
    NSUInteger          i;
    
    ap  = [ NSAutoreleasePool new ];
    foo = [ Foo new ];
    
    for( i = 33; i < 256; i++ )
    {
        [ NSThread detachNewThreadSelector: @selector( log: ) toTarget: foo withObject: [ NSString stringWithFormat: @"%c", i ] ];
    }
    
    [ foo release ];
    [ ap release ];
    
    return EXIT_SUCCESS;
}
