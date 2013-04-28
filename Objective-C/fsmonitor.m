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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

static void __printKEvent( struct kevent * e )
{
    char                tmp[ 512 ];
    char                text[ 512 ];
    static const char * filters[ 9 ];
    
    filters[ 0 ] = "EVFILT_UNKNOWN";    /*  0 */
    filters[ 1 ] = "EVFILT_READ";       /* -1 */
    filters[ 2 ] = "EVFILT_WRITE";      /* -2 */
    filters[ 3 ] = "EVFILT_AIO";        /* -3 */
    filters[ 4 ] = "EVFILT_VNODE";      /* -4 */
    filters[ 5 ] = "EVFILT_PROC";       /* -5 */
    filters[ 6 ] = "EVFILT_SIGNAL";     /* -6 */
    filters[ 7 ] = "EVFILT_TIMER";      /* -7 */
    filters[ 8 ] = "EVFILT_MACHPORT";   /* -8 */
    
    memset( tmp, 0, 512 );
    memset( text, 0, 512 );
    
    sprintf
    (
        text,
        "    Identifier: %p\n"
        "    Filter:     %s (%hi)\n",
        ( void * )( e->ident ),
        filters[ -e->filter ],
        e->filter
    );
    
    if ( e->flags == 0 )
    {
        sprintf( tmp, "    Flags:      0x0000\n" );
        strcat( text, tmp );
    }
    else
    {
        sprintf
        (
            tmp,
            "    Flags:      0x%04x (%s%s%s%s%s%s%s%s)\n",
            e->flags,
            ( e->flags & EV_EOF     ) ?                                                            "EV_EOF"                      : "",
            ( e->flags & EV_ERROR   ) ? ( ( e->flags & ( ~( EV_ERROR - 1 ) & ~EV_ERROR ) )    ? " | EV_ERROR"   : "EV_ERROR"   ) : "",
            ( e->flags & EV_ADD     ) ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_ADD     - 1 ) ) ) ? " | EV_ADD"     : "EV_ADD"     ) : "",
            ( e->flags & EV_DELETE  ) ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_DELETE  - 1 ) ) ) ? " | EV_DELETE"  : "EV_DELETE"  ) : "",
            ( e->flags & EV_ENABLE  ) ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_ENABLE  - 1 ) ) ) ? " | EV_ENABLE"  : "EV_ENABLE"  ) : "",
            ( e->flags & EV_DISABLE ) ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_DISABLE - 1 ) ) ) ? " | EV_DISABLE" : "EV_DISABLE" ) : "",
            ( e->flags & EV_ONESHOT ) ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_ONESHOT - 1 ) ) ) ? " | EV_ONESHOT" : "EV_ONESHOT" ) : "",
            ( e->flags & EV_CLEAR )   ? ( ( e->flags & ( EV_SYSFLAGS | ( EV_CLEAR   - 1 ) ) ) ? " | EV_CLEAR"   : "EV_CLEAR"   )  : ""
        );
        
        strcat( text, tmp );
    }
    
    sprintf
    (
        tmp,
        "    FFlags:     0x%08x (%s%s%s%s%s%s%s)\n",
        e->fflags,
        ( e->fflags & NOTE_DELETE ) ?                                            "NOTE_DELETE"                   : "",
        ( e->fflags & NOTE_WRITE  ) ? ( ( e->fflags & ( NOTE_WRITE  - 1 ) ) ? " | NOTE_WRITE"  : "NOTE_WRITE"  ) : "",
        ( e->fflags & NOTE_EXTEND ) ? ( ( e->fflags & ( NOTE_EXTEND - 1 ) ) ? " | NOTE_EXTEND" : "NOTE_EXTEND" ) : "",
        ( e->fflags & NOTE_ATTRIB ) ? ( ( e->fflags & ( NOTE_ATTRIB - 1 ) ) ? " | NOTE_ATTRIB" : "NOTE_ATTRIB" ) : "",
        ( e->fflags & NOTE_LINK   ) ? ( ( e->fflags & ( NOTE_LINK   - 1 ) ) ? " | NOTE_LINK"   : "NOTE_LINK"   ) : "",
        ( e->fflags & NOTE_RENAME ) ? ( ( e->fflags & ( NOTE_RENAME - 1 ) ) ? " | NOTE_RENAME" : "NOTE_RENAME" ) : "",
        ( e->fflags & NOTE_REVOKE ) ? ( ( e->fflags & ( NOTE_REVOKE - 1 ) ) ? " | NOTE_REVOKE" : "NOTE_REVOKE" ) : ""
    );
    
    strcat( text, tmp );
    
    sprintf
    (
        tmp,
        "    Data:       %p (%s)\n"
        "    UData:      %p (%s)\n",
        ( void * )( e->data ),
        ( char * )( e->data ),
        e->udata,
        ( char * )( e->udata )
    );
    
    strcat( text, tmp );
    printf( "%s", text );
}

int main( int argc, char * argv[] )
{
    int             queue;
    int             fd;
    int             n;
    char          * path;
    struct timespec timeout;
    struct kevent   events[ 1 ];
    struct kevent   data[ 1 ];
    
    if( argc < 2 )
    {
        fprintf( stderr, "Usage: %s PATH\n", argv[ 0 ] );
        
        exit( EXIT_FAILURE );
    }
    
    path = argv[ 1 ];
    
    if( ( queue = kqueue() ) < 0 )
    {
        fprintf( stderr, "Could not open kernel queue. Error %i - %s\n", errno, strerror( errno ) );
        
        exit( EXIT_FAILURE );
    }
    
    if( ( fd = open( path, O_EVTONLY ) ) <= 0 )
    {
        fprintf( stderr, "The file %s could not be opened for monitoring. Error %i - %s.\n", path, errno, strerror( errno ) );
        
        exit( EXIT_FAILURE );
    }
    
    EV_SET
    (
        &events[ 0 ],
        fd,
        EVFILT_VNODE,
        EV_ADD | EV_CLEAR,
        NOTE_DELETE | NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE,
        0,
        ( void * )path
    );
    
    while( 1 )
    {    
        timeout.tv_sec  = 1;
        timeout.tv_nsec = 000000000;
        n               = kevent( queue, events, 1, data, 1, &timeout );
        
        if( n < 0 || data[ 0 ].flags == EV_ERROR )
        {
            fprintf( stderr, "An error occurred (event count %i).  Error %i -  %s.\n", n, errno, strerror( errno ) );
            break;
        }
        
        if( n > 0 )
        {
            printf( "Event received:\n" );
            __printKEvent( data );
        }
        else
        {
            printf( "Waiting for event...\n" );
        }
    }
    
    close( fd );
    
    return EXIT_SUCCESS;
}
