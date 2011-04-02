<?php

# $Id: Bar.class.php 57 2009-01-30 05:52:42Z macmade $

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
