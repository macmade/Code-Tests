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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
 * Portable code section
 ******************************************************************************/

/*!
 * @enum    DataType
 * @brief   C data types
 */
typedef enum
{
    DataTypeNone      = 0x00,       /*! No data type */
    DataTypeChar      = 0x01,       /*! char */
    DataTypeUChar     = 0x02,       /*! unsigned char */
    DataTypeShort     = 0x03,       /*! short */
    DataTypeUShort    = 0x04,       /*! unsigned short */
    DataTypeInt       = 0x05,       /*! int */
    DataTypeUInt      = 0x06,       /*! unsigned int */
    DataTypeLong      = 0x07,       /*! long */
    DataTypeULong     = 0x08,       /*! unsigned long */
    DataTypeLongLong  = 0x09,       /*! long long */
    DataTypeULongLong = 0x0A,       /*! unsigned long long */
    DataTypeFloat     = 0x0B,       /*! float */
    DataTypeDouble    = 0x0C,       /*! double */
    DataTypePointer   = 0x0D        /*! Pointer */
}
DataType;

/*!
 * @struct  Function_Struct
 * @brief   Functiom implementation details
 * @details The function signature consists of the function's return type,
 *          The function's name, and the function's arguments. Those three parts
 *          are separated by a semicolon character.
 *          The return type and the arguments are coded as follow:
 *          
 *              - c     char
 *              - C     unsigned char
 *              - s     short
 *              - S     unsigned short
 *              - i     int
 *              - I     unsigned int
 *              - l     long
 *              - L     unsigned long
 *              - e     long long
 *              - E     unsigned long long
 *              - f     float
 *              - d     double
 *              - p     pointer
 * 
 *          For instance, the signature for 'int foo( void * ptr, int x )' will
 *          be: i:foo:pi.
 */
struct Function_Struct
{
    char       * name;                  /*! The function's name */
    char       * arguments;             /*! The function's arguments */
    char         signature[ 256 ];      /*! The function's signature */
    DataType     returnType;            /* The function's return type */
    DataType     argumentTypes[ 6 ];    /* The function's arguments type */
    unsigned int argumentCount;         /*! The number of arguments */
    void ( * func )( void );            /* The pointer to the function */
};

/*!
 * @struct  Function_Struct
 * @brief   Function implementation type
 */
typedef struct Function_Struct Function;

/*!
 * @var     functions
 * @brief   The functions table
 */
static Function    * functions     = NULL;

/*!
 * @var     functionCount
 * @brief   The number of functions in the functions table
 */
static unsigned int  functionCount = 0;

/*!
 * @var     functionSize
 * @brief   The functions table allocated size
 */
static unsigned int  functionSize  = 0;

/*!
 * @def     RegisterFunction
 * @brief   Registers a runtime function
 * @param   returnType  The function's return type
 * @param   name        The function's name
 * @param   ...         The function's arguments
 * @result  void
 */
#define RegisterFunction( r, f, ... ) __RegisterFunction( #r, #f, ( void ( * )( void ) )f, #__VA_ARGS__ )

/*!
 * @def     Call
 * @brief   Calls a runtime function
 * @param   f       The function to call
 * @param   ...     The function's arguments
 */
