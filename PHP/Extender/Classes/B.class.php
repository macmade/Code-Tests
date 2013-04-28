<?php

# $Id$

class B extends ClassExtender
{
    protected $NL  = '';
    protected $TAB = '';
    
    public function __construct()
    {
        $this->NL  = chr( 10 );
        $this->TAB = chr( 9 );
        $this->printStr( 'Class Extender tests', 0, 2 );
    }
    
    public function printStr( $str, $tabNum = 0, $nlNum = 1 )
    {
        print str_repeat( $this->TAB, $tabNum ) . $str . str_repeat( $this->NL, $nlNum );
        return true;
    }
    
    public function doTest()
    {
        $this->printStr( '*** Extends A1' );
        $this->_extends( 'A1' );
        
        $this->printStr( '*** Extends A2', 0, 2 );
        $this->_extends( 'A2' );
        
        $this->printStr( '*** Calls protected methods' );
        $this->printStr( $this->_protectedMemberMethod(), 1);
        $this->printStr( $this->_anotherProtectedMemberMethod(), 1 );
        $this->printStr( $this->_againAnotherProtectedMemberMethod(), 1, 2 );
        
        $this->printStr( '*** Gets a protected property' );
        $this->printStr( $this->_protectedMemberProperty, 1, 2 );
        
        $this->printStr( '*** Sets a protected property' );
        $this->_protectedMemberProperty .= ' And I can even change it\'s value...';
        
        print $this->NL;
        
        $this->printStr( '*** Calls on protected methods' );
        $this->printStr( $this->_protectedMemberProperty, 1, 2 );
        
        $this->printStr( '*** Releases A2', 0, 2 );
        $this->_releaseExtends( 'A2' );
        
        $this->printStr( '*** Calls protected methods' );
        $this->printStr( $this->_protectedMemberMethod(), 1, 2 );
        
        $this->printStr( '*** Extends A2', 0, 2 );
        $this->_extends( 'A2' );
        
        $this->printStr( '*** Calls protected methods' );
        $this->printStr( $this->_protectedMemberMethod(), 1, 2 );
        
        $this->printStr( '*** Releases A2', 0, 2 );
        $this->_releaseExtends( 'A2' );
        
        $this->printStr( '*** Calls protected methods' );
        $this->printStr( $this->_protectedMemberMethod(), 1, 2 );
        
        $this->printStr( '*** Extends a specific method of A2', 0, 2 );
        $this->_extends( 'A2', array( '_anotherProtectedMemberMethod' ) );
        
        $this->printStr( '*** Calls protected methods' );
        $this->printStr( $this->_protectedMemberMethod(), 1);
        $this->printStr( $this->_anotherProtectedMemberMethod(), 1 );
        $this->printStr( $this->_againAnotherProtectedMemberMethod(), 1, 2 );
    }
}
