#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sysctl.h>
#include <pwd.h>
#include <grp.h>
#import <Foundation/Foundation.h>

@interface Process: NSObject
{
@protected
    
    NSInteger   _pid;       /* Process ID */
    NSInteger   _uid;       /* User UID */
    NSInteger   _gid;       /* Group ID */
    NSString  * _name;      /* Process name */
    NSString  * _owner;     /* User name */
    NSString  * _group;     /* Group name */
    NSUInteger  _paddr;     /* Swap address */
    
    NSInteger   _ppid;      /* Parent process ID */
    NSString  * _command;   /* Launch command */
    NSDate    * _lstart;    /* Start time */
    CGFloat     _cpu;       /* CPU usage */
    NSInteger   _pri;       /* Scheduling priority */
    NSInteger   _rss;       /* Real memory / KB */
    NSInteger   _vsz;       /* Virtual size / KB */
    CGFloat     _mem;       /* Memory usage */
    NSInteger   _nice;      /* Nice value */
    
@private
    
    id _Process_Reserved[ 5 ] __attribute__( ( unused ) );
}

@property( atomic, readonly ) NSInteger     pid;
@property( atomic, readonly ) NSInteger     uid;
@property( atomic, readonly ) NSInteger     gid;
@property( atomic, readonly ) NSString *    name;
@property( atomic, readonly ) NSString *    owner;
@property( atomic, readonly ) NSString *    group;

+ ( NSArray * )allProcesses;

@end

static OSStatus __get_processes( struct kinfo_proc ** processes, size_t * count );
static OSStatus __get_processes( struct kinfo_proc ** processes, size_t * count )
{
    int                 err;
    struct kinfo_proc * proc;
    int                 name[ 4 ];
    size_t              length;
    
    proc        = NULL;
    name[ 0 ]   = CTL_KERN;
    name[ 1 ]   = KERN_PROC;
    name[ 2 ]   = KERN_PROC_ALL;
    name[ 3 ]   = 0;
    length      = 0;
    
    *( processes )  = NULL;
    *( count )      = 0;
    
    err = sysctl( name, 3, NULL, &length, NULL, 0 );
    
    if( err == 0 )
    {
        if( NULL == ( proc = ( struct kinfo_proc * )malloc( length ) ) )
        {
            err = ENOMEM;
        }
        else
        {
            err = sysctl( name, 3, proc, &length, NULL, 0 );
        
            if( err == 0 )
            {
                *( count ) = length / sizeof( struct kinfo_proc );
            }
            else
            {
                free( proc );
                
                err  = errno;
                proc = NULL;
            }
        }
    }
    else
    {
        err = errno;
    }
    
    *( processes ) = proc;
    
    return err;
}

@interface Process( Private )

- ( id )initWithProc: ( struct kinfo_proc * )proc;

@end

@implementation Process( Private )

- ( id )initWithProc: ( struct kinfo_proc * )proc
{
    struct passwd * user;
    struct group  * group;
    
    if( proc == NULL || proc->kp_proc.p_comm == NULL )
    {
        return nil;
    }
    
    if( ( self = [ super init ] ) )
    {
        _pid   = ( NSInteger )( proc->kp_proc.p_pid );
        _uid   = ( NSInteger )( proc->kp_eproc.e_ucred.cr_uid );
        _gid   = ( NSInteger )( proc->kp_eproc.e_ucred.cr_gid );
        _ppid  = ( NSInteger )( proc->kp_eproc.e_ppid );
        _name  = [ [ NSString alloc ] initWithCString: proc->kp_proc.p_comm encoding: NSUTF8StringEncoding ];
        _paddr = ( NSUInteger )( proc->kp_eproc.e_paddr );
        
        if( ( user = getpwuid( proc->kp_eproc.e_ucred.cr_uid ) ) )
        {
            _owner = [ [ NSString alloc ] initWithCString: user->pw_name encoding: NSUTF8StringEncoding ];
        }
        
        if( ( group = getgrgid( proc->kp_eproc.e_ucred.cr_gid ) ) )
        {
            _group = [ [ NSString alloc ] initWithCString: group->gr_name encoding: NSUTF8StringEncoding ];
        }
    }
    
    return self;
}

@end

@implementation Process

@synthesize pid   = _pid;
@synthesize uid   = _uid;
@synthesize gid   = _gid;
@synthesize name  = _name;
@synthesize owner = _owner;
@synthesize group = _group;

+ ( NSArray * )allProcesses
{
    NSMutableArray    * array;
    Process           * process;
    struct kinfo_proc * processes;
    size_t              count;
    size_t              i;
    
    processes = NULL;
    count     = 0;
    
    if( __get_processes( &processes, &count ) )
    {
        return nil;
    }
    
    array = [ NSMutableArray arrayWithCapacity: count ];
    
    for( i = 0; i < count; i++ )
    {
        process = [ [ Process alloc ] initWithProc: &( processes[ ( count - i ) - 1 ] ) ];
        
        if( process != nil )
        {
            [ array addObject: process ];
        }
    }
    
    free( processes );
    
    return [ NSArray arrayWithArray: array ];
}

- ( void )dealloc
{
    [ _name  release ];
    [ _owner release ];
    [ _group release ];
    
    [ super dealloc ];
}

- ( NSString * )description
{
    return [ NSString stringWithFormat: @"%@: %i - %@ (%i:%i / %@:%@)", [ super description ], _pid, _name, _uid, _gid, _owner, _group ];
}

@end

int main( void )
{
    @autoreleasepool
    {
        NSLog( @"%@", [ Process allProcesses ] );
    }
    
    return 0;
}