#define Call( f, ... ) \
(   GetFunction( #f ) != NULL \
&&  GetFunction( #f )->returnType == DataTypeInt )    ? CallInt(    GetFunction( #f ), __VA_ARGS__ ) \
: ( GetFunction( #f )->returnType == DataTypeFloat )  ? CallFloat(  GetFunction( #f ), __VA_ARGS__ ) \
: ( GetFunction( #f )->returnType == DataTypeDouble ) ? CallDouble( GetFunction( #f ), __VA_ARGS__ ) \
: 0

/*!
 * @brief   Registers a runtime function
 * @details Do not call this function directly. Use the RegisterFunction macro instead.
 * @param   returnType  The function's return type
 * @param   name        The function's name
 * @param   func        The pointer to the function
 * @param   arguments   The function's arguments
 * @result  void
 */
void __RegisterFunction( char * returnType, char * name, void ( * func )( void ), char * arguments );
void __RegisterFunction( char * returnType, char * name, void ( * func )( void ), char * arguments )
{
    char     * args_orig;
    char     * args;
    char     * arg;
    char     * type;
    Function * f;
    
    if( functions == NULL )
    {
        functions    = ( Function * )calloc( sizeof( Function ), 100 );
        functionSize = 100;
    }
    
    if( functionCount == functionSize )
    {
        functions = realloc( functions, functionCount + 100 );
    }
    
    f = &( functions[ functionCount++ ] );
    
    memset( f->signature, 0, 256 );
    
    if( strcmp( returnType, "char" ) == 0 )
    {
        f->returnType = DataTypeChar;
        
        strcat( f->signature, "c:" );
    }
    else if( strcmp( returnType, "unsigned char" ) == 0 )
    {
        f->returnType = DataTypeUChar;
        
        strcat( f->signature, "C:" );
    }
    else if( strcmp( returnType, "short" ) == 0 )
    {
        f->returnType = DataTypeShort;
        
        strcat( f->signature, "s:" );
    }
    else if( strcmp( returnType, "unsigned short" ) == 0 )
    {
        f->returnType = DataTypeUShort;
        
        strcat( f->signature, "S:" );
    }
    else if( strcmp( returnType, "int" ) == 0 )
    {
        f->returnType = DataTypeInt;
        
        strcat( f->signature, "i:" );
    }
    else if( strcmp( returnType, "unsigned int" ) == 0 )
    {
        f->returnType = DataTypeUInt;
        
        strcat( f->signature, "I:" );
    }
    else if( strcmp( returnType, "long" ) == 0 )
    {
        f->returnType = DataTypeLong;
        
        strcat( f->signature, "l:" );
    }
    else if( strcmp( returnType, "unsigned long" ) == 0 )
    {
        f->returnType = DataTypeULong;
        
        strcat( f->signature, "L:" );
    }
    else if( strcmp( returnType, "long long" ) == 0 )
    {
        f->returnType = DataTypeLongLong;
        
        strcat( f->signature, "e:" );
    }
    else if( strcmp( returnType, "unsigned long long" ) == 0 )
    {
        f->returnType = DataTypeULongLong;
        
        strcat( f->signature, "E:" );
    }
    else if( strcmp( returnType, "float" ) == 0 )
    {
        f->returnType = DataTypeFloat;
        
        strcat( f->signature, "f:" );
    }
    else if( strcmp( returnType, "double" ) == 0 )
    {
        f->returnType = DataTypeDouble;
        
        strcat( f->signature, "d:" );
    }
    else if( returnType[ strlen( returnType ) - 1 ] == '*' )
    {
        f->returnType = DataTypePointer;
        
        strcat( f->signature, "p:" );
    }
    
    strcat( f->signature, name );
    strcat( f->signature, ":" );
    
    args      = calloc( sizeof( char ), strlen( f->signature ) + 1 );
    args_orig = args;
    
    strcpy( args, arguments );
    
    while( 1 )
    {
        if( f->argumentCount == 6 )
        {
            break;
        }
        
        arg = strchr( args, ',' );
        
        if( arg != NULL )
        {
            arg[ 0 ] = 0;
            type     = args;
            
            if( arg[ 1 ] == ' ' )
            {
                arg++;
            }
        }
        else
        {
            type = args;
        }
        
        f->argumentCount++;
        
        if( strcmp( arguments, "void" ) == 0 )
        {
            break;
        }
        else if( strcmp( type, "char" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeChar;
            
            strcat( f->signature, "c" );
        }
        else if( strcmp( type, "unsigned char" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeUChar;
            
            strcat( f->signature, "C" );
        }
        else if( strcmp( type, "short" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeShort;
            
            strcat( f->signature, "s" );
        }
        else if( strcmp( type, "unsigned short" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeUShort;
            
            strcat( f->signature, "S" );
        }
        else if( strcmp( type, "int" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeInt;
            
            strcat( f->signature, "i" );
        }
        else if( strcmp( type, "unsigned int" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeUInt;
            
            strcat( f->signature, "I" );
        }
        else if( strcmp( type, "long" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeLong;
            
            strcat( f->signature, "l" );
        }
        else if( strcmp( type, "unsigned long" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeULong;
            
            strcat( f->signature, "L" );
        }
        else if( strcmp( type, "long long" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeLongLong;
            
            strcat( f->signature, "e" );
        }
        else if( strcmp( type, "unsigned long long" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeULongLong;
            
            strcat( f->signature, "E" );
        }
        else if( strcmp( arguments, "float" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeFloat;
            
            strcat( f->signature, "f" );
        }
        else if( strcmp( type, "double" ) == 0 )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypeDouble;
            
            strcat( f->signature, "d" );
        }
        else if( type[ strlen( type ) - 1 ] == '*' )
        {
            f->argumentTypes[ f->argumentCount ] = DataTypePointer;
            
            strcat( f->signature, "p" );
        }
        
        if( arg == NULL )\
        {
            break;
        }
        else
        {
            args = arg + 1;
        }
    }
    
    free( args_orig );
    
    f->name      = name;
    f->arguments = arguments;
    f->func      = func;
}

/*!
 * @brief   Finds a runtime function
 * @param   name    The function's name
 * @result  A pointer to the function implementation structure, or NULL if the method does not exist
 */
Function * GetFunction( char * name );
Function * GetFunction( char * name )
{
    int i;
    
    for( i = 0; i < functionCount; i++ )
    {
        if( strcmp( functions[ i ].name, name ) == 0 )
        {
            return &( functions[ i ] );
        }
    }
    
    return NULL;
}

/* Mac OS X */
#if defined( __MACH__ )
    
/* 64bits */
#if defined( __LP64__ )

/*******************************************************************************
 * System V - x86 - 64bits
 ******************************************************************************/
/*!
 * @def     System5_x86_64
 * @brief   ...
 * @param   type            The return type
 * @param   returnRegister  The register used for the return value
 */
 #define System5_x86_64( type, returnRegister )                     \
     /*! Number of arguments needed to be passed */                 \
    "mov %[c], %%r10;"                                              \
    "mov %[c], %%ebx;"                                              \
    "sub $1, %%ebx;"                                                \
                                                                    \
    /* Loop for each argument */                                    \
    "arg_loop_" type ":"                                            \
                                                                    \
    /* Checks if we've got arguments left */                        \
    "    cmp $0, %%r10;"                                            \
                                                                    \
    /* No, call the function */                                     \
    "    jz call_func_" type ";"                                    \
                                                                    \
    /* Argument 1 is passed in RDI */                               \
    "    cmp $0, %%ebx;"                                            \
    "    je arg_push_rdi_" type ";"                                 \
                                                                    \
    /* Argument 2 is passed in RSI */                               \
    "    cmp $1, %%ebx;"                                            \
    "    je arg_push_rsi_" type ";"                                 \
                                                                    \
    /* Argument 3 is passed in RDX */                               \
    "    cmp $2, %%ebx;"                                            \
    "    je arg_push_rdx_" type ";"                                 \
                                                                    \
    /* Argument 4 is passed in RCX */                               \
    "    cmp $3, %%ebx;"                                            \
    "    je arg_push_rcx_" type ";"                                 \
                                                                    \
    /* Argument 5 is passed in R8 */                                \
    "    cmp $4, %%ebx;"                                            \
    "    je arg_push_r8_" type ";"                                  \
                                                                    \
    /* Argument 6 is passed in R9 */                                \
    "    cmp $5, %%ebx;"                                            \
    "    je arg_push_r9_" type ";"                                  \
                                                                    \
    /* Fill RDI with the first argument */                          \
    "    arg_push_rdi_" type ":"                                    \
    "    pop %%rdi;"                                                \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Fill RSI with the second argument */                         \
    "    arg_push_rsi_" type ":"                                    \
    "    pop %%rsi;"                                                \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Fill RDX with the third argument */                          \
    "    arg_push_rdx_" type ":"                                    \
    "    pop %%rdx;"                                                \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Fill RCX with the fourth argument */                         \
    "    arg_push_rcx_" type ":"                                    \
    "    pop %%rcx;"                                                \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Fill R8 with the fifth argument */                           \
    "    arg_push_r8_" type ":"                                     \
    "    pop %%r8;"                                                 \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Fill R9 with the sixth argument */                           \
    "    arg_push_r9_" type ":"                                     \
    "    pop %%r9;"                                                 \
    "    jmp arg_push_end_" type ";"                                \
                                                                    \
    /* Process next argument */                                     \
    "    arg_push_end_" type ":"                                    \
    "    dec %%r10;"                                                \
    "    dec %%ebx;"                                                \
    "    jmp arg_loop_" type ";"                                    \
                                                                    \
    /* Calls the function - Return value is placed in RAX */        \
    "call_func_" type ":"                                           \
                                                                    \
    "    call *%[f];"                                               \
    "    " returnRegister ", %[r];"                                 \
                                                                    \
    : [ r ] "=m" ( result )                                         \
    : [ f ] "m" ( f->func ),                                        \
      [ c ] "m" ( argCount )

int CallInt( Function * f, ... );
int CallInt( Function * f, ... )
{
    int result;
    
    char       * type;
    char       * arg;
    char       * args;
    unsigned int argCount;
    va_list      ap;
    int          intval;
    
    args = calloc( sizeof( char ), strlen( f->arguments ) + 1 );
    
    strcpy( args, f->arguments );
    va_start( ap, f );
    
    while( 1 )
    {
        arg = strchr( args, ',' );
        
        if( arg != NULL )
        {
            arg[ 0 ] = 0;
            type     = args;
            
            if( arg[ 1 ] == ' ' )
            {
                arg++;
            }
        }
        else
        {
            type = args;
        }
        
        argCount++;
        
        if( strcmp( type, "int" ) == 0 )
        {
            intval = va_arg( ap, int );
            
            __asm__
            (
                "push %[v];"
                :
                : [ v ] "m" ( intval )
            );
        }
        
        if( arg == NULL )
        {
            break;
        }
        else
        {
            args = arg + 1;
        }
    }
    
    va_end( ap );
    
    __asm__( System5_x86_64( "int", "mov %%rax" ) );
    
    return result;
}

float CallFloat( Function * f, ... );
float CallFloat( Function * f, ... )
{
    return 0;
}

double CallDouble( Function * f, ... );
double CallDouble( Function * f, ... )
{
    return 0;
}

#endif

#endif

/*******************************************************************************
 * Testing
 ******************************************************************************/

int Foo( int a);
int Foo( int a )
{
    printf( "A: %i\n", a );
    
    return 42;
}

int main( void )
{
    int x;
    
    RegisterFunction( int, Foo, int );
    
    x = Call( Foo, 27 );
    
    printf( "X: %i\n", x );
    
    return 0;
}
