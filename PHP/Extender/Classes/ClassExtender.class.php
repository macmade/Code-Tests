<?php

# $Id$

################################################################################
# TO THINK ABOUT
################################################################################
# 
# Should we use specific methods, instead of overloading, in the case someone
# already uses overloading?
# 
# Protected are public with this system. May be fixed by using specific methods
# instead overloading, or maybe with some Reflection trick...
# Changing the visibility of the overloading methods is not possible.
# Check get_class( $this )
# 
# We'll have to deal with the class_parents() stuff, and the like...
# This class should have methods to return the object type.
# ClassManager should then detects an object extends the ClassExtender, and
# calls it's method (when using the objectIsA() method, for instance).
# 
# Checks the late static bindings stuff in PHP 5.3
# (http://www.php.net/manual/en/language.oop5.late-static-bindings.php)
# 
# Is there a need for __isset() or __unset overloading? Probably...
# 
# How about static methods and properties? Overload doesn't work with them.
# What about callStatic() in class manager?
# PHP 5.3 seems to have a __callStatic() magic method...
# 
# Possibility to force the use of a specific parent class, instead of the last,
# if using specifig methods instead overloading?
# 
# What about using static array to store informations about classes? This will
# avoid creating Reflection objects each time...
# 
# What if a parent class has abstract methods declarations? Maybe use the
# Reflection to manage this...
# 
# Think about the error management. Some things are clearly fatals, but what
# for the others? Don't tell anything, produces warnings, or all fatals?
# To be or not to be an error... That's the Reflection...
# 
# Checks what happens with a chain of dynamic extends...
# 
# Should we use eval() to automatically create the extender classes? Or maybe
# this class (or class manager) should physicaly creates the extender classes
# files (kinda cache)? Otherwise, let the developer do it...
# 
# Checks the naming of the methods (and the extended:: prefix).
# Also checks the conventions for the extender classes.
# 
# Someone should maybe send a valentine postcard to Robert...
# 
################################################################################

abstract class ClassExtender
{
    private $_instances     = array();
    private $_classes       = array();
    private $_methods       = array();
    private $_properties    = array();

    private function __call( $name, array $args = array() )
    {
        // Checks if the requested method is available
        if( isset( $this->_methods[ $name ] ) ) {
            
            // Changes the method name, to be sure that the __call() method of the extender will be called
            $extendedName = 'extended::' . $name;
            
            // Gets the name of the class which contains the requested method
            $className    = key( $this->_methods[ $name ] );
            
            // Calls the method and returns its result
            return $this->_instances[ $className ]->$extendedName();
        }
        
        // The requested method is not available
        trigger_error( 'Cannot call unexisting method "' . $name . '"', E_USER_ERROR );
    }

    public function __get( $name )
    {
        // Checks if the requested property is available
        if( isset( $this->_properties[ $name ] ) ) {
            
            // Changes the metpropertyhod name, to be sure that the __get() method of the extender will be called
            $extendedName = 'extended::' . $name;
            
            // Gets the name of the class which contains the requested property
            $className    = key( $this->_properties[ $name ] );
            
            // Gets and returns the property
            return $this->_instances[ $className ]->$extendedName;
        }
        
        // The requested property is not available
        trigger_error( 'Cannot access unexisting property "' . $name . '"', E_USER_ERROR );
    }

    public function __set( $name, $value )
    {
        // Checks if the requested property is available
        if( isset( $this->_properties[ $name ] ) ) {
            
            // Changes the property name, to be sure that the __set() method of the extender will be called
            $extendedName = 'extended::' . $name;
            
            // Gets the name of the class which contains the requested property
            $className    = key( $this->_properties[ $name ] );
            
            // Sets and returns the property
            return $this->_instances[ $className ]->$extendedName = $value;
        }
        
        // The requested property is not available
        trigger_error( 'Cannot access unexisting property "' . $name . '"', E_USER_ERROR );
    }

