<?php

# $Id$

class A2Extender extends A2
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
