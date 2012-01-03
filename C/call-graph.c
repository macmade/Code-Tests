#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

/*******************************************************************************
 * 0x0000: | main [ @start:0x0000 ]
 * 0x0000: | ----> | foo [ @start:0x0000 ]
 * 0x0000: | <---- | foo [ @start:0x0000 ]
 * 0x0000: | ----> | bar [ @start:0x0000 ]
 * 0x0000: |       | ----> | foobar [ @start:0x0000 ]
 * 0x0000: |       |       | ----> | foobaz [ @start:0x0000 ]
 * 0x0000: |       |       | <---- | foobaz [ @start:0x0000 ]
 * 0x0000: |       | <---- | foobar [ @start:0x0000 ]
 * 0x0000: | <---- | bar [ @start:0x0000 ]
 * 0x0000: | main [ @start:0x0000 ]
*******************************************************************************/

static unsigned int __call_level = 0;

void __cyg_profile_func_enter( void * func_addr, void * call_site ) __attribute__( ( no_instrument_function ) );
void __cyg_profile_func_enter( void * func_addr, void * call_site )
{
    char         buf[ 1024 ];
    Dl_info      func_info;
    Dl_info      caller_info;
    const char * caller_name;
    const char * func_name;
    unsigned int i;
    
    func_name   = ( dladdr( func_addr, &func_info ) )   ? func_info.dli_sname   : "<unknown>";
    caller_name = ( dladdr( call_site, &caller_info ) ) ? caller_info.dli_sname : "<unknown>";
    buf[ 0 ]    = 0;
    
    if( __call_level > 1 )
    {
        for( i = 0; i < __call_level; i++ )
        {
            strcat( buf, "      | " );
        }
    }
    
    if( __call_level > 0 )
    {
        strcat( buf, "----> | " );
    }
    
    printf
    (
        "0x%010lX: | %s%s [ @%s:0x%010lX ]\n",
        ( uintptr_t )func_addr,
        buf,
        func_name,
        caller_name,
        ( uintptr_t )call_site
    );
    
    __call_level++;
}

void __cyg_profile_func_exit( void * func_addr, void * call_site ) __attribute__( ( no_instrument_function ) );
void __cyg_profile_func_exit( void * func_addr, void * call_site )
{
    ( void )func_addr;
    ( void )call_site;
    
    __call_level--;
}

void foo( void );
void bar( void );
void foobar( void );
void foobaz( void );

void foo( void )
{}

void bar( void )
{
    foobar();
}

void foobar( void )
{
    foobaz();
}

void foobaz( void )
{}

int main( void )
{
    foo();
    bar();
    
    return 0;
}
