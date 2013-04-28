<?php

# $Id$

class Bar
{
    public function __construct()
    {
        $foo = new Foo();
        
        $foo->watchProperty( 'test1', $this, 'myWatcher' );
        $foo->watchProperty( 'test2', $this, 'myWatcher' );
        $foo->watchProperty( 'test3', $this, 'myWatcher' );
        $foo->watchProperty( 'test4', $this, 'myWatcher' );
        $foo->watchProperty( 'test5', $this, 'myWatcher' );
        
        $foo->changeProperties();
    }
    
    public function myWatcher( $name, $value )
    {
        print 'Property ' . $name . ' has changed with new value "' . $value . '"' . chr( 10 );
    }
}
