/*******************************************************************************
 * Copyright (c) 2010, Jean-David Gadina - www.xs-labs.com
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  -   Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *  -   Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  -   Neither the name of 'Jean-David Gadina' nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#import <Foundation/Foundation.h>
#include <sys/acl.h>

typedef enum
{
    ACLEntryTypeUnknown = ACL_UNDEFINED_TAG,
    ACLEntryTypeAllow   = ACL_EXTENDED_ALLOW,
    ACLEntryTypeDeny    = ACL_EXTENDED_DENY
}
ACLEntryType;

@interface ACLEntry: NSObject
{
@protected
    
    NSString   * _guid;
    ACLEntryType _type;
	BOOL         _readData;
	BOOL         _listDirectory;
	BOOL         _writeData;
	BOOL         _addFile;
	BOOL         _execute;
	BOOL         _search;
	BOOL         _delete;
	BOOL         _appendData;
	BOOL         _addSubDirectory;
	BOOL         _deleteChild;
	BOOL         _readAttributes;
	BOOL         _writeAttributes;
	BOOL         _readExtendedAttributes;
	BOOL         _writeExtendedAttributes;
	BOOL         _readSecurity;
	BOOL         _writeSecurity;
	BOOL         _changeOwner;
    
@private
    
    id _ACLEntry_Reserved[ 5 ] __attribute__( ( unused ) );
}

@property( atomic, readonly ) NSString   * guid;
@property( atomic, readonly ) ACLEntryType type;
@property( atomic, readonly ) BOOL         readData;
@property( atomic, readonly ) BOOL         listDirectory;
@property( atomic, readonly ) BOOL         writeData;
@property( atomic, readonly ) BOOL         addFile;
@property( atomic, readonly ) BOOL         execute;
@property( atomic, readonly ) BOOL         search;
@property( atomic, readonly ) BOOL         delete;
@property( atomic, readonly ) BOOL         appendData;
@property( atomic, readonly ) BOOL         addSubDirectory;
@property( atomic, readonly ) BOOL         deleteChild;
@property( atomic, readonly ) BOOL         readAttributes;
@property( atomic, readonly ) BOOL         writeAttributes;
@property( atomic, readonly ) BOOL         readExtendedAttributes;
@property( atomic, readonly ) BOOL         writeExtendedAttributes;
@property( atomic, readonly ) BOOL         readSecurity;
@property( atomic, readonly ) BOOL         writeSecurity;
@property( atomic, readonly ) BOOL         changeOwner;

+ ( NSArray * )entriesForFile: ( NSString * )path;
- ( id )initWithACLEntry: ( acl_entry_t )entry;

@end

@implementation ACLEntry

@synthesize guid                    = _guid;
@synthesize type                    = _type;
@synthesize readData                = _readData;
@synthesize listDirectory           = _listDirectory;
@synthesize writeData               = _writeData;
@synthesize addFile                 = _addFile;
@synthesize execute                 = _execute;
@synthesize search                  = _search;
@synthesize delete                  = _delete;
@synthesize appendData              = _appendData;
@synthesize addSubDirectory         = _addSubDirectory;
@synthesize deleteChild             = _deleteChild;
@synthesize readAttributes          = _readAttributes;
@synthesize writeAttributes         = _writeAttributes;
@synthesize readExtendedAttributes  = _readExtendedAttributes;
@synthesize writeExtendedAttributes = _writeExtendedAttributes;
@synthesize readSecurity            = _readSecurity;
@synthesize writeSecurity           = _writeSecurity;
@synthesize changeOwner             = _changeOwner;

+ ( NSArray * )entriesForFile: ( NSString * )path
{
    acl_t            acl;
    acl_entry_t      ace;
    NSMutableArray * array;
    ACLEntry       * entry;
    
    acl   = acl_get_file( [ path cStringUsingEncoding: NSUTF8StringEncoding ], ACL_TYPE_EXTENDED );
    entry = NULL;
    
    if( acl == NULL )
    {
        return nil;
    }
    
    array = [ NSMutableArray arrayWithCapacity: ACL_MAX_ENTRIES ];
    
    while( acl_get_entry( acl, ACL_NEXT_ENTRY, &ace ) == 0 )
    {
        entry = [ [ ACLEntry alloc ] initWithACLEntry: ace ];
        
        if( entry != nil )
        {
            [ array addObject: entry ];
            [ entry release ];
        }
    }
    
    acl_free( acl );
    
    return [ NSArray arrayWithArray: array ];
}

- ( id )initWithACLEntry: ( acl_entry_t )entry
{
    unsigned int     i;
    acl_permset_t    permset;
    acl_tag_t        tag;
    guid_t         * guid;
    char           * guid_s[ KAUTH_GUID_SIZE + 5 ]; /* 00000000-0000-0000-0000-000000000000 */
    char             guid_c[ 3 ];
    
    if( ( self = [ super init ] ) && entry != NULL )
    {
        tag     = 0;
        permset = NULL;
        guid    = acl_get_qualifier( entry );
        
        acl_get_tag_type( entry, &tag );
        acl_get_permset( entry, &permset );
        
        if( tag == ACL_EXTENDED_ALLOW )
        {
            _type = ACLEntryTypeAllow;
        }
        else if( tag == ACL_EXTENDED_DENY )
        {
            _type = ACLEntryTypeDeny;
        }
        else
        {
            _type = ACLEntryTypeUnknown;
        }
        
        if( permset != NULL )
        {
            _readData                   = ( BOOL )acl_get_perm_np( permset, ACL_READ_DATA );
            _listDirectory              = ( BOOL )acl_get_perm_np( permset, ACL_LIST_DIRECTORY );
            _writeData                  = ( BOOL )acl_get_perm_np( permset, ACL_WRITE_DATA );
            _addFile                    = ( BOOL )acl_get_perm_np( permset, ACL_ADD_FILE );
            _execute                    = ( BOOL )acl_get_perm_np( permset, ACL_EXECUTE );
            _search                     = ( BOOL )acl_get_perm_np( permset, ACL_SEARCH );
            _delete                     = ( BOOL )acl_get_perm_np( permset, ACL_DELETE );
            _appendData                 = ( BOOL )acl_get_perm_np( permset, ACL_APPEND_DATA );
            _addSubDirectory            = ( BOOL )acl_get_perm_np( permset, ACL_ADD_SUBDIRECTORY );
            _deleteChild                = ( BOOL )acl_get_perm_np( permset, ACL_DELETE_CHILD );
            _readAttributes             = ( BOOL )acl_get_perm_np( permset, ACL_READ_ATTRIBUTES );
            _writeAttributes            = ( BOOL )acl_get_perm_np( permset, ACL_WRITE_ATTRIBUTES );
            _readExtendedAttributes     = ( BOOL )acl_get_perm_np( permset, ACL_READ_EXTATTRIBUTES );
            _writeExtendedAttributes    = ( BOOL )acl_get_perm_np( permset, ACL_WRITE_EXTATTRIBUTES );
            _readSecurity               = ( BOOL )acl_get_perm_np( permset, ACL_READ_SECURITY );
            _writeSecurity              = ( BOOL )acl_get_perm_np( permset, ACL_WRITE_SECURITY );
            _changeOwner                = ( BOOL )acl_get_perm_np( permset, ACL_CHANGE_OWNER );
        }
        
        memset( guid_s, 0, KAUTH_GUID_SIZE + 5 );
        memset( guid_c, 0, 3 );
        
        for( i = 0; i < KAUTH_GUID_SIZE; i++ )
        {
            if( i == 4 || i == 6 || i == 8 || i == 10 )
            {
                strcat( ( char * )guid_s, "-" );
            }
            
            sprintf( ( char * )guid_c, "%02X", ( unsigned int )( guid->g_guid[ i ] ) );
            strcat( ( char * )guid_s, ( char * )guid_c );
        }
        
        _guid = [ [ NSString alloc ] initWithCString: ( char * )guid_s encoding: NSASCIIStringEncoding ];
        
        acl_free( guid );
    }
    
    return self;
}

