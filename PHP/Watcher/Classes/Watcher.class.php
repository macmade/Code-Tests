<?php

# $Id: Watcher.class.php 57 2009-01-30 05:52:42Z macmade $

abstract class Watcher
{
    private $_publicProperties    = array();
    private $_protectedProperties = array();
    private $_watchers            = array();
    
    public function __get( $name )
    {
        if( isset( $this->_publicProperties[ $name ] ) ) {
            
            return $this->_publicProperties[ $name ];
        }
        
        if( isset( $this->_protectedProperties[ $name ] ) ) {
            
            return $this->_publicProperties[ $name ];
        }
        
        trigger_error( 'Property ' . $name . ' does not exist', E_USER_ERROR );
    }
    
    public function __set( $name, $value )
    {
        if( isset( $this->_publicProperties[ $name ] ) ) {
            
            if( isset( $this->_watchers[ $name ] ) ) {
                
                foreach( $this->_watchers[ $name ] as $watcher ) {
                    
                    $watcher[ 'object' ]->$watcher[ 'method' ]( $name, $value );
                }
            }
            
            return $this->_publicProperties[ $name ] = $value;
        }
        
        if( isset( $this->_protectedProperties[ $name ] ) ) {
            
            if( isset( $this->_watchers[ $name ] ) ) {
                
                foreach( $this->_watchers[ $name ] as $watcher ) {
                    
                    $watcher[ 'object' ]->$watcher[ 'method' ]( $name, $value );
                }
            }
            
            return $this->_publicProperties[ $name ] = $value;
        }
        
        trigger_error( 'Property ' . $name . ' does not exist', E_USER_ERROR );
    }
    
    protected function registerPublicProperty( $name, $value )
    {
        if( isset( $this->_publicProperties[ $name ] ) ||  isset( $this->_protectedProperties[ $name ] ) ) {
            
            trigger_error( 'A property with the same name has already been registered', E_USER_ERROR );
        }
        
        $this->_publicProperties[ $name ] = $value;
        
        return true;
    }
    
    protected function registerProtectedProperty( $name, $value )
    {
        if( isset( $this->_publicProperties[ $name ] ) ||  isset( $this->_protectedProperties[ $name ] ) ) {
            
            trigger_error( 'A property with the same name has already been registered', E_USER_ERROR );
        }
        
        $this->_protectedProperties[ $name ] = $value;
        
        return true;
    }
    
    public function watchProperty( $name, $object, $method )
    {
        if( !isset( $this->_publicProperties[ $name ] ) && !isset( $this->_protectedProperties[ $name ] ) ) {
            
            trigger_error( 'Property ' . $name . ' does not exist', E_USER_ERROR );
        }
        
        if( !isset( $this->_watchers[ $name ] ) ) {
            
            $this->_watchers[ $name ] = array();
        }
        
        $this->_watchers[ $name ][] = array(
            'object' => $object,
            'method' => $method
        );
        
        return true;
    }
}
