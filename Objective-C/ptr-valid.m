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

#import <Foundation/Foundation.h>
#import <objc/objc.h>
#import <objc/objc-runtime.h>

BOOL is_valid_objc_instance( id obj );

int main( void )
{
    printf( "Valid ptr: %i\n", is_valid_objc_instance( ( id )1 ) );
    return 0;
}

static jmp_buf __is_valid_objc_instance_jmp_buf;

void __is_valid_objc_instance_sig( int sig );
void __is_valid_objc_instance_sig( int sig )
{
    ( void )sig;
    longjmp( __is_valid_objc_instance_jmp_buf, 1 );
}

BOOL is_valid_objc_instance( id obj )
{
    char                 c;
    struct objc_object * object;
    Class                objectClass;
    Class              * classes;
    int                  classCount;
    BOOL                 classFound;
    
    void ( * sigsegv )( int );
    void ( * sigbus  )( int );
    
    sigsegv = NULL;
    sigbus  = NULL;
    
    if( obj == NULL )
    {
        return YES;
    }
    
    if( setjmp( __is_valid_objc_instance_jmp_buf ) )
    {
        ( void )signal( SIGSEGV, sigsegv );
        ( void )signal( SIGBUS, sigbus );
        return NO;
    }
    
    sigsegv = signal( SIGSEGV, __is_valid_objc_instance_sig );
    sigbus  = signal( SIGBUS,  __is_valid_objc_instance_sig );
    
    memcpy( ( void * )&c, ( const void * )obj, 1 );
    
    object      = ( struct objc_object * )obj;
    objectClass = object->isa;
    classCount  = objc_getClassList( NULL, 0 );
    classes     = malloc( sizeof( Class ) * classCount );
    classCount  = objc_getClassList( classes, classCount );
    classFound  = NO;
    
    for( ; classCount >= 0; classCount-- )
    {
        if( classes[ classCount ] == objectClass )
        {
            classFound = YES;
            break;
        }
    }
    
    free( classes );
    ( void )signal( SIGSEGV, sigsegv );
    ( void )signal( SIGBUS, sigbus );
    
    return classFound;
}