- ( void )dealloc
{
    [ _guid release ];
    [ super dealloc ];
}

- ( NSString * )description
{
    return [ NSString stringWithFormat: @"%@: %@", [ super description ], _guid ];
}

@end

int main( int argc, char * argv[] )
{
    NSArray  * entries;
    ACLEntry * entry;
    
    @autoreleasepool
    {
        if( argc < 2 )
        {
            printf( "Usage: %s FILE\n", argv[ 0 ] );
            return EXIT_FAILURE;
        }
        
        entries = [ ACLEntry entriesForFile: [ NSString stringWithCString: argv[ 1 ] encoding: NSUTF8StringEncoding ] ];
        
        for( entry in entries )
        {
            NSLog
            (
                @"\n"
                @"Type:                         %@\n"
                @"GUID:                         %@\n"
                @"-----\n"
                @"Read data:                    %@\n"
                @"List directory:               %@\n"
                @"Write data:                   %@\n"
                @"Add file:                     %@\n"
                @"Execute:                      %@\n"
                @"Search:                       %@\n"
                @"Delete:                       %@\n"
                @"Append data:                  %@\n"
                @"Add sub directory:            %@\n"
                @"Delete child:                 %@\n"
                @"Read attributes:              %@\n"
                @"Write attributes:             %@\n"
                @"Read extended attributes      %@\n"
                @"Write extended attributes:    %@\n"
                @"Read security:                %@\n"
                @"Write security:               %@\n"
                @"Change owner:                 %@\n"
                "\n",
                ( entry.type == ACLEntryTypeAllow ) ? @"Allow" : ( ( entry.type == ACLEntryTypeDeny ) ? @"Deny" : @"Unknown" ),
                entry.guid,
                ( entry.readData )                  ? @"Yes" : @"No",
                ( entry.listDirectory )             ? @"Yes" : @"No",
                ( entry.writeData )                 ? @"Yes" : @"No",
                ( entry.addFile )                   ? @"Yes" : @"No",
                ( entry.execute )                   ? @"Yes" : @"No",
                ( entry.search )                    ? @"Yes" : @"No",
                ( entry.delete )                    ? @"Yes" : @"No",
                ( entry.appendData )                ? @"Yes" : @"No",
                ( entry.addSubDirectory )           ? @"Yes" : @"No",
                ( entry.deleteChild )               ? @"Yes" : @"No",
                ( entry.readAttributes )            ? @"Yes" : @"No",
                ( entry.writeAttributes )           ? @"Yes" : @"No",
                ( entry.readExtendedAttributes )    ? @"Yes" : @"No",
                ( entry.writeExtendedAttributes )   ? @"Yes" : @"No",
                ( entry.readSecurity )              ? @"Yes" : @"No",
                ( entry.writeSecurity )             ? @"Yes" : @"No",
                ( entry.changeOwner )               ? @"Yes" : @"No"
            );
        }
    }
    
    return EXIT_SUCCESS;
}
