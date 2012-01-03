#import <Foundation/Foundation.h>
#import <objc/runtime.h>

@interface NSObject( Abstract )

- ( void )registerAbstractMethod: ( NSString * )name;

@end

@implementation NSObject( Abstract )

- ( void )registerAbstractMethod: ( NSString * )name
{
    Class         cls;
    SEL           selector;
    Method        m;
    NSException * e;
    
    selector = NSSelectorFromString( name );
    cls      = [ self class ];
    m        = class_getInstanceMethod(  cls, selector );
    
    if( m == NULL )
    {
        e = [ NSException exceptionWithName: @"AbstractClassException" reason: [ NSString stringWithFormat: @"Class %@ must implement abstract method %@", NSStringFromClass( cls ), name ] userInfo: nil ];
        
        @throw e;
    }
}

@end

@interface TestAbstractClass: NSObject
{
@protected
    
    
    
@private
    
    id _TestAbstractClass_Reserved[ 5 ];
}

@end

@implementation TestAbstractClass

- ( id )init
{
    if( ( self = [ super init ] ) )
    {
        [ self registerAbstractMethod: @"doSomething" ];
    }
    
    return self;
}

@end

@interface TestClass: TestAbstractClass
{
@protected
    
    
    
@private
    
    id _TestClass_Reserved[ 5 ];
}

@end

@implementation TestClass



@end

int main( void )
{
    TestClass * test;
    
    test = [ [ TestClass alloc ] init ];
    
    [ test release ];
    
    return 0;
}