    protected function _extends( $className, array $methods = array(), array $properties = array() )
    {
        // Checks if the requested parent class has already been inherited from 
        if( isset( $_instances[ $className ] ) ) {
            
            // Here I should check if all the methods are defined...
            return true;
        }
        
        // Name of the extender class
        $extender = $className . 'Extender';
        
        // Checks if the parent class exists
        // What about class manager here?
        if( !class_exists( $className ) ) {
            
            // Error management?
            trigger_error( 'Cannot inherit from unexisting class "' . $className . '"', E_USER_ERROR );
        }
        
        // Checks if the extender class exists
        if( !class_exists( $extender ) ) {
            
            // Error management?
            trigger_error( 'Cannot inherit from class "' . $className . '" because it does not have an extender', E_USER_ERROR );
        }
        
        // Creates a new instance of the extender class
        $this->_instances[ $className ] = new $extender();
        #$this->_instances[ $className ] = self::$_classManager->newInstance( $extender );
        
        // Creates a reflection object for the parent class
        $reflection                   = new ReflectionClass( $className );
        
        // Creates the storage place for the parent class informations
        $this->_classes[ $className ] = array(
            'methods'    => array(),
            'properties' => array()
        );
        
        // Checks if only some methods should be inherited
        if( count( $methods ) ) {
            
            // Process each specified method
            foreach( $methods as $key => $value ) {
                
                // Checks if the method exist
                if( !$reflection->hasMethod( $value ) ) {
                    
                    // No, process the next method
                    // Error management?
                    continue;
                }
                
                // Checks if another parent class already registered the method
                if( !isset( $this->_methods[ $value ] ) ) {
                    
                    // No, creates the storage place for the method
                    $this->_methods[ $value ] = array();
                }
                
                // Registers the method name
                $this->_methods[ $value ][ $className ]              = true;
                $this->_classes[ $className ][ 'methods' ][ $value ] = true;
                
                // Moves the pointer at the end of the methods array
                // This will make sure we get the method from the parent class inherited last
                end( $this->_methods[ $value ] );
                
            }
            
        } else {
            
            // Gets the methods from the reflection object
            $methods    = $reflection->getMethods();
            
            // Process each method
            foreach( $methods as $key => $value ) {
                
                // Checks if another parent class already registered the method
                if( !isset( $this->_methods[ $value->name ] ) ) {
                    
                    // No, creates the storage place for the method
                    $this->_methods[ $value->name ] = array();
                }
                
                // Registers the method name
                $this->_methods[ $value->name ][ $className ]              = true;
                $this->_classes[ $className ][ 'methods' ][ $value->name ] = true;
                
                // Moves the pointer at the end of the methods array
                // This will make sure we get the method from the parent class inherited last
                end( $this->_methods[ $value->name ] );
            }
        }
        
        // Checks if only some properties should be inherited
        if( count( $properties ) ) {
            
            // Process each specified property
            foreach( $properties as $key => $value ) {
                
                // Checks if the property exist
                if( !$reflection->hasProperty( $value ) ) {
                    
                    // No, process the next property
                    // Error management?
                    continue;
                }
                
                // Checks if another parent class already registered the property
                if( !isset( $this->_properties[ $value ] ) ) {
                    
                    // No, creates the storage place for the property
                    $this->_properties[ $value ] = array();
                }
                
                // Registers the property name
                $this->_properties[ $value ][ $className ]              = true;
                $this->_classes[ $className ][ 'properties' ][ $value ] = true;
                
                // Moves the pointer at the end of the properties array
                // This will make sure we get the property from the parent class inherited last
                end( $this->_properties[ $value ] );
            }
            
        } else {
            
            // Gets the properties from the reflection object
            $properties = $reflection->getProperties();
            
            // Process each property
            foreach( $properties as $key => $value ) {
                
                // Checks if another parent class already registered the property
                if( !isset( $this->_properties[ $value->name ] ) ) {
                    
                    // No, creates the storage place for the property
                    $this->_properties[ $value->name ] = array();
                }
                
                // Registers the property name
                $this->_properties[ $value->name ][ $className ]              = true;
                $this->_classes[ $className ][ 'properties' ][ $value->name ] = true;
                
                // Moves the pointer at the end of the properties array
                // This will make sure we get the property from the parent class inherited last
                end( $this->_properties[ $value->name ] );
            }
        }
        
        return true;
    }

    protected function _releaseExtends( $className )
    {
        // Checks if the requested parent class has been inherited from 
        if( isset( $this->_instances[ $className ] ) ) {
            
            // Deletes the instance
            unset( $this->_instances[ $className ] );
            
            // Process each method of the parent class
            foreach( $this->_classes[ $className ][ 'methods' ] as $key => $value ) {
                
                // The method is no longer available from the parent class
                unset( $this->_methods[ $key ][ $className ] );
                
                // Checks if there are still other classes that have the method
                if( !count( $this->_methods[ $key ] ) ) {
                    
                    // No, removes the method from the list
                    unset( $this->_methods[ $key ] );
                    
                } else {
                    
                    // Moves the pointer at the end of the methods array
                    // This will make sure we get the method from the parent class inherited last
                    end( $this->_methods[ $key ] );
                }
            }
            
            // Process each property of the parent class
            foreach( $this->_classes[ $className ][ 'properties' ] as $key => $value ) {
                
                // The property is no longer available from the parent class
                unset( $this->_properties[ $key ][ $className ] );
                
                // Checks if there are still other classes that have the property
                if( !count( $this->_properties[ $key ] ) ) {
                    
                    // No, removes the property from the list
                    unset( $this->_properties[ $key ] );
                    
                } else {
                    
                    // Moves the pointer at the end of the properties array
                    // This will make sure we get the property from the parent class inherited last
                    end( $this->_properties[ $key ] );
                }
            }
            
            // Removes the classes informations
            unset( $this->_classes[ $className ] );
        }
        
        return true;
    }
}
