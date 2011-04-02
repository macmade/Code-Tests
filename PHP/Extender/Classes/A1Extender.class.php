<?php

# $Id: A1Extender.class.php 57 2009-01-30 05:52:42Z macmade $

class A1Extender extends A1
{
    public function __call( $name, array $args = array() )
    {            
        if( substr( $name, 0, 10 ) === 'extended::' ) {
            
            $name = substr( $name, 10 );
            
            return $this->$name();
        }
    }
    
    public function __get( $name )
    {            
        if( substr( $name, 0, 10 ) === 'extended::' ) {
            
            $name = substr( $name, 10 );
            
            return $this->$name;
        }
    }
    
    public function __set( $name, $value )
    {            
        if( substr( $name, 0, 10 ) === 'extended::' ) {
            
            $name = substr( $name, 10 );
            
            return $this->$name = $value;
        }
    }
}
