#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct Function_Struct
{
    char * returnType;
    char * name;
    char * arguments;
    void ( * func )( void );
}
Function;

static Function    * functions     = NULL;
static unsigned int  functionCount = 0;
static unsigned int  functionSize  = 0;

void __RegisterFunction( char * returnType, char * name, void ( * func )( void ), char * arguments );
void __RegisterFunction( char * returnType, char * name, void ( * func )( void ), char * arguments )
{
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
    
    f             = &( functions[ functionCount++ ] );
    f->returnType = returnType;
    f->name       = name;
    f->arguments  = arguments;
    f->func       = func;
}

#if defined( __MACH__ )
    #if defined( __LP64__ )
        #define CallInt CallInt_X85_64_S5
    #endif
#endif

#define RegisterFunction( r, f, ... ) __RegisterFunction( #r, #f, ( void ( * )( void ) )f, #__VA_ARGS__ )
#define Call( f, ... ) ( GetFunction( #f ) != NULL && strcmp( GetFunction( #f )->returnType, "int" ) == 0 ) ? CallInt( GetFunction( #f ), __VA_ARGS__ ) : 0

int CallInt_X85_64_S5( Function * f, ... );
int CallInt_X85_64_S5( Function * f, ... )
{
    int          result;
    char       * type;
    char       * arg;
    char       * args;
    unsigned int argCount;
    va_list      ap;
    int          intval;
    
    args = calloc( sizeof( char ), strlen( f->arguments ) + 1 );
    
    strcpy( args, f->arguments );
    
    va_start( ap, f );
    
    printf( "ABI: x86 64 bits SV - integer type\n" );
    printf( "Calling function %s with signature: %s %s( %s )\n", f->name, f->returnType, f->name, f->arguments );
    
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
        
        printf( "    Processing argument of type: %s\n", type );
        
        argCount++;
        
        if( strcmp( type, "int" ) == 0 )
        {
            intval = va_arg( ap, int );
            
            printf( "        Argument value: %i\n", intval );
            
            __asm__( "push %[v]" : : [ v ] "m" ( intval ) );
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
    
    printf( "    %i arguments need to be passed to function %s\n", argCount, f->name );
    
    __asm__
    (
        "mov %[c], %%r10;"
        "mov %[c], %%ebx;"
        "sub $1, %%ebx;"
        
        "arg_loop:"
            
        "    cmp $0, %%r10;"
        "    jz end_arg;"
        
        "    cmp $0, %%ebx;"
        "    je arg_push_rdi;"
        
        "    cmp $1, %%ebx;"
        "    je arg_push_rsi;"
        
        "    cmp $2, %%ebx;"
        "    je arg_push_rdx;"
        
        "    cmp $3, %%ebx;"
        
        "    je arg_push_rcx;"
        "    arg_push_rdi:"
        "    pop %%rdi;"
        "    jmp arg_push_end;"
        
        "    arg_push_rsi:"
        "    pop %%rsi;"
        "    jmp arg_push_end;"
        
        "    arg_push_rdx:"
        "    pop %%rdx;"
        "    jmp arg_push_end;"
        
        "    arg_push_rcx:"
        "    pop %%rcx;"
        "    jmp arg_push_end;"
        
        "    arg_push_end:"
        "    dec %%r10;"
        "    dec %%ebx;"
        "    jmp arg_loop;"
            
        "end_arg:"
            
        "    call *%[f];"
        "    mov %%rax, %[r];"
        
        : [ r ] "=m" ( result )
        : [ f ] "m" ( f->func ),
          [ c ] "m" ( argCount )
    );
    
    return result;
}

int Foo( int a, int b, int c );
int Foo( int a, int b, int c )
{
    printf( "A: %i\n", a );
    printf( "B: %i\n", b );
    printf( "C: %i\n", c );
    
    return 42;
}

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

int main( void )
{
    int x;
    
    RegisterFunction( int, Foo, int, int, int );
    
    x = Call( Foo, 27, 42, 64 );
    
    printf( "X: %i\n", x );
    
    return 0;
}
