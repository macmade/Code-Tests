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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int tcp_server_create( unsigned int port, unsigned int max_clients );
void tcp_connection_process( int socket );

int main( void )
{
    int socket_id;
    int client_id;
    int child_pid;
    
    socket_id = tcp_server_create( 2000, 1 );
    
    printf( "Parent process: %i\n", getpid() );
    
    while( 1 )
    {
        if( -1 == ( client_id = accept( socket_id, NULL, 0 ) ) )
        {
            fprintf( stderr, "Accept error\n" );
            exit( EXIT_FAILURE );
        }
        
        child_pid = fork();
        
        if( child_pid == -1 )
        {
            fprintf( stderr, "Fork error\n" );
            exit( EXIT_FAILURE );
        }
        else if( child_pid == 0 )
        {
            tcp_connection_process( client_id );
        }
        else
        {
            printf( "Child process forked: %i\n", child_pid );
            close( client_id );
        }
    }
    
    shutdown( socket_id, 2 );
    close( socket_id );
    
    return EXIT_SUCCESS;
}

void tcp_connection_process( int sock )
{
    FILE         * channel;
    fd_set         read_fd;
    struct timeval timeout;
    int            result;
    int            length;
    char           buffer[ 8192 ];
    
    char greetings[] = "Hello Dave, you're looking well today...\n";
    channel          = fdopen( sock, "r+" );
    
    memset( ( char * )&timeout, 0, sizeof( struct timeval ) );
    
    timeout.tv_sec = 5;
    
    fwrite( greetings, sizeof( char ), strlen( greetings ) + 1, channel );
    fflush( channel );
    
    while( 1 )
    {
        FD_ZERO( &read_fd );
        FD_SET( sock, &read_fd );
        
        result = select
        (
            sock + 1,
            &read_fd,
            NULL,
            NULL,
            &timeout
        );
        
        if( result == 0 )
        {
            fwrite( "Connection timed-out\n", sizeof( char ), 22, channel );
            
            break;
        }
        
        if( FD_ISSET( sock, &read_fd ) )
        {
            while( ( length = read( sock, buffer, sizeof( buffer ) - 1 ) ) )
            {
                if( length == 2 )
                {
                    fwrite( "End...\n", sizeof( char ), 8, channel );
                    fclose( channel );
                    
                    return;
                }
                
                fwrite( "Received data: ", sizeof( char ), 16, channel );
                fwrite( buffer, sizeof( char ), length + 1, channel );
                fflush( channel );
            }
        }
    }
    
    fclose( channel );
}

int tcp_server_create( unsigned int port, unsigned int max_clients )
{
    int result;
    unsigned int socket_id;
    unsigned int reuse_option_value;
    struct sockaddr_in sock_name;
    
    if( -1 == ( int )( socket_id = socket( PF_INET, SOCK_STREAM, 0 ) ) )
    {
        fprintf( stderr, "Cannot create the socket\n" );
        exit( EXIT_FAILURE );
    }
    
    setsockopt
    (
        socket_id,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse_option_value,
        sizeof( unsigned int )
    );
    
    memset( ( char * )&sock_name, 0, sizeof( struct sockaddr_in ) );
    
    sock_name.sin_family      = AF_INET;
    sock_name.sin_port        = htons( port );
    sock_name.sin_addr.s_addr = htonl( INADDR_ANY );
    
    result = bind
    (
        socket_id,
        ( struct sockaddr * )&sock_name,
        sizeof( struct sockaddr_in )
    );
    
    if( result == -1 )
    {
        fprintf( stderr, "Bind error\n" );
        exit( EXIT_FAILURE );
    }
    
    if( -1 == ( listen( socket_id, max_clients ) ) )
    {
        fprintf( stderr, "Listen error\n" );
        exit( EXIT_FAILURE );
    }
    
    return socket_id;
}
