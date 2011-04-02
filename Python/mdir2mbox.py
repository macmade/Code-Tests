#!/usr/bin/python 
# -*- coding: utf-8 -*-

import mailbox
import sys
import email

mdir    = mailbox.Maildir( sys.argv [ -2 ], email.message_from_file )
outfile = file( sys.argv[ -1 ], 'w' )

for mdir_msg in mdir:
    msg = email.message_from_string( str( mdir_msg ) )
    outfile.write( str( msg ) )
    outfile.write( '\n' )

outfile.close()
