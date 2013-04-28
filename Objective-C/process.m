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

#import <Cocoa/Cocoa.h>

@interface Process: NSObject
{
    NSUInteger pid;
    NSString * name;
    NSString * path;
}

@property( readonly ) NSUInteger pid;
@property( readonly ) NSString * name;
@property( readonly ) NSString * path;

+ ( Process * )processWithPath: ( NSString * )path;

@end

@interface Process( Private )

+ ( NSString * )processInfosByPath: ( NSString * )path;

@end

@implementation Process( Private )

+ ( NSString * )processInfosByPath: ( NSString * )path
{
    NSTask       * task;
    NSPipe       * outPipe;
    NSFileHandle * output;
    NSData       * data;
    NSString     * ps;
    NSArray      * lines;
    NSString     * line;
    
    outPipe = [ NSPipe pipe ];
    task    = [ NSTask new ];
    output  = [ outPipe fileHandleForReading ];
    
    [ task setLaunchPath: @"/bin/ps" ];
    [ task setArguments: [ NSArray arrayWithObjects: @"-Arx", nil ] ];
    [ task setStandardOutput: outPipe ];
    [ task launch ];
    
    while( [ task isRunning ] )
    {}
    
    data  = [ output readDataToEndOfFile ];
    ps    = [ [ NSString alloc ] initWithData: data encoding: NSASCIIStringEncoding ];
    lines = [ ps componentsSeparatedByString: @"\n" ];
    
    for( line in lines )
    {
        @try
        {
            if( [ [ [ line substringFromIndex: 25 ] substringToIndex: [ path length ] ] isEqualToString: path ] )
            {
                return line;
            }
        }
        @catch( NSException * e )
        {
            continue;
        }
    }
    
    return nil;
}

@end

@implementation Process

@synthesize pid;
@synthesize name;
@synthesize path;

+ ( Process * )processWithPath: ( NSString * )path
{
    Process * process;
    NSString  * processInfos;
    
    processInfos = [ self processInfosByPath: path ];
    
    if( processInfos == nil )
    {
        return nil;
    }
    
    process       = [ Process new ];
    process->pid  = [ [ processInfos substringToIndex: 5 ] integerValue ];
    process->name = [ [ path lastPathComponent ] copy ];
    process->path = [ path copy ];
    
    return process;
}

- ( NSString * )description
{
    NSString * desc;
    
    desc = [ super description ];
    
    return [ NSString stringWithFormat: @"%@ - %@ (%u) - %@", desc, name, pid, path ];
}

- ( void )dealloc
{
    [ name release ];
    [ path release ];
    
    [ super dealloc ];
}

@end

int main( void )
{
    Process         * process;
    NSAutoreleasePool * ap;
    
    ap      = [ NSAutoreleasePool new ];
    process = [ Process processWithPath: @"/usr/local/webstart/sw/inadyn/bin/inadyn" ];
    
    NSLog( @"%@", process );
    
    [ ap release ];
    
    return 0;
}
