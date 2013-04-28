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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>
#include <Cocoa/Cocoa.h>

@interface IdleTime: NSObject
{
@protected
    
    mach_port_t   ioPort;
    io_iterator_t ioIterator;
    io_object_t   ioObject;
    
@private
    
    id r1;
    id r2;
}

@property( readonly ) uint64_t timeIdle;
@property( readonly ) NSUInteger secondsIdle;

@end

@implementation IdleTime

- ( id )init
{
    kern_return_t status;
    
    if( ( self = [ super init ] ) )
    {
        status = IOMasterPort( MACH_PORT_NULL, &ioPort );
        
        if( status != KERN_SUCCESS )
        {
            @throw [ NSException
                        exceptionWithName:  @"IdleTimeIOKitError"
                        reason:             @"Error communicating with IOKit"
                        userInfo:           nil
                   ];
        }
        
        status = IOServiceGetMatchingServices
        (
            ioPort,
            IOServiceMatching( "IOHIDSystem" ),
            &ioIterator
        );
        
        if( status != KERN_SUCCESS )
        {
            @throw [ NSException
                        exceptionWithName:  @"IdleTimeIOHIDError"
                        reason:             @"Error accessing IOHIDSystem"
                        userInfo:           nil
                   ];
        }
        
        ioObject = IOIteratorNext( ioIterator );
        
        if ( ioObject == 0 )
        {
            IOObjectRelease( ioIterator );
            
            @throw [ NSException
                        exceptionWithName:  @"IdleTimeIteratorError"
                        reason:             @"Invalid iterator"
                        userInfo:           nil
                   ];
        }
        
        IOObjectRetain( ioObject );
        IOObjectRetain( ioIterator );
    }
    
    return self;
}

- ( void )dealloc
{
    IOObjectRelease( ioObject );
    IOObjectRelease( ioIterator );
    
    [ super dealloc ];
}

- ( uint64_t )timeIdle
{
    kern_return_t          status;
    CFTypeRef              idle;
    CFTypeID               type;
    uint64_t               t;
    CFMutableDictionaryRef properties;
    
    properties = NULL;

    status = IORegistryEntryCreateCFProperties
    (
        ioObject,
        &properties,
        kCFAllocatorDefault,
        0
    );
    
    if( status != KERN_SUCCESS || properties == NULL )
    {
        @throw [ NSException
                    exceptionWithName:  @"IdleTimeSystemPropError"
                    reason:             @"Cannot get system properties"
                    userInfo:           nil
               ];
    }
    
    idle = CFDictionaryGetValue( properties, CFSTR( "HIDIdleTime" ) );
    
    if( !idle )
    {
        CFRelease( ( CFTypeRef )properties );
        
        @throw [ NSException
                    exceptionWithName:  @"IdleTimeSystemTimeError"
                    reason:             @"Cannot get system idle time"
                    userInfo:           nil
               ];
    }
    
    CFRetain( idle );
    
    type = CFGetTypeID( idle );
    
    if( type == CFDataGetTypeID() )
    {
        CFDataGetBytes
        (
            ( CFDataRef )idle,
            CFRangeMake( 0, sizeof( t ) ),
            ( UInt8 * )&t
        );
    }
    else if( type == CFNumberGetTypeID() )
    {
        CFNumberGetValue
        (
            ( CFNumberRef )idle,
            kCFNumberSInt64Type,
            &t
        );
    }
    else
    {
        CFRelease( idle );
        CFRelease( ( CFTypeRef )properties );
        
        @throw [ NSException
                    exceptionWithName:  @"IdleTimeTypeError"
                    reason:             [ NSString stringWithFormat: @"Unsupported type: %d\n", ( int )type ]
                    userInfo:           nil
               ];
    }
    
    CFRelease( idle );
    CFRelease( ( CFTypeRef )properties );
    
    return t;
}

- ( NSUInteger )secondsIdle
{
    uint64_t t;
    
    t = self.timeIdle;
    
    return ( NSUInteger )( t >> 30 );
}

@end

@interface Test: NSObject
{
@protected
    
    NSTimer  * timer;
    IdleTime * idle;
    
@private
    
    id r1;
    id r2;
}

@end

@implementation Test

- ( void )printIdleTime: ( NSTimer * )timerObject
{
    ( void )timerObject;
    NSLog( @"Idle time in seconds: %u", ( unsigned int )idle.secondsIdle );
}

- ( id )init
{
    if( ( self = [ super init ] ) )
    {
        idle  = [ [ IdleTime alloc ] init ];
        timer = [ NSTimer
                    scheduledTimerWithTimeInterval: 1
                    target:                         self
                    selector:                       @selector( printIdleTime: )
                    userInfo:                       NULL
                    repeats:                        YES
                ];
    }
    
    return self;
}

- ( void )dealloc
{
    [ timer invalidate ];
    [ idle release ];
    [ super dealloc ];
}

@end

BOOL shouldKeepRunning = YES;

int main( int argc, char * argv[] )
{
    NSRunLoop         * loop;
    NSAutoreleasePool * pool;
    Test              * test;
    
    ( void )argc;
    ( void )argv;
    
    loop = [ NSRunLoop currentRunLoop ];
    pool = [ [ NSAutoreleasePool alloc ] init ];
    test = [ [ Test alloc ] init ];
    
    while( shouldKeepRunning && [ loop runMode: NSDefaultRunLoopMode beforeDate: [ NSDate distantFuture ] ] );
    
    [ test release ];
    [ pool release ];
    
    return EXIT_SUCCESS;
}
