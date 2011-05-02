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

#define Class( name, props, ... )       \
typedef struct __ ## name ## _Struct * name;  \
typedef struct __ ##name ## _Struct          \
{                                       \
    struct props properties;            \
    __VA_ARGS__                         \
}                                       \
__ ## name;

#define MethodMember( cName, mName, ret, ... )  ret ( * mName )( struct __ ## cName ## _Struct * self, __VA_ARGS__ )

#define MethodPrototype( cName, mName, ret, ... )      ret cName ## _ ## mName ( struct __ ## cName ## _Struct * self, __VA_ARGS__ )

#define MethodImplementation( cName, mName, ret, ... ) ret cName ## _ ## mName ( struct __ ## cName ## _Struct * self, __VA_ARGS__ )

#define New( cName )                                \
    ( cName )malloc( sizeof( __ ## cName ) );       \
    cName ## _RT_Init( o )

#define Call( o, mName, ... ) o->mName( o, __VA_ARGS__ )

/*----------------------------------------------------------------------------*/

Class
(
    MyClass,
    
    {
        unsigned int x;
        unsigned int y;
    },
    
    MethodMember( MyClass, SayHelloWorld,    void, char * test );
    MethodMember( MyClass, SayHelloUniverse, void, char * test );
)

MethodPrototype( MyClass, SayHelloWorld, void, char * test );
MethodPrototype( MyClass, SayHelloUniverse, void, char * test );


/*----------------------------------------------------------------------------*/

void MyClass_RT_Init( __MyClass * o );
void MyClass_RT_Init( __MyClass * o )
{
    o->SayHelloWorld    = MyClass_SayHelloWorld;
    o->SayHelloUniverse = MyClass_SayHelloUniverse;
}

/*----------------------------------------------------------------------------*/

MethodImplementation( MyClass, SayHelloWorld, void, void )
{
    printf( "hello, world\n" );
}

MethodImplementation( MyClass, SayHelloUniverse, void, char * test )
{
    printf( "hello, universe\n" );
}

/*----------------------------------------------------------------------------*/

int main( void )
{
    __MyClass * o;
    
    o = New( MyClass );
    
    o->SayHelloUniverse( o, NULL );
    
    printf( "%i\n", o->properties.x );
    
    return 0;
}
