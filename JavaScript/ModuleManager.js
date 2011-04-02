// <![CDATA[

// $Id: ModuleManager.js 57 2009-01-30 05:52:42Z macmade $

var ModuleManager = null;

( function()
{
    var _instance = null;
    
    ModuleManager = function ModuleManager()
    {
        if( _instance ) {
            
            return _instance;
        }
        
        var _moduleClasses = new Array();
        
        this.registerModuleClass = function( moduleName, moduleClass )
        {
            if( _moduleClasses[ moduleName ] === undefined ) {
                
                _moduleClasses[ moduleName ] = new moduleClass();
                
            } else {
                
                throw new Error( 'Module \'' + moduleName + '\' has already been registered.' );
            }
        }
        
        this.getModule = function( moduleName )
        {
            if( _moduleClasses[ moduleName ] === undefined ) {
                
                // Maybe this should return an empty object instead of throwing
                // an error
                throw new Error( 'Module \'' + moduleName + '\' has not been registered yet.' );
                
            } else {
                
                return _moduleClasses[ moduleName ];
            }
        }
        
        _instance = this;
    }
    
    ModuleManager.getInstance = function ()
    {
        if ( _instance === null ) {
            
            _instance = new ModuleManager();
        }
        
        return _instance;
    }
    
} )();

ModuleManager.getInstance().registerModuleClass(
    'test',
    function()
    {
        this.test = function()
        {
            alert( 'This is a test...' );
        }
    }
);

myModule = ModuleManager.getInstance().getModule( 'test' ).test();

// ]]>
