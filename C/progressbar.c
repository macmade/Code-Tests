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

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef __bool_true_false_are_defined
    #define bool                            _Bool
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

static unsigned int __libprogressbar_length = 75;

void * __libprogressbar_update_status( void * arg );
bool libprogressbar_create_progressbar( unsigned int * percent );
void libprogressbar_set_length( unsigned int length );

bool libprogressbar_create_progressbar( unsigned int * percent )
{
    pthread_t update;
    int       rc;
    
    rc = pthread_create( &update, NULL, __libprogressbar_update_status, ( void * )percent );
    
    return ( rc ) ? false : true;
}

void libprogressbar_set_length( unsigned int length )
{
    __libprogressbar_length = length;
}

void * __libprogressbar_update_status( void * arg )
{
    unsigned int i;
    unsigned int length;
    unsigned int percent;
    unsigned int gear;
    bool         complete;
    
    length   = __libprogressbar_length;
    complete = false;
    gear     = 0;
    
    for( ; ; )
    {
        percent  = *( ( unsigned int * )arg );
        complete = ( percent == 100 ) ? true : false;
        gear     = ( gear    == 800 ) ? 0    : gear + 1;
        
        printf( "[" );
        
        if( percent > 1 )
        {
            for( i = 0; i <= ( ( ( double )length / 100 ) * percent ) - 2; i++ )
            {
                printf( "=" );
            }
            
            printf( ( percent == 100 ) ? "=" : ">" );
        }
        
        for( i = ( ( ( double )length / 100 ) * percent ); i < length; i++ )
        {
            printf( " " );
        }
        
        printf( "]" );
        
        printf( " %u%% ", percent );
        
        if( complete == false )
        {
            if( gear < 200 )
            {
                printf( "|" );
            }
            else if( gear < 400 )
            {
                printf( "/" );
            }
            else if( gear < 600 )
            {
                printf( "-" );
            }
            else if( gear < 800 )
            {
                printf( "\\" );
            }
        }
        
        if( complete == true )
        {
            printf( "[DONE]" );
        }
        
        printf( "\r" );
        fflush( stdout );
        
        if( complete == true )
        {
            break;
        }
        
        usleep( 500 );
    }
    
    printf( "\n" );
    
    return NULL;
}

int main( void )
{
    unsigned int i;
    unsigned int percent;
    
    i       = 0;
    percent = 0;
    
    libprogressbar_create_progressbar( &percent );
    
    for( i = 0; i <= 100; i++ )
    {
        percent = i;
        usleep( 50000 );
    }
    
    return EXIT_SUCCESS;
}
