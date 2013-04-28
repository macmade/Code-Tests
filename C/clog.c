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

#include <asl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
 * Public functions prototypes
 ******************************************************************************/

#pragma mark - Public functions prototypes -

void CLog(          char * fmt, ... );
void CLogEmergency( char * fmt, ... );
void CLogAlert(     char * fmt, ... );
void CLogCritical(  char * fmt, ... );
void CLogError(     char * fmt, ... );
void CLogWarning(   char * fmt, ... );
void CLogNotice(    char * fmt, ... );
void CLogInfo(      char * fmt, ... );
void CLogDebug(     char * fmt, ... );

/*******************************************************************************
 * C entry point
 ******************************************************************************/

#pragma mark - C entry point -

int main( void )
{
    CLogNotice( "hello, world" );
    return EXIT_SUCCESS;
}

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/

#pragma mark - Private functions prototypes -

void __clog_create_client( void );
void __clog_log( int level, char * fmt, va_list ap );

/*******************************************************************************
 * Private macros
 ******************************************************************************/

#pragma mark - Private macros -

#define __CLOG_VARGS_LOG( LEVEL )   \
    va_list ap;                     \
    va_start( ap, fmt );            \
    __clog_log( LEVEL, fmt, ap );   \
    va_end( ap );

#ifndef __bool_true_false_are_defined
    #ifdef _Bool
        #define bool                        _Bool
    #else
        #define bool                        char
    #endif
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

/*******************************************************************************
 * Private variables
 ******************************************************************************/

#pragma mark - Private variables -

static aslclient __clog_client;
static bool      __clog_client_connected = false;

/*******************************************************************************
 * Private functions
 ******************************************************************************/

#pragma mark - Private functions -

void __clog_create_client( void )
{
    __clog_client = asl_open( "CLog", "com.apple.console", ASL_OPT_STDERR | ASL_OPT_NO_DELAY );
}

void __clog_log( int level, char * fmt, va_list ap )
{
    aslmsg msg;
    
    if( __clog_client_connected == false )
    {
        __clog_create_client();
    }
    
    if( __clog_client == NULL )
    {
        fprintf( stderr, "Impossible to create the ASL client\n" );
        exit( EXIT_FAILURE );
    }
    
    
    msg = asl_new( ASL_TYPE_MSG );
    
    asl_set( msg, ASL_KEY_FACILITY, "com.apple.console" );
    asl_vlog( __clog_client, msg, level, fmt, ap );
}

/*******************************************************************************
 * Public functions
 ******************************************************************************/

#pragma mark - Public functions -

void CLog( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_WARNING );
}

void CLogEmergency( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_EMERG );
}

void CLogAlert( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_ALERT );
}

void CLogCritical( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_CRIT );
}

void CLogError( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_ERR );
}

void CLogWarning( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_WARNING );
}

void CLogNotice( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_NOTICE );
}

void CLogInfo( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_INFO );
}

void CLogDebug( char * fmt, ... )
{
    __CLOG_VARGS_LOG( ASL_LEVEL_DEBUG );
}
