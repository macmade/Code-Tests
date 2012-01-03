#import <Foundation/Foundation.h>

/* #define NSTHREAD_USES_GCD   1 */

@implementation NSThread( GCD )

#if defined( NSTHREAD_USES_GCD ) && NSTHREAD_USES_GCD == 1

+ ( void )detachNewThreadSelector: ( SEL )selector toTarget: ( id )target withObject: ( id )argument
{
    dispatch_async
    (
        dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 ),
        ^
        {
            [ target performSelector: selector withObject: argument ];
        }
    );
}

#endif

@end

@interface Foo: NSObject
{
@private
    
    id _Test_Reserved[ 5 ];
}

- ( void )log: ( NSString * )str;

@end

@implementation Foo

- ( void )log: ( NSString * )str
{
    printf( "%s ", [ str UTF8String ] );
    fflush( stdout );
}

@end

int main( void )
{
    NSAutoreleasePool * ap;
    Foo               * foo;
    NSUInteger          i;
    
    ap  = [ NSAutoreleasePool new ];
    foo = [ Foo new ];
    
    for( i = 33; i < 256; i++ )
    {
        [ NSThread detachNewThreadSelector: @selector( log: ) toTarget: foo withObject: [ NSString stringWithFormat: @"%c", i ] ];
    }
    
    [ foo release ];
    [ ap release ];
    
    return EXIT_SUCCESS;
}
