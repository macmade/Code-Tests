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

#include <iostream>
#include <functional>
#include <cstdlib>
#include <cmath>
#include <objc/runtime.h>
#include <objc/message.h>

static id _OBJCXX_Observer_initWithFunc( id self, SEL selector, std::function< void( id ) > * func );
static id _OBJCXX_Observer_initWithFunc( id self, SEL selector, std::function< void( id ) > * func )
{
    Ivar ivar;
    
    self = objc_msgSend( self, sel_registerName( "init" ) );
    
    if( self )
    {
        ivar = class_getInstanceVariable( object_getClass( self ), "_func" );
        
        object_setIvar( self, ivar, reinterpret_cast< id >( func ) );
    }
    
    return self;
}

static void _OBJCXX_Observer_dealloc( id self, SEL selector );
static void _OBJCXX_Observer_dealloc( id self, SEL selector )
{
    Ivar                          ivar;
    std::function< void( id ) > * func;
    
    if( self )
    {
        ivar = class_getInstanceVariable( object_getClass( self ), "_func" );
        
        if( ivar )
        {
            func = reinterpret_cast< std::function< void( id ) > * >( object_getIvar( self, ivar ) );
            
            delete func;
        }
    }
    
    {
        struct objc_super super;
        
        super.receiver    = self;
        super.super_class = class_getSuperclass( object_getClass( self ) );
        
        objc_msgSendSuper( &super, selector );
    }
}

static void _OBJCXX_Observer_receiveNotification( id self, SEL selector, id notification );
static void _OBJCXX_Observer_receiveNotification( id self, SEL selector, id notification )
{
    Ivar                          ivar;
    std::function< void( id ) > * func;
    
    if( self )
    {
        ivar = class_getInstanceVariable( object_getClass( self ), "_func" );
        
        if( ivar )
        {
            func = reinterpret_cast< std::function< void( id ) > * >( object_getIvar( self, ivar ) );
            
            if( func )
            {
                ( *( func ) )( notification );
            }
        }
    }
}

int main( void )
{
    Class cls;
    
    cls = objc_allocateClassPair( objc_getClass( "NSObject" ), "_OBJCXX_Observer", 0 );
    
    if( cls == nullptr )
    {
        std::cout << "error" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    if( class_addIvar( cls, "_func", sizeof( std::function< void( id ) > * ), log2( sizeof( std::function< void( id ) > * ) ), "" ) == false )
    {
        std::cout << "error" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    if( class_addMethod( cls, sel_registerName( "initWithFunc:" ), reinterpret_cast< IMP >( _OBJCXX_Observer_initWithFunc ), "" ) == false )
    {
        std::cout << "error" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    if( class_addMethod( cls, sel_registerName( "dealloc" ), reinterpret_cast< IMP >( _OBJCXX_Observer_dealloc ), "" ) == false )
    {
        std::cout << "error" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    if( class_addMethod( cls, sel_registerName( "receiveNotification:" ), reinterpret_cast< IMP >( _OBJCXX_Observer_receiveNotification ), "" ) == false )
    {
        std::cout << "error" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    objc_registerClassPair( cls );
    
    {
        id                            obj;
        std::function< void( id ) > * func;
        
        func = new std::function< void( id ) >
        (
            []( id notification )
            {
                std::cout << "Received notification: " << notification << std::endl;
            }
        );
        
        obj = objc_msgSend( reinterpret_cast< id >( cls ), sel_registerName( "alloc" ) );
        obj = objc_msgSend( obj, sel_registerName( "initWithFunc:" ), func );
        
        if( obj == nullptr )
        {
            std::cout << "error" << std::endl;
            
            return EXIT_FAILURE;
        }
        
        objc_msgSend( obj, sel_registerName( "receiveNotification:" ), nullptr );
        objc_msgSend( obj, sel_registerName( "release" ) );
    }
    
    return EXIT_SUCCESS;
}
