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

#import <Cocoa/Cocoa.h>
#import <sys/stat.h>

BOOL checkDirectoryReadAccess( NSString * path );
BOOL checkDirectoryReadAccess( NSString * path )
{
    unsigned int            uid;
    unsigned int            gid;
    struct stat             sb;
    BOOL                    isDir;
    NSFileManager         * fm;
    NSDirectoryEnumerator * dir;
    NSString              * file;
    NSString              * filePath;
    
    fm = [ NSFileManager defaultManager ];
    
    if( [ fm fileExistsAtPath: path isDirectory: &isDir ] == NO || isDir == NO )
    {
        return NO;
    }
    
    dir = [ fm enumeratorAtPath: path ];
    uid = getuid();
    gid = getgid();
    
    if( [ [ path substringFromIndex: [ path length ] - 1 ] isEqualToString: @"/" ] == NO )
    {
        path = [ path stringByAppendingString: @"/" ];
    }
    
    for( file in dir )
    {
        filePath = [ NSString stringWithFormat: @"%@%@", path, file ];
        
        lstat( [ filePath cStringUsingEncoding: NSUTF8StringEncoding ], &sb );
        
        if( sb.st_uid != uid )
        {
            if( sb.st_gid != gid && !( sb.st_mode & S_IROTH ) )
            {
                return NO;
            }
            else if( !( sb.st_mode & S_IRGRP ) )
            {
                return NO;
            }
        }
        else if( !( sb.st_mode & S_IRUSR ) )
        {
            return NO;
        }
    }
    
    return YES;
}

int main( void )
{
    NSAutoreleasePool * ap;
    
    ap = [ NSAutoreleasePool new ];
    
    if( checkDirectoryReadAccess( @"/Users/macmade/Desktop" ) == YES )
    {
        NSLog( @"Readable" );
    }
    else
    {
        NSLog( @"Not readable" );
    }
    
    [ ap release ];
    
    return EXIT_SUCCESS;
}
