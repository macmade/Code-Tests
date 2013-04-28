<?php

# $Id$

class A1
{
    protected $_protectedMemberProperty = 'This is a protected member property... Isn\'t it nice?';
    
    public static function publicStaticMethod()
    {
        return __METHOD__;
    }
    
    protected function _protectedMemberMethod()
    {
        return __METHOD__;
    }
    
    protected function _againAnotherProtectedMemberMethod()
    {
        return __METHOD__;
    }
}
