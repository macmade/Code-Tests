<?php

# $Id: Foo.class.php 57 2009-01-30 05:52:42Z macmade $

class Foo extends Watcher
{
    public function __construct()
    {
        $this->registerPublicProperty( 'test1', false );
        $this->registerPublicProperty( 'test2', false );
        $this->registerPublicProperty( 'test3', false );
        $this->registerPublicProperty( 'test4', false );
        $this->registerPublicProperty( 'test5', false );
    }
    
    public function changeProperties()
    {
        $this->test1 = true;
        $this->test2 = true;
        $this->test3 = true;
        $this->test4 = true;
        $this->test5 = true;
    }
}
